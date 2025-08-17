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
  input [1:0] sel,
  output empty,one,max,
  output logic [9:0] q
);
  always_ff @(posedge step or negedge nrst)
    if (!nrst)
      q <= 10'b0;
    else if(en)
      case(sel)
        2'b01 :  q <= q + 1;
        2'b10 :  q <= q - 1;
      endcase
  assign empty = ~|q;
  assign one = ~|q[9:1]&q[0];
  assign max = &q;
endmodule

module arith(
  input push,
  input [1:0] op,
  input signed[15:0] a,b,d,
  output signed[15:0] add,mult,sub,top
);
  assign add  = a + b;
  assign mult = a * b;
  assign sub  = -a;
  
  //Kontrola wejść danych szczytu stosu i pamięci
  always_comb begin
    if(push)
      top = d;
    else
      case(op)
        2'b01   : top = sub;
        2'b10   : top = add;
        2'b11   : top = mult;
        default : top = d;
      endcase
  end
  
endmodule

module adress(
  input un,bin,
  input [9:0] cnt,
  output [9:0] rdaddr,wraddr 
);
  //Ustawia adresy zapisu/odczytu
  //Chęć przeprowadzenia operacji binarnej:
  //rdaddr = wraddr => pierwszy element przed szczytem stosu
  //(element na szczycie w rejestrze)
  //rdaddr => aktualna ilość elementów stosu - 1 w p.p.
  //Operacja unarna wraddr => ilość el. - 1
  //Default (push) => wraddr = ilość el. stosu

  always_comb begin
    rdaddr = cnt - 10'b1;
    wraddr = cnt;
    if(bin)
    {wraddr,rdaddr} = {2{cnt - 10'b10}};
    else if (un)
      wraddr = cnt - 10'b1;
  end
endmodule

module main(
  input nrst,step, push,
  input [1:0] op,
  input [15:0] d,
  output [9:0] cnt,
  output [15:0] out
);
  logic wr,empty,one,max,bin,un;
  logic [1:0] sel;
  logic [9:0] wraddr,rdaddr;
  logic [15:0] q,top,add,mult,sub;
  
  //wr - pozwala na zapis w pamięci RAM/zapis w rejestrze/zwiększenie lub zmniejszenie licznika
  //empty - wskazuje czy stos jest pusty
  //one - wskazuje czy na stosie znajduje się 1 i tylko 1 element
  //max - wskazuje czy limit stosu został osiągnięty
  //sel - wybiera operację dla licznika
  //wraddr,rdaddr - adresy odczytu/zapisu
  //q - wyjście pamięci RAM
  //top - wejście pamięci RAM i rejsetru
  //add - wynik operacji dodawania
  //mult - wynik operacji mnożenia
  //sub - wynik operacji obliczającej liczbę przeciwną
  //bin - wskazuje czy można wykonać op binarną jeżeli chcemy ją wykonać
  //un  - analogicznie operację unarną
  
  assign bin = ~|{push,one,empty}&op[1];
  assign un = &{!push,op[0],~|{op[1],empty}};
  
  //Ustawienie zwiększa,zmniejsza lub nie zmienia stanu licznika
  //zwiększa gdy push
  //zmniejsza gdy operacja binarna
  //nie zmienia w p.p.
  assign sel = push ? 2'b01 : bin ? 2'b10 : 2'b00;
  
  // Kontrola zapisu/załadowania
  // Nie pozwala na przepełnienie stosu
  always_comb begin
    if(!max&push|{bin,un})
      wr = 'b1;
    else
      wr = 'b0;
  end
  
  adress   adresowanie(un,bin,cnt,rdaddr,wraddr);
  arith    arytmetyka(push,op,out,q,d,add,mult,sub,top);
  memory   stos(step,wr,rdaddr,wraddr,top,q);
  counter  l_el(step,nrst,wr,sel,empty,one,max,cnt);
  register szczyt_stosu(step,wr,nrst,top,out);
endmodule
