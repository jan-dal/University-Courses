module circuit(
  input[31:0] i,
  output[31:0] o
);
  logic[31:0] p;
  integer k;
  
  always_comb begin
    {o[31],p[31]} = {2{i[31]}};
    
    for(k=30; k >= 0; k = k-1)
      {o[k],p[k]} = {2{p[k+1]^i[k]}};
  end
endmodule

module main(
  input[31:0] i,
  output[31:0] o
);
  circuit converter(i,o);
endmodule
