module multiplexer(
  input[7:0] a,b,
  input s,
  output[7:0] o
);
  
  assign o[0] = (a[0]&&!s)||(b[0]&&s);
  assign o[1] = (a[1]&&!s)||(b[1]&&s);
  assign o[2] = (a[2]&&!s)||(b[2]&&s);
  assign o[3] = (a[3]&&!s)||(b[3]&&s);
  assign o[4] = (a[4]&&!s)||(b[4]&&s);
  assign o[5] = (a[5]&&!s)||(b[5]&&s);
  assign o[6] = (a[6]&&!s)||(b[6]&&s);
  assign o[7] = (a[7]&&!s)||(b[7]&&s);
  
endmodule
