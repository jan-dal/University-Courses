module RAM (
  input wr, clk,
  input[7:0] wraddr, rdaddr,
  input[7:0] datain,
  output[7:0] dataout
);
  logic[7:0] mem [255:0];
  
  always_ff @(posedge clk)
    if(wr)
      mem[wraddr] <= datain;
  
  assign dataout = mem[rdaddr];
endmodule

module Counter (
  input up,dn,zero,nrst,clk,
  output logic[7:0] q
);
  always_ff @(posedge clk or negedge nrst)
    if(!nrst)
      q <= 8'b0;
    else if(zero)
      q <= 8'b0;
    else if(up)
      q <= q + 1;
    else if(dn)
      q <= q - 1;        
endmodule

module Memarith(
  input addmem, submem, wrzero,
  input[7:0] datain, in_data,
  output[7:0] dataout
);
  always_comb begin
    dataout = in_data;
    if(addmem)
      dataout = datain + 1;
    else if(submem)
      dataout = datain - 1;
    else if(wrzero)
      dataout = 8'b0;
   end   
endmodule


module Dane (
  input clk, nrst, addmem, submem, uhd, dhd, 
         memwr, wrzero, txtwr, upc,
         dpc, uc, dc, crst, pcrst, hdrst,
  input[7:0] in_data,
  output hdzero, memzero, islcl, isrcl, cgt,
  output[7:0] out_data, instr
);
  const logic[7:0]
    RCL   = 8'b1011101,
    LCL   = 8'b1011011;
  logic[7:0] c, pc, hd,hd1, datain;
  logic[7:0] hdp;
  Memarith AR(addmem, submem, wrzero, out_data, in_data, datain);
  Counter C(uc, dc, crst, nrst, clk, c);
  Counter PC(upc, dpc, pcrst, nrst, clk, pc);
  Counter HD(uhd, dhd, hdrst, nrst, clk, hd);
  RAM HDMEM(memwr, clk, hd, hd, datain , out_data);
  RAM PCMEM(txtwr, clk, pc, pc, in_data, instr);
  assign hdp = hd + 1;
  always_comb begin
    hdzero  = 'b0;  // hd == 0
    memzero = 'b0;  // mem[hd] == 0
    islcl   = 'b0;  // mempc[pc] == [
    isrcl   = 'b0;  // mempc[pc] == ]
    cgt     = 'b0;  // c > 0 
    if (hdp == 0)
      hdzero = 'b1;
    if (out_data == 0)
      memzero = 'b1;
    if (instr == RCL)
      isrcl = 'b1;
    if (instr == LCL)
      islcl = 'b1;
    if (c > 0)
      cgt = 'b1;
  end
endmodule

