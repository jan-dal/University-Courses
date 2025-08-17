// Do zapisywania stanu układu używam przerzutnika typu D wyzwalanego zboczem
module dff(
  input en,d,
  output q
);
  logic r,s,nr,ns;
  nand gq(q,nr,nq),gnq(nq,ns,q),
  gr(nr,en,r),gs(ns,nr,en,s),gr1(r,nr,s),
  gs1(s,ns,d);
endmodule

// Logika kontrolująca odpowiednie przerzutniki

module MSBlogic(
  input en, q, d, l, r, i,
  output data, c
);
  logic k,n,m;
  assign k = d&&r&&l;
  assign n = l&&!r&&i;
  assign m = !l&&r&&q;
  assign c = !en&&(r||l) ? en : 'b0;
  assign data = m||n||k;
endmodule

module Midlogic(
  input en, ql, qr, d, l, r,
  output data, c
);
  logic k,m,n;
  assign k = d&&l&&r;
  assign m = qr&&r&&!l;
  assign n = ql&&!r&&l;
  assign c = !en&&(r||l) ? en : 'b0;
  assign data = m||n||k;
endmodule

module LSBlogic(
  input en, q, d, l, r, i,
  output data, c
);
  logic k,n,m;
  assign k = d&&r&&l;
  assign n = !l&&r&&i;
  assign m = l&&!r&&q;
  assign c = !en&&(r||l) ? en : 'b0;
  assign data = m||n||k;
endmodule

// Ogólny moduł 8 bitowy kontrolujący przerzutniki

module Logic(
  input[7:0] d,q,
  input i,l,r,en,
  output[7:0] data,c
);
  MSBlogic M7(en,q[6],d[7],l,r,i,data[7],c[7]);
  Midlogic M6(en,q[7],q[5],d[6],l,r,data[6],c[6]),
  M5(en,q[6],q[4],d[5],l,r,data[5],c[5]),
  M4(en,q[5],q[3],d[4],l,r,data[4],c[4]),
  M3(en,q[4],q[2],d[3],l,r,data[3],c[3]),
  M2(en,q[3],q[1],d[2],l,r,data[2],c[2]),
  M1(en,q[2],q[0],d[1],l,r,data[1],c[1]);
  LSBlogic M0(en,q[1],d[0],l,r,i,data[0],c[0]);
endmodule

// Ogólny moduł pamięci 8 bitowej

module Pamiec(
  input[7:0] in,en,
  output[7:0] out
);
  dff d7(en[0],in[7],out[7]),
  d6(en[1],in[6],out[6]),
  d5(en[2],in[5],out[5]),
  d4(en[3],in[4],out[4]),
  d3(en[4],in[3],out[3]),
  d2(en[5],in[2],out[2]),
  d1(en[6],in[1],out[1]),
  d0(en[7],in[0],out[0]);
endmodule

// Moduł główny

module Rejestr(
  input[7:0] d,
  input i,c,r,l,
  output[7:0] q  
);
  logic[7:0] cl,k;
  Pamiec P0(k,cl,q);
  Logic L0(d,q,i,l,r,c,k,cl);
endmodule
