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
  output carry
);
  
  generator A0(a[0],b[0],c0,g0,p0,s[0]);
  
  assign c1 = p0&&c0|| g0;
  
  generator A1(a[1],b[1],c1,g1,p1,s[1]);
  
  assign c2 = c0&&p0&&p1 || p1&&g0 || g1;
  
  generator A2(a[2],b[2],c2,g2,p2,s[2]);
  
  assign c3 = c0&&p0&&p1&&p2 || g0&&p1&&p2 || g1&&p2 ||g2;
  
  generator A3(a[3],b[3],c3,g3,p3,s[3]);
  
  assign carry = c0&&p0&&p1&&p2&&p3 || g0&&p1&&p2&&p3 ||
    g1&&p2&&p3 || g2&&p3 || g3;
endmodule
