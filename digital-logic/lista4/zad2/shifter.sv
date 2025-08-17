module lv1(
  input[15:0] a,
  input k,
  output[15:0] b
);
  assign b[0] = k? a[1] : a[0];
  assign b[1] = k? a[2] : a[1];
  assign b[2] = k? a[3] : a[2];
  assign b[3] = k? a[4] : a[3];
  assign b[4] = k? a[5] : a[4];
  assign b[5] = k? a[6] : a[5];
  assign b[6] = k? a[7] : a[6];
  assign b[7] = k? a[8] : a[7];
  assign b[8] = k? a[9] : a[8];
  assign b[9] = k? a[10] : a[9];
  assign b[10] = k? a[11] : a[10];
  assign b[11] = k? a[12] : a[11];
  assign b[12] = k? a[13] : a[12];
  assign b[13] = k? a[14] : a[13];
  assign b[14] = k? a[15] : a[14];
  assign b[15] = k? 'b0 : a[15];
endmodule

module lv2(
  input[15:0] a,
  input k,
  output[15:0] b
);
  assign b[0] = k? a[2] : a[0];
  assign b[1] = k? a[3] : a[1];
  assign b[2] = k? a[4] : a[2];
  assign b[3] = k? a[5] : a[3];
  assign b[4] = k? a[6] : a[4];
  assign b[5] = k? a[7] : a[5];
  assign b[6] = k? a[8] : a[6];
  assign b[7] = k? a[9] : a[7];
  assign b[8] = k? a[10] : a[8];
  assign b[9] = k? a[11] : a[9];
  assign b[10] = k? a[12] : a[10];
  assign b[11] = k? a[13] : a[11];
  assign b[12] = k? a[14] : a[12];
  assign b[13] = k? a[15] : a[13];
  assign b[14] = k? 'b0 : a[14];
  assign b[15] = k? 'b0 : a[15];
endmodule

module lv3(
  input[15:0] a,
  input k,
  output[15:0] b
);
  assign b[0] = k? a[4] : a[0];
  assign b[1] = k? a[5] : a[1];
  assign b[2] = k? a[6] : a[2];
  assign b[3] = k? a[7] : a[3];
  assign b[4] = k? a[8] : a[4];
  assign b[5] = k? a[9] : a[5];
  assign b[6] = k? a[10] : a[6];
  assign b[7] = k? a[11] : a[7];
  assign b[8] = k? a[12] : a[8];
  assign b[9] = k? a[13] : a[9];
  assign b[10] = k? a[14] : a[10];
  assign b[11] = k? a[15] : a[11];
  assign b[12] = k? 'b0 : a[12];
  assign b[13] = k? 'b0 : a[13];
  assign b[14] = k? 'b0 : a[14];
  assign b[15] = k? 'b0 : a[15];
endmodule

module lv4(
  input[15:0] a,
  input k,
  output[15:0] b
);
  assign b[0] = k? a[8] : a[0];
  assign b[1] = k? a[9] : a[1];
  assign b[2] = k? a[10] : a[2];
  assign b[3] = k? a[11] : a[3];
  assign b[4] = k? a[12] : a[4];
  assign b[5] = k? a[13] : a[5];
  assign b[6] = k? a[14] : a[6];
  assign b[7] = k? a[15] : a[7];
  assign b[8] = k? 'b0 : a[8];
  assign b[9] = k? 'b0 : a[9];
  assign b[10] = k? 'b0 : a[10];
  assign b[11] = k? 'b0 : a[11];
  assign b[12] = k? 'b0 : a[12];
  assign b[13] = k? 'b0 : a[13];
  assign b[14] = k? 'b0 : a[14];
  assign b[15] = k? 'b0 : a[15];
endmodule


module funnel_shifter(
	input[15:0] i,
	input[3:0] n,
  output[7:0] b
);
	logic[15:0] k,l,m;
	
	lv1 L0(i,n[0],k);
	lv2 L1(k,n[1],l);
	lv3 L2(l,n[2],m);
	lv4 L3(m,n[3],b);



endmodule


