module counter #(
    parameter WIDTH = 8
)(
    // interface signals
    input logic clk, // clock
    input logic rst, // reset
    input logic en, // counter enable
    output logic [WIDTH-1:0] count
);

always_ff @ (posedge clk, posedge rst) 
    if (rst) count <= {WIDTH{1'b0}}; // reset with 8 zero's
    else count <= count + (en ? {{WIDTH{1'b0}}, en} : {WIDTH{1'b1}} ); // en=1: add one, en=0: minus one

endmodule
