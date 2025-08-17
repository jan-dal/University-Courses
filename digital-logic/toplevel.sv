// Write your modules here!
module circuit(input[0:3] i, output o);
  assign o = (|{i[2:3],i[0]})&
             (|i[1:3])&
             (|i[0:2])&
             (|{i[0:1],i[3]})&
             !(&i);
                
endmodule
