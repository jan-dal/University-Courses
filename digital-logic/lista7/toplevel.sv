module tff(
  output q, nq,
  input t, clk, nrst
);
  logic ns, nr, ns1, nr1, j, k;
  nand n1(ns, clk, j), n2(nr, clk, k), n3(q, ns, nq),
       n4(nq, nr, q, nrst), n5(ns1, !clk, t, nq),
       n6(nr1, !clk, t, q), n7(j, ns1, k), n8(k, nr1, j, nrst);
endmodule

module uklad_komb(
  output[3:0] out,
  input[3:0] in,
  input en,down,step
);
  logic[3:0] t;
  // Liczenie co dwa jest bardzo podobne do liczenia co jeden, należy zacząć na drugim przerzutniku oraz
  // połączyć 1 z 4 i dodać wyjątek by nie zmieniać stanu 1 przerzutnika gdy mamy 14 lub 15
  // Przy odliczaniu w dół jest odwrotnie
  
  // T0 (en&&!step) || (down ? !in[3]&&t[3]&&step&&(in[1]&in[2]&in[3]): in[3]&&t[3]&&step&& !(in[1]&in[2]&in[3])) = 
  // = en&&!step || step&&t[3]&&(down ? in[1]&&in[2] : in[1]&&!(in[2]&&in[3]))
  // T1 en&&step||down? !in[0]&&t[0] : in[0]&&t[0] = en&&step || (down^in[0])&&t[0]
  // T2 down ? !in[1]&&t[1] : in[1]&&t[1] = (down^in[1])&&t[1]
  // T3 down ? !in[2]&&t[2] : in[2]&&t[2] = (down^in[2])&&t[2]
  assign t = {(down^in[2])&t[2],
              (down^in[1])&t[1],
              en&step || (down^in[0])&t[0],
              (en&!step) || t[3]&step&(down ? in[1]&in[2]: in[3]&!(in[2]&in[1]))};
  assign out = t;
endmodule

module main(
  output[3:0] out,
  input down,step,nrst,clk
);
  logic[3:0] k;
  uklad_komb K0(k,out,'b1,down,step);
  tff 
  T0(out[0], ,k[0],clk,nrst),
  T1(out[1], ,k[1],clk,nrst),
  T2(out[2], ,k[2],clk,nrst),
  T3(out[3], ,k[3],clk,nrst);
endmodule
