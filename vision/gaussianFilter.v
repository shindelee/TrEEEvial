module gaussianFilter (
  input wire clk,
  input wire [7:0] pixel_in,
  output reg [7:0] pixel_out
);

  // Gaussian Kernel
  parameter [7:0] WEIGHTS [5][5] = { {2, 4, 5, 4, 2},
                                      {4, 9,12, 9, 4},
                                      {5, 12, 15, 12, 5},
                                      {4, 9,12, 9, 4},
                                      {2, 4, 5, 4, 2}};
  reg [7:0] buffer [5][5];
  integer i, j;

  always @(posedge clk) begin
    // Shift the buffer
    for (i=4; i>0; i=i-1)
      for (j=0; j<5; j=j+1)
        buffer[i][j] <= buffer[i-1][j];
    // Insert new pixel
    buffer[0] <= pixel_in;

    // Compute the Gaussian
    pixel_out <= 0;
    for (i=0; i<5; i=i+1)
      for (j=0; j<5; j=j+1)
        pixel_out <= pixel_out + buffer[i][j] * WEIGHTS[i][j];
    pixel_out <= pixel_out / 159;
  end
endmodule
