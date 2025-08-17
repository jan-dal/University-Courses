// Moduł arytmetyczny wykonujący obliczenia :
// Mnożenie a * x lub x * x
// Odejmowanie n - 1
// Dzielenie n / 2

module arith(
  input NP,
  input [15:0] a, x,
  input [7:0] n,
  output [7:0] nq,
  output [15:0] aq, xq
);
  logic [15:0] kq,k;
  assign k = NP ? a : x;
  assign kq = k*x;
  assign {xq,aq} = NP ? {x,kq} : {kq,a};
  assign nq = NP ? n - 1 : n >> 1;
endmodule


module Main(
  input clk, nrst,start,
  input [7:0] inn,
  input [15:0] inx,
  output ready,
  output logic [15:0] out
);
  const logic RDY = 'b1;
  const logic BSY = 'b0;
  logic [7:0] n;
  logic [15:0] aq,xq,nq,x,a;
  
  assign nzero = |n;  //Czy n nie jest 0?
  assign NP = n[0];   //Czy n nieparzyste?
  arith obliczenia(NP,out,x,n,nq,aq,xq);
  
  always_ff @(posedge clk or negedge nrst)
    if (!nrst)
      ready <= RDY;
  else case(ready)
    RDY : if (start) begin
      out   <= 16'b1;
      x     <= inx;
      n     <= inn;
      ready <= BSY;
    end
    BSY : if (nzero) begin
      x   <= xq;
      n   <= nq;
      out <= aq;
    end else
      ready <= RDY;
   endcase
endmodule
