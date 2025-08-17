
// Ram z synchronicznym odczytem i zapisem
module Memory(
  input clk, wr, rd,
  input [2:0] wraddr, rdaddr,
  input [7:0] datain,
  output [7:0] dataout
);
  logic [7:0] mem [7:0];
  integer k;
  initial
    for (k = 0; k < 8; k = k + 1)
      mem[k] = 8-k;

  always_ff @(posedge clk)
    if(rd)
      dataout <= mem[rdaddr];
  always_ff @(posedge clk)
    if(wr)
      mem[wraddr] <= datain;
endmodule

// c   <- nie jest rejestrem (wyjście dataout, ponieważ odczyt jest synchroniczny)
// i,j <- liczniki
// m   <- minimalny element (rejestr)
// jm  <- adres m (rejestr)

// Moduł dane posiada moduł (Adres) automatycznie ustalający adresy dla pamięci

module Dane(
  input clk,
  input [7:0] control, dataout,
  output [3:0] sdane,
  output [15:0] aadr
);
  logic [2:0] i, j, jm;
  logic [7:0] m;
// control = {swap,end&!i==jm,end&i==jm,j==7,c<m,inner,outer&!i==7,ready&start}
  always_ff @(posedge clk)
    if(control[0]) i <= 3'b0;
    else if(control[1]) begin
      j  <= i + 1;
      jm <= i;
      m  <= dataout;
      end
    else if(control[2]) begin
      if (control[3]) begin
        m  <= dataout;
        jm <= j;
        end
      if (!control[4]) j <= j + 1;
      end
    else if(control[5] | control[7])
      i <= i + 1;
   Adres A0(j,i,jm,control,m,dataout,aadr);
   
// Sygnały dla ścieżki sterowania
  always_comb begin
    sdane = 4'b0;
    if(i == 7)
      sdane[0] = 1'b1;
    if(dataout < m)
      sdane[1] = 1'b1;
    if(j == 7)
      sdane[2] = 1'b1;
    if(jm == i)
      sdane[3] = 1'b1;
  end
endmodule

// Ustala adresy i sygnały wr rd dla automatu
module Adres(
  input [2:0] j, i, jm,
  input [7:0] control, m, c,
  output [15:0] aadr
);
  logic [2:0] id,jd;
//  Ogólnie u/aadr = {wr, rd, wraddr, rdaddr, datain}
  assign jd = j + 1;
  assign id = i + 1;
  always_comb begin
    aadr = 15'b0;
    if(control[0])
      aadr[14] = 1'b1;
    else if(control[1]) begin
      aadr[14] = 1'b1;
      aadr[10:8] = id;
      end
    else if(control[2]&!control[4]) begin
      aadr[14] = 1'b1;
      aadr[10:8] = jd;
      end
    else if(control[2]&control[4]) begin
      aadr[14] = 1'b1;
      aadr[10:8] = i;
      end
    else if(control[5]) begin
      aadr[14] = 1'b1;
      aadr[10:8] = id;
      end
    else if(control[6]) begin 
      aadr[15] = 1'b1;
      aadr[13:11] = jm;
      aadr[7:0] = c;
      end
    else if(control[7]) begin
      aadr[15:14] = 2'b11;
      aadr[13:8] = {i,id};
      aadr[7:0] = m;
      end
  end
endmodule

// Jeden rejestr (automat) 5 stanów, kilka sygnałów sterujących
module Sterowanie(
  input clk, nrst, start,
  input [3:0] sdane,
  output ready,
  output [7:0] control
);
  logic [2:0] status;
  const logic [2:0] 
    RDY = 3'b001,
    OUT = 3'b010,
    INN = 3'b100,
    SWP = 3'b110,
    END = 3'b000;
    
  assign ready = status[0];
// Automat
// sdane[0] <- i == 7;
// sdane[1] <- c < m;
// sdane[2] <- j == 7;
// sdane[3] <- i == jm;
  always_ff @(posedge clk or negedge nrst)
    if(!nrst)
      status <= RDY;
    else unique case(status)
      RDY : if (start) status <= OUT;
      OUT : if (sdane[0]) status <= RDY;
            else status <= INN;
      INN : if (sdane[2]) status <= END;
      END : if (sdane[3]) status <= OUT;
            else status <= SWP;
      SWP : status <= OUT;
    endcase
    
// Sygnały sterujące
// control = {swap,end&!i==jm,end&i==jm,j==7,c<m,inner,outer&!i==7,ready&start}


  assign control[0] = ready&start;
  assign control[1] = status[1]&!status[2]&!sdane[0];
  assign control[2] = status[2]&!status[1];
  assign control[3] = sdane[1];
  assign control[4] = sdane[2];
  assign control[5] = sdane[3]&!status;
  assign control[6] = !sdane[3]&!status;
  assign control[7] = &status[2:1];

endmodule


// Moduł ustalający kto ma dostęp do pamięci na podstawie ready i start
module Dostep(
  input ready, start,
  input [15:0] uadr, aadr,
  output wr, rd,
  output [2:0] wraddr, rdaddr,
  output [7:0] datain
);
//  Ogólnie u/aadr = {wr, rd, wraddr, rdaddr, datain}
  assign {wr,rd,wraddr,rdaddr,datain} = !start&ready ? uadr : aadr;
endmodule


module Sorter(
  input clk, nrst, start, wr,
  input [2:0] addr,
  input [7:0] datain,
  output ready,
  output [7:0] dataout
);
  logic owr,ord;
  logic [2:0] owraddr, ordaddr;
  logic [3:0] sdane;
  logic [7:0] odatain, control;
  logic [15:0] aadr;

  Dostep M0(ready,start,{wr, 1'b1, addr, addr, datain},aadr,owr,ord,owraddr,ordaddr,odatain);
  Memory RAM(clk, owr, ord, owraddr, ordaddr, odatain, dataout);
  Dane D0(clk,control,dataout,sdane,aadr);
  Sterowanie S0(clk,nrst,start,sdane,ready,control);
endmodule
