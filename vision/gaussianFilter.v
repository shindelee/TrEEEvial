module gaussian_filter (
    input wire clk,
    input wire [7:0] pixel_in,
    output reg [7:0] pixel_out
);

reg [7:0] buffer [4:0][4:0];  // 5x5 buffer for storing the last 5 rows of pixels
integer row, col;

// The 5x5 Gaussian kernel approximated with integers.
// These values correspond to the binomial coefficients of Pascal's triangle row 5,
// which is a good approximation of the Gaussian distribution.
localparam [7:0] kernel[5][5] = {
    {1,  4,  6,  4,  1},
    {4, 16, 24, 16,  4},
    {6, 24, 36, 24,  6},
    {4, 16, 24, 16,  4},
    {1,  4,  6,  4,  1}
};

always @(posedge clk) begin
    // Shift the pixel values in the buffer to make room for the new pixel.
    for (row=0; row<4; row=row+1) begin
        for (col=0; col<4; col=col+1) begin
            buffer[row][col] <= buffer[row][col+1];
        end
        buffer[row][4] <= buffer[row+1][4];
    end
    for (col=0; col<4; col=col+1) begin
        buffer[4][col] <= buffer[4][col+1];
    end
    buffer[4][4] <= pixel_in;

    // Compute the convolution with the kernel.
    integer sum;
    sum = 0;
    for (row=0; row<5; row=row+1) begin
        for (col=0; col<5; col=col+1) begin
            sum = sum + buffer[row][col] * kernel[row][col];
        end
    end

    // Divide by the sum of the kernel coefficients (in this case, 256) to normalize.
    // This is implemented as a right shift by 8 bits.
    pixel_out <= sum >>> 8;
end

endmodule
