module medianFilter (
  input wire clk,
  input wire [7:0] pixel_in,
  output reg [7:0] pixel_out
);
  reg [7:0] buffer [2:0][2:0];
  reg [7:0] sorted [8:0];
  integer i, j, k;

  always @(posedge clk) begin
    // Shift the buffer
    for (i=2; i>0; i=i-1)
      for (j=0; j<3; j=j+1)
        buffer[i][j] <= buffer[i-1][j];
    // Insert new pixel
    buffer[0][0] <= pixel_in;

    // Flatten the buffer into the sorted array
    k = 0;
    for (i=0; i<3; i=i+1)
      for (j=0; j<3; j=j+1)
        sorted[k] <= buffer[i][j];
        k = k + 1;

    // Sort the array (Bubble Sort)
    for (i=0; i<8; i=i+1)
      for (j=0; j<8-i; j=j+1)
        if (sorted[j] > sorted[j+

