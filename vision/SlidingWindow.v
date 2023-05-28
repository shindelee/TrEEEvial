module SlidingWindow (
    input wire clk,
    input wire [7:0] pixel_in,  // Input pixel data
    output wire [7:0] pixel_out // Output pixel data (center pixel of the buffer)
);

    // 3x3 pixel buffer
    reg [7:0] pixel_buffer [0:2][0:2];

    integer i, j;

    always @(posedge clk) begin
        // Shift rows up
        for(i = 0; i < 2; i = i + 1) begin
            for(j = 0; j < 3; j = j + 1) begin
                pixel_buffer[i][j] <= pixel_buffer[i + 1][j];
            end
        end

        // Shift last row left
        for(j = 0; j < 2; j = j + 1) begin
            pixel_buffer[2][j] <= pixel_buffer[2][j + 1];
        end

        // Insert new pixel
        pixel_buffer[2][2] <= pixel_in;

        // Output the center pixel
        pixel_out <= pixel_buffer[1][1];
    end

endmodule
