module compare(
  input[3:0] a,b,
  output[3:0] c,d
);
  always_comb begin
    if(a > b)
    {c,d} = {a,b};
    else
    {c,d} = {b,a};
  end
endmodule

// Implementacja sieci sortującej

module sort(
  input[15:0] i,
  output[15:0] o
);
  logic[3:0] k1,k2,k3,k4,
             g1,g2,g3,g4;
  
  compare C0(i[3:0],i[11:8],k1,k2);
  compare C1(i[7:4],i[15:12],k3,k4);
  compare C2(k1,k3,g1,g2);
  compare C3(k2,k4,g3,g4);
  compare C4(g2,g3,o[11:8],o[7:4]);
  assign o[3:0] = g4;
  assign o[15:12] = g1;
endmodule
