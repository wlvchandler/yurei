module Memory (
    input clk,
    input write_enable,
    input [15:0] address,
    input [7:0] data_in,
    output reg [7:0] data_out
);

reg [7:0] ram [0:65535];

always @(posedge clk) begin
    if (write_enable) begin
        ram[address] <= data_in;
    end
    data_out <= ram[address];
end

endmodule
