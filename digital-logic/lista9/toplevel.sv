module Main(
  input clk,nrst,door,start,finish,
  output heat,light,bell
);
  logic[2:0] k,in;
  assign in = {start,finish,door};
  const logic[3:0] CL = 3'b000, CK = 3'b011, P = 3'b010,
                    B = 3'b100, OP = 3'b001;
  
  always_ff @(posedge clk or negedge nrst)
    if(!nrst)
      k <= 3'b000;
    else unique casez(k)
      CL : casez(in) 3'b??1 : k <= OP; // door opened
                     3'b1?0 : k <= CK; // started
                     endcase
      CK : casez(in) 3'b?10 : k <= B;  // finish
                     3'b??1 : k <= P;  // paused
                     endcase
      P  : casez(in) 3'b??0 : k <= CK; // cooking
                     endcase
      B  : casez(in) 3'b??1 : k <= OP; // door opened
                     endcase
      OP : casez(in) 3'b??0 : k <= CL; // door closed
                     endcase
    endcase
   
  assign {bell,light,heat} = {k[2],k[0]|k[1],k[1]&k[0]};

endmodule
