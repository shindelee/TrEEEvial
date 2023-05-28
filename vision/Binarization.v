module Binarization (
    input wire clk,
    input wire reset,
    input wire [7:0] pixel_data_in,  // Input pixel data
    output reg [7:0] pixel_data_out  // Output pixel data after binarization
);
    // You can adjust this threshold according to your requirements
    localparam THRESHOLD = 8'h80; // 128 in decimal

    // can be in terms of 0s and 1s

    always @(posedge clk or posedge reset) begin
        if (reset) begin
            pixel_data_out <= 8'b0; // Clear on reset
        end else begin
            // Perform binarization
            if (pixel_data_in > THRESHOLD) begin
                pixel_data_out <= 8'hFF; // White OR pixel_data_out = 1
            end else begin
                pixel_data_out <= 8'h00; // Black OR pixel_data_out = 0
            end
        end
    end

endmodule
