// RAM z odczytem asynchronicznym i zapisem synchronicznym
module memory(
  input step,wr,
  input [9:0] rdaddr, wraddr,
  input [15:0] in,
  output [15:0] out
);
  logic [15:0] mem [0:1023];
  
  always_ff @(posedge step)
    if (wr) mem[wraddr] <= in;
  assign out = mem[rdaddr];
endmodule

// Rejestr z ładowaniem synchronicznym
module register(
  input step,load,nrst,
  input [15:0] in,
  output logic [15:0] q
);
  always_ff @(posedge step or negedge nrst)
    if (!nrst)
      q <= 16'b0;
    else if(load)
      q <= in;
    else
      q <= q;
endmodule

// Licznik z predykatami wartości licznika 0?/1?/max?
module counter(
  input step,nrst,en,
  input sel,
  output empty,one,max,
  output logic [9:0] q
);
  always_ff @(posedge step or negedge nrst)
    if (!nrst)
      q <= 10'b0;
    else if(en)
      unique case(sel)
        2'b1 :  q <= q + 1;
        2'b0 :  q <= q - 1;
      endcase
  assign empty = ~|q;
  assign one = ~|q[9:1]&q[0];
  assign max = &q;
endmodule

module arith(
  input push,
  input [2:0] op,
  input signed[15:0] a,b,d,
  output signed[15:0] top
);
  logic g;
  logic [15:0] add,mult,sub;
  assign add  = a + b;
  assign mult = a * b;
  assign sub  = -a;
  assign g = (0 < a) ? 'b1 : 'b0;
  
  //Kontrola wejść danych szczytu stosu
  always_comb begin
    if(push)
      top = d;
    else
      unique case(op)
        3'b000 : top = g;
        3'b001 : top = sub;
        3'b010 : top = add;
        3'b011 : top = mult;
        default : top = b;
      endcase
  end
  
endmodule

module adress(
  input push,
  input [2:0] op,
  input [9:0] cnt,
  input [15:0] out,
  output [9:0] rdaddr,wraddr 
);
  logic [9:0] c1,c2,cn;
  //Ustawia adresy zapisu/odczytu
  //Chęć przeprowadzenia operacji binarnej:
  //rdaddr = wraddr => pierwszy element przed szczytem stosu
  //(element na szczycie w rejestrze)
  //rdaddr => aktualna ilość elementów stosu - 1 w p.p.
  //Operacja unarna wraddr => ilość el. - 1
  //Default (push) => wraddr = ilość el. stosu
  assign c1 = cnt - 1;
  assign c2 = cnt - 2;
  assign cn = c2 - out;
  always_comb begin
    if(push)
      {rdaddr,wraddr} = {10'bx,c1};
    else case(op)
      3'b000 : {rdaddr,wraddr} = 20'bx;
      3'b001 : {rdaddr,wraddr} = 20'bx;
      3'b010 : {rdaddr,wraddr} = {c2, 10'bx};
      3'b011 : {rdaddr,wraddr} = {c2, 10'bx};
      3'b100 : {rdaddr,wraddr} = {c2, c2};
      3'b101 : {rdaddr,wraddr} = {cn, 10'bx};
      3'b110 : {rdaddr,wraddr} = {c2, 10'bx};
      3'b111 : {rdaddr,wraddr} = {c2, 10'bx};
    endcase
  end
endmodule

