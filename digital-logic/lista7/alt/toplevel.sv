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
  assign t = {(down^in[2])&t[2],
              (down^in[1])&t[1],
              step ? en : (down^in[0])&t[0],
              step ? t[3]&(down ? !in[3]&|in[1:2]: in[3]&(~&in[1:2])) : en};

  // Liczenie co dwa jest bardzo podobne do liczenia co jeden, należy zacząć na drugim przerzutniku oraz
  // połączyć 1 z 4 i dodać wyjątek by nie zmieniać stanu 1 przerzutnika gdy mamy 14 lub 15
  // Przy odliczaniu w dół jest odwrotnie
  
  // T0 en&!step | (down ? !in[3]&t[3]&step&!(!in[1]&!in[2]&!in[3]): in[3]&t[3]&step& !(in[1]&in[2]&in[3])) = 
  // = step ? t[3]&(down ? !in[3]&|in[1:2]: in[3]&(~&in[1:2])) : en
  // T1 en&step | down? !step!in[0]&t[0] : !stepin[0]&t[0] = step ? en : (down^in[0])&t[0]
  // T2 down ? !in[1]&t[1] : in[1]&t[1] = (down^in[1])&t[1]
  // T3 down ? !in[2]&t[2] : in[2]&t[2] = (down^in[2])&t[2]
  // 0000 <-> 1110
  // 0001 <-> 1111
  // T0 nie zmieniamy gdy t[1:3] jest wysokie gdy liczymy w górę
  // gdy t[1:3] jest niskie gdy liczymy w dół
  assign out = t; 
endmodule

module main#(parameter N = 4)(
  output[N-1:0] out,
  input down,step,nrst,clk
);
  genvar n;
  logic[N-1:0] k;
  uklad_komb K0(k,out,'b1,down,step);
  for(n=0; n < N; n++)
    tff T(out[n], ,k[n],clk,nrst);
endmodule
