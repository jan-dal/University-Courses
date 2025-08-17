module funkcja(input a,b,c,d,x,y, output o);
  
  assign o = (!x && ((!y && a) || (y && b))) || 
             (x && ((!y && c) || (y && d)));
  
endmodule
