module BCDadder(
	input[3:0] a,b,
	input c0,
	output[3:0] o,
	output c4
	
);
  logic[3:0] l,six;
  
  // if l > 9 add 6
 
  bit4adder B0(a,b,c0,l,c);
  
  assign six[1] = l[3]&&l[2] || l[3]&&l[1] || c;
  assign six[2] = l[3]&&l[2] || l[3]&&l[1] || c;
  assign six[0] = 'b0;
  assign six[3] = 'b0;
  assign c4 = six[1] || six[2] || c;
  bit4adder B1(six,l,'b0,o,cout);
endmodule
