module main(
  input clk,nrst,wr,start,
  input [9:0] addr,
  input [15:0] datain,
  output logic ready,
  output [15:0] out
);
  logic push,en;
  logic [2:0] op;
  logic [9:0] ip,cnt,addrcntr;
  logic [15:0] dataout,din;
  
  always_ff @(posedge clk or negedge nrst)
    if(!nrst)
      ready <= 'b1;
    else case(ready)
      'b1 : if(start) ready <= 'b0;
      'b0 : if(dataout[15]&dataout[14]) ready <= 'b1;
    endcase
  
  always_ff @(posedge clk or negedge nrst)
    if(!nrst)
      ip <= 10'b0;
    else if(!ready)
      if (&{dataout[15],!dataout[14],dataout[2:0]})
        ip <= out;
      else
        ip <= ip + 1;
  
  assign addrcntr = ready ? addr : ip;
  assign en = &{!ready,!&dataout[15:14]};
  memory codemem(clk,wr,addrcntr,addrcntr,datain,dataout);
  assign push = !dataout[15] ? 'b1 :'b0;
  assign din = !dataout[15] ? dataout[14:0] : 16'bx;
  assign op = dataout[15]&!dataout[14] ? dataout[2:0] : 3'b0;
  
  kalkulator calc(nrst,clk,push,en,op,din,cnt,out);
endmodule
