module dane(
  input clk,nrst,start,load,swap,sub,
  input [7:0] ina,inb,
  output ready,lt,equal,
  output logic [7:0] out
);
  const logic RDY = 'b1;
  const logic BSY = 'b0;
  logic [7:0] b;

  always_ff @(posedge clk or negedge nrst)
    if (!nrst)
      ready <= RDY;
    else case(ready)
      RDY : if(start)
	      ready <= BSY;
	  BSY : if(equal)
	      ready <= RDY;
    endcase
  
  always_ff @(posedge clk)
    if (load) begin
      out <= ina;
      b <= inb;
    end else if (swap) begin
      out <= b;
      b <= out;
    end else if (sub)
      out <= out - b;
  
  // Przetwarzanie danych sygnały wysyłane do sterowania
  always_comb begin
    {equal,lt} = 2'b00;
    if (out == b)
      equal = 'b1;
    else if (out < b)
      lt = 'b1;
    end
endmodule

module sterowanie(
  input lt,start,ready,equal,
  output load,sub,swap
);
  const logic RDY = 'b1;
  const logic BSY = 'b0;
  logic lt;
  
  always_comb begin
    {load,swap,sub} = 3'b000;
    case(ready)
      RDY : if(start)
          {load,swap,sub} = 3'b100;
      BSY : if(lt)
          {load,swap,sub} = 3'b010;
      else if (!equal)
          {load,swap,sub} = 3'b001;
    endcase
  end
endmodule

module main(
  input clk,nrst,start,
  input [7:0] ina, inb,
  output ready,
  output [7:0] out
);
  logic rdy,load,swap,sub,equal,lt;
  dane dane(clk,nrst,start,load,swap,sub,ina,inb,ready,lt,equal,out);
  sterowanie sterowanie(lt,start,ready,equal,load,sub,swap); 
endmodule
