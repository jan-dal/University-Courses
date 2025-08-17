module shifter(
  input[3:0] i,
  input l,r,
  output[3:0] o
);
  logic no;
  assign no = !(l||r);
  
  assign o[0] = i[1]&&r || i[0]&&no;
  assign o[1] = i[0]&&l || i[2]&&r || i[1]&&no;
  assign o[2] = i[1]&&l || i[3]&&r || i[2]&&no;
  assign o[3] = i[2]&&l || i[3]&&no;
  
  
  
endmodule
