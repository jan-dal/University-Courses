/// Dodawanie 4 bity:
//	analogiczne do dodawania pisemnego
//  + za pomocą sumatora binarnego oblicz pierwszą cyfrę BCD
//  + jeżeli suma > 9 to dodaj 6 (w rezultacie dodawanie mod 10)
//    dodatkowo przekaż resztę do kolejnej cyfry

//  Dodawanie 8 bitów:
//  + 2 cyfrę oblicz analogicznie biorąc pod uwagę resztę z poprzedniej
//  (automatycznie nastąpi dodawanie mod 100)

//  Dodawanie 4*n bitów:
//  + połącz szeregowo jak pełne sumatory przy dodawaniu binarnym

/// Odejmowanie (8 bitów):
//  + oblicz dopełnienie do 9 odjemnika
//  + dodaj go do odjemnej
//  - jeżeli wynik jest ujemny dodaj 1 aby otrzymać odejmowanie mod 100 (dla 8bit)
//  - jeżeli wynik jest dodatni dodaj 1 aby otrzymać resztę mod 100
//  + wystarczy zatem dodać 1 do różnicy przy odejmowaniu



/// multiplexer

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


/// Compliment 9's

module compliment(
  input[3:0] a,
  output[3:0] b
);
  assign b[0] = !a[0];
  assign b[1] = a[1];
  assign b[2] = a[2]^a[1];
  assign b[3] = ~|a[3:1];
  
endmodule


// Half adder
module halfadder(
  input a, 
  input b,
  output o,
  output c
);

  assign o = a ^ b;
  assign c = a & b;

endmodule

// Full adder
module fulladder(
  input a,
  input b,
  input d,
  output o,
  output c
);

  logic t, c1, c2;

  halfadder ha1(a, b, t, c1);
  halfadder ha2(t, d, o, c2);

  assign c = c1 | c2;

endmodule


// 4 bit adder

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
///


/// 4bit BCD adder

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
///

/// 8bit BCD adder

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
///

/// 8bit BCD adder/sub mod 100

module BTCsub(
  input[7:0] a,b,
  input sub,
  output[7:0] o
);
  logic[7:0] cb,mb,sum0;
  logic[3:0] cb0,cb1;
  
  compliment C0(b[3:0],cb0);
  compliment C1(b[7:4],cb1);
  
  assign cb = {cb1,cb0};
  
  multiplexer M0(b,cb,sub,mb);
  
  bit8bcdadder B0(a,mb,'b0,sum0,c0);
 
  
  bit8bcdadder B1('b0,sum0,sub,o,c1);
  
  
endmodule
