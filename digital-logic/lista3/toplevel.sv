module generator(
  input a,b,c,
  output g,p,s
);
  assign g = a&&b;
  assign p = a||b;
  assign s = a^b^c;
endmodule

  

module bit4adder(
  input[3:0] a,b,
  input c0,
  output[3:0] s,
  output G,P
);
  
  generator A0(a[0],b[0],c0,g0,p0,s[0]);
  
  assign c1 = p0&&c0|| g0;
  
  generator A1(a[1],b[1],c1,g1,p1,s[1]);
  
  assign c2 = c0&&p0&&p1 || p1&&g0 || g1;
  
  generator A2(a[2],b[2],c2,g2,p2,s[2]);
  
  assign c3 = c0&&p0&&p1&&p2 || g0&&p1&&p2 || g1&&p2 ||g2;
  
  generator A3(a[3],b[3],c3,g3,p3,s[3]);
  
  assign G = g0&&p1&&p2&&p3 || g1&&p2&&p3 || g2&&p3 ||g3;
  assign P = p0&&p1&&p2&&p3;

endmodule

module bit16adder(
  input[15:0] a,b,
  output[15:0] o,
);
  assign c0 = 'b0;
  bit4adder B0(a[3:0],b[3:0],c0,o[3:0],G0,P0);
  
  assign c4 = P0&&c0 || G0;
  bit4adder B1(a[7:4],b[7:4],c4,o[7:4],G1,P1);
  
  assign c8 = c0&&P0&&P1 || G0&&P1 || G1;
  bit4adder B2(a[11:8],b[11:8],c8,o[11:8],G2,P2);
  
  assign c12 = c0&&P0&&P1&&P2 || G0&&P1&&P2 || G1&&P2 ||G2;
  bit4adder B3(a[15:12],b[15:12],c12,o[15:12],G3,P3);

  assign c15 = c0&&P0&&P1&&P2&&P3 || G0&&P1&&P2&&P3 
    || G1&&P2&&P3 ||G2&&P3 || G3;
endmodule


  
  
  

