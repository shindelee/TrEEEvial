module EdgeDetection (
  input clk,
  input reset,
  input [7:0] image_in,   // Input pixel data from the camera
  output [7:0] image_out  // Output pixel data after edge detection (sobel filter)
);

// Declare the Sobel filter matrices
parameter [2:0][2:0] GX = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
parameter [2:0][2:0] GY = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

// Input Image Stream
input [7:0] in_data;

// Output Image Stream
output reg [7:0] out_data;

// Variables to store pixel data
// 3x3 pixel buffer
reg [7:0] pixel_buffer[2:0][2:0];

always @(posedge clk) begin
  if (reset) begin
    pixel_out <= 8'b0;  // Clear on reset
  end 

  else begin
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

    // Apply the Sobel operator
    reg [7:0] Gx, Gy, G;
    Gx = pixel_buffer[2][2] + 2*pixel_buffer[2][1] + pixel_buffer[2][0] - pixel_buffer[0][2] - 2*pixel_buffer[0][1] - pixel_buffer[0][0];
    Gy = pixel_buffer[2][2] + 2*pixel_buffer[1][2] + pixel_buffer[0][2] - pixel_buffer[2][0] - 2*pixel_buffer[1][0] - pixel_buffer[0][0];

    // Calculate the magnitude
    int G = sqrt(Gx * Gx + Gy * Gy);

    // Output the result
    out_data = mag;
  end
end

endmodule
