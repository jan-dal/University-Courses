module sort(
  input[15:0] i,
  output[15:0] o
);
  logic[3:0] k1,k2,k3,k4,
             g1,g2,g3,g4;
  
  always_comb begin
    if (i[3:0] > i[11:8])
    {k1,k2} = {i[3:0],i[11:8]};
    else
    {k1,k2} = {i[11:8],i[3:0]};

    if (i[7:4] > i[15:12])
    {k3,k4} = {i[7:4],i[15:12]};
    else
    {k3,k4} = {i[15:12],i[7:4]};
  end
  
  always_comb begin
    if (k1 > k3)
    {g1,g2} = {k1,k3};
    else
    {g1,g2} = {k3,k1};
   	
    if (k2 > k4)
    {g3,g4} = {k2,k4};
    else
    {g3,g4} = {k4,k2};
  end
  
  always_comb begin
    if (g3 > g2)
    {o[7:4],o[11:8]} = {g3,g2};
    else
    {o[7:4],o[11:8]} = {g2,g3};
    
    o[3:0] = g4;
    o[15:12] = g1;
  end
endmodule