module Sterowanie(
  input clk, nrst, start, out_ack, in_valid,
        hdzero, memzero, islcl, isrcl, cgt, 
  input[7:0] instr,
  output ready, addmem, submem, uhd, dhd, 
         outval, inack, memwr, wrzero, txtwr,
         upc, dpc, uc, dc, crst, pcrst, hdrst
);
  logic[2:0] stan;
  const logic[2:0] 
    RDY  = 3'b000,
    ZERO = 3'b001,
    WRK  = 3'b010,
    WRT  = 3'b011,
    RD   = 3'b100,
    RGT  = 3'b101,
    LFT  = 3'b111;
  const logic[7:0]
    PLUS  = 8'b101011,
    MINUS = 8'b101101,
    LT    = 8'b111100,
    GT    = 8'b111110,
    DOT   = 8'b101110,
    COM   = 8'b101100,
    RCL   = 8'b1011101,
    LCL   = 8'b1011011,
    END   = 8'b0000000;
   
  always_ff @(posedge clk or negedge nrst)
    if(!nrst)
      stan <= RDY;
    else unique case(stan)
      RDY  : if(start)
               stan <= ZERO;
      ZERO : if(hdzero)
               stan <= WRK;
      WRK  : unique case(instr)
               DOT : stan <= WRT;
               COM : stan <= RD;
               RCL : if (!memzero)
                       stan <= RGT;
               LCL : if (memzero)
                       stan <= LFT;
               END : stan <= RDY;
             endcase
      WRT  : if(out_ack)
               stan <= WRK;
      RD   : if(in_valid)
               stan <= WRK;
      RGT  : if(islcl&!cgt)
               stan <= WRK;
      LFT  : if(isrcl&!cgt)
               stan <= WRK;
    endcase

  always_comb begin
    ready  = 'b0;    // Sygnał ready
    addmem = 'b0;    // Dodaj 1 do pamięci
    submem = 'b0;    // Odejmij 1 od pamięci
    uhd    = 'b0;    // Zwiększ hd
    dhd    = 'b0;    // Zmniejsz hd
    outval = 'b0;    // Out_valid
    inack  = 'b0;    // in_ack
    memwr  = 'b0;    // Zapisz w pamięci dane
    wrzero = 'b0;    // Zapisz 0 w pamięci danych
    txtwr  = 'b0;    // Zapisz w pamięci instr.
    upc    = 'b1;    // Zwiększ pc
    dpc    = 'b0;    // Zmniejsz pc
    uc     = 'b0;    // Zwiększ c
    dc     = 'b0;    // Zmniejsz c
    crst   = 'b0;    // Resetuj c
    pcrst  = 'b0;    // Resetuj pc
    hdrst  = 'b0;    // Resetuj hd
    if(nrst) 
      unique case(stan)
      RDY  : begin
        ready = 'b1;
        upc   = 'b0;
        if(!start&in_valid) begin
          inack  = 'b1;
          txtwr  = 'b1;
          upc    = 'b1;
          end
      end
      ZERO : begin
        pcrst  = 'b1;
        wrzero = 'b1;
        memwr  = 'b1;
        uhd    = 'b1;
        end
      WRK  : unique case(instr)
        PLUS  : begin 
          addmem = 'b1;
          memwr  = 'b1;
        end
        MINUS : begin
          submem = 'b1;
          memwr  = 'b1;
          end
        GT    : uhd    = 'b1;
        LT    : dhd    = 'b1;
        DOT   : upc     = 'b0;
        COM   : upc     = 'b0;
        LCL   : if(memzero)
          crst = 'b1;
        RCL   : if(!memzero) begin
          crst = 'b1;
          dpc  = 'b1;
          upc  = 'b0; 
          end 
        END   : begin
          pcrst  = 'b1;
          hdrst  = 'b1;
          end
      endcase
      LFT : if(islcl) 
        uc = 'b1;
        else if(isrcl&cgt)
          dc = 'b1;
      RGT : begin
        dpc = 'b1;
        upc = 'b0;
        if(isrcl) 
          uc = 'b1;
        else if(islcl&cgt)
          dc = 'b1;
        else if(islcl&!cgt) begin
          dpc = 'b0;
          upc = 'b1;
          end
        end
      WRT : begin
        outval = 'b1;
        if (!out_ack)
          upc = 'b0;
      end
      RD  : begin
        inack = 'b1;
        if (!in_valid)
          upc    = 'b0;
        else
          memwr = 'b1;
        end
    endcase
  end
endmodule

module main(
  input clk, nrst, in_valid, out_ack, start,
  input[7:0] in_data,
  output in_ack, out_valid, ready,
  output[7:0] out_data
);
  logic addmem, submem, uhd, dhd, memwr, wrzero, txtwr,
        upc, dpc, uc, dc, crst, pcrst, hdrst, hdzero, memzero,
        islcl, isrcl, cgt;
  logic[7:0] instr;
  
  Dane D0(clk, nrst, addmem, submem, uhd, dhd, memwr, wrzero,
          txtwr, upc, dpc, uc, dc, crst, pcrst, hdrst, in_data,
          hdzero, memzero, islcl, isrcl, cgt, out_data, instr);
  Sterowanie S0(clk, nrst, start, out_ack, in_valid, hdzero, memzero,
                islcl, isrcl, cgt, instr, ready, addmem, submem,
                uhd, dhd, out_valid, in_ack, memwr, wrzero, txtwr,
                upc, dpc, uc, dc, crst, pcrst, hdrst);
endmodule
