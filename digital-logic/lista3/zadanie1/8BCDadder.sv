module bit8bcdadder(
  input[7:0] a,b,
  input c0,
  output[7:0] o,
);
  logic[3:0] sum1,sum2;
  
  BCDadder B0(a[3:0],b[3:0],c0,sum1,c1);
  
  BCDadder B1(a[7:4],b[7:4],c1,sum2,carry);
  
  assign o = {sum2,sum1};
  
endmodule