module kalkulator(
  input nrst,step, push, en,
  input [2:0] op,
  input [15:0] d,
  output [9:0] cnt,
  output [15:0] out
);
  logic wr,empty,one,max,bin,un;
  logic [9:0] wraddr,rdaddr;
  logic [15:0] memout,top,add,mult,sub;
  
  //wrmem - pozwala na zapis w pamięci RAM/zapis w rejestrze/zwiększenie lub zmniejszenie licznika
  //empty - wskazuje czy stos jest pusty
  //one - wskazuje czy na stosie znajduje się 1 i tylko 1 element
  //max - wskazuje czy limit stosu został osiągnięty
  //sel - wybiera operację dla licznika
  //wraddr,rdaddr - adresy odczytu/zapisu
  //memout - wyjście pamięci RAM
  //top - wejście rejsetru
  
  // ------------------
  //  Tabela sygnałów 
  // ------------------
  //	op		rdaddr		wraddr		memdatain		regdatain		licznik
  //	-		  x			  x  		    x 			  -reg      	   0
  //	+		cnt-2		  x							reg+mem[n-1]      -1
  //	*		cnt-2 		  x							reg*mem[n-1]      -1
  //	g		  x  		  x							  0 or 1           0
  //	l	 cnt-mem[n]-1     x							 mem[n-p-1]        0
  //	s		cnt-2 		cnt-2		   reg			 mem[n-1]          0
  //	p		cnt-2 		  x						   	 mem[n-1]         -1
  //	push	  x	 		cnt-1		   reg			 datain           +1
  
  
  // Kontrola zapisu/załadowania
  // Nie pozwala na przepełnienie stosu
  // Zapis do pamięci tylko w przypadku s lub push&!max
  // Zapis do rejestru tylko w przypadku en = 1 i !max i (!empty || push)
  // Ustawienie licznika tylko gdy en = 1 i przeprowadzamy operację  

  
  always_comb begin
    memwr = 'b0;
    if(en)
      if(push&!max&!empty)
        memwr = 'b1;
      else if(!empty&!one&!op[0]&!op[1]&op[2])
        memwr = 'b1;
  end
  
  always_comb begin
    len = 'b0;
    if(en)
      if(push&!max)
        len = 'b1;
      else
        case(op)
          3'b010 : if(!empty&!one) len = 'b1;
          3'b011 : if(!empty&!one) len = 'b1;
          3'b110 : if(!empty) len = 'b1;
          3'b111 : if(!empty) len = 'b1;
          default len = 'b0;
        endcase
  end
  
  always_comb begin
    regwr = 'b0;
    if(en)
      if(push&!max)
        regwr = 'b1;
      else
        case(op)
          3'b000 : if(!empty) regwr = 'b1;
          3'b001 : if(!empty) regwr = 'b1;
          3'b010 : if(!empty&!one) regwr = 'b1;
          3'b011 : if(!empty&!one) regwr = 'b1;
          3'b100 : if(!empty&!one) regwr = 'b1;
          3'b101 : if(!empty&!one) regwr = 'b1;
          3'b110 : if(!empty) regwr = 'b1;
          3'b111 : if(!empty) regwr = 'b1;
        endcase
  end
  adress   adresowanie(push,op,cnt,out,rdaddr,wraddr);
  arith    arytmetyka(push,op,out,memout,d,top);
  memory   stos(step,memwr,rdaddr,wraddr,out,memout);
  counter  l_el(step,nrst,len,push,empty,one,max,cnt);
  register szczyt_stosu(step,regwr,nrst,top,out);
endmodule

// Główny moduł kalkulatora programowalnego 

module main(
  input clk,nrst,wr,start,
  input [9:0] addr,
  input [15:0] datain,
  output logic ready,
  output [15:0] out
);
  logic const READY = 'b1, BUSY = 'b0;
  logic push,en;
  logic [2:0] op;
  logic [9:0] ip,addrcntr;
  logic [15:0] dataout,din;
  
  // addrcntr - kontroluje dostęp do pamięci instrukcji
  // din - dane przekazywane do kalkulatora
  // op  - operacje dla kalkulatora
  // ip  - licznik instrukcji
  
  memory codemem(clk,wr,addrcntr,addrcntr,datain,dataout);
  kalkulator calc(nrst,clk,push,en,op,din, ,out);
  
  always_ff @(posedge clk or negedge nrst)
    if(!nrst)
      ready <= 'b1;
    else case(ready)
      READY : if(start) ready <= 'b0;
      BUSY  : if(dataout[15]&dataout[14]) ready <= 'b1;
    endcase
  
  always_ff @(posedge clk or negedge nrst)
    if(!nrst)
      ip <= 10'b0;
    else case(ready)
      READY : ip <= 10'b0;
      BUSY  : if(&{dataout[15],!dataout[14],dataout[2:0]})
                ip <= out;
              else
                ip <= ip + 1;
    endcase

  
  assign addrcntr = ready ? addr : ip;
  assign en = &{!ready,!&dataout[15:14]};
  assign push = !dataout[15] ? 'b1 :'b0;
  assign din = !dataout[15] ? dataout[14:0] : 16'bx;
  assign op = dataout[15]&!dataout[14] ? dataout[2:0] : 3'b0;
  
endmodule
