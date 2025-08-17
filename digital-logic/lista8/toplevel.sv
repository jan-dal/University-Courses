module Licznik(
  input clk,load,rst,
  input[15:0] d,
  output[15:0] cnt
);
  logic[15:0] q;

  always_ff @(posedge clk)
    if(load) q <= d;
    else if(rst) q <= 16'b0;
    else q <= q + 16'b1;
  assign q = cnt;

endmodule

module Rejestr(
  input clk,load,
  input[15:0] d,
  output[15:0] cmp
);
  logic[15:0] q;
  
  always_ff @(posedge clk)
    if(load) q <= d;
  
  assign cmp = q;
endmodule


module Komb(
  input[15:0] cnt,cmp,top,
  output rst,out
);
  logic q;
  
  always_comb begin
    if(cnt >= top)
      rst = 1'b1;
    else
      rst = 1'b0;
  end
  
  always_comb begin
    if(cnt >= cmp)
      out = 1'b0;
    else
      out = 1'b1;
  end  
endmodule

module Main(
  input clk,
  input[1:0] sel,
  input[15:0] d,	
  output out,
  output[15:0] cmp,cnt,top
);
  logic[2:0] k;
  
  always_comb begin
    case(sel)
      2'b01  : k = 3'b001;
      2'b10  : k = 3'b010;
      2'b11  : k = 3'b100;
      default: k = 3'b000;
    endcase
  end
  
  Licznik L0(clk,k[2],rst,d,cnt);
  Rejestr RP(clk,k[0],d,cmp);
  Rejestr RWS(clk,k[1],d,top);
  
  Komb K0(cnt,cmp,top,rst,out);
endmodule
