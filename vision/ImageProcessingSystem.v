module ImageProcessingSystem (
    input wire clk,
    input wire reset,
    input wire [7:0] pixel_data_in,  // Input pixel data from the camera
    output wire [7:0] pixel_data_out  // Output pixel data after processing
);

    wire [7:0] edge_pixel_data;
    
    EdgeDetection sobelfilter (
        .clk(clk),
        .reset(reset),
        .pixel_data_in(pixel_data_in),
        .pixel_data_out(edge_pixel_data)
    );
    
    Binarization binarization (
        .clk(clk),
        .reset(reset),
        .pixel_data_in(edge_pixel_data),
        .pixel_data_out(pixel_data_out)
    );

endmodule
