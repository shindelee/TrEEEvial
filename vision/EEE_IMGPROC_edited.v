module EEE_IMGPROC(

	// global clock & reset
	clk,
	reset_n,
	
	// mm slave
	s_chipselect,
	s_read,
	s_write,
	s_readdata,
	s_writedata,
	s_address,

	// stream sink
	sink_data,
	sink_valid,
	sink_ready,
	sink_sop,
	sink_eop,
	
	// streaming source
	source_data,
	source_valid,
	source_ready,
	source_sop,
	source_eop,
	
	// conduit
	mode
	
);


// global clock & reset
input	clk;
input	reset_n;

// mm slave
input							s_chipselect;
input							s_read;
input							s_write;
output	reg	[31:0]	            s_readdata;
input	[31:0]				    s_writedata;
input	[2:0]					s_address;


// streaming sink
input	[23:0]            	    sink_data;
input							sink_valid;
output							sink_ready;
input							sink_sop;
input							sink_eop;

// streaming source
output	[23:0]			  	    source_data;
output							source_valid;
input							source_ready;
output							source_sop;
output							source_eop;

// conduit export
input                           mode;

////////////////////////////////////////////////////////////////////////
//
parameter IMAGE_W = 11'd640;		// image width
parameter IMAGE_H = 11'd480;		// image height
parameter MESSAGE_BUF_MAX = 256;
parameter MSG_INTERVAL = 6;
parameter BB_COL_DEFAULT = 24'h00ff00;  // bounding box default color: green


wire [7:0]   red, green, blue, grey;
wire [7:0]   red_out, green_out, blue_out;
wire         sop, eop, in_valid, out_ready;

////////////////////////////////////////////////////////////////////////
/*
reg[7:0] red_gauss_stage_1, red_gauss_stage_2, red_gauss_stage_3, red_gauss_stage_4, red_gauss_stage_5;
reg[7:0] green_gauss_stage_1, green_gauss_stage_2, green_gauss_stage_3, green_gauss_stage_4, green_gauss_stage_5;
reg[7:0] blue_gauss_stage_1, blue_gauss_stage_2, blue_gauss_stage_3, blue_gauss_stage_4, blue_gauss_stage_5;

always @(posedge clk) begin
	red_gauss_stage_1 <= red;
	red_gauss_stage_2 <= red_gauss_stage_1;
	red_gauss_stage_3 <= red_gauss_stage_2;
	red_gauss_stage_4 <= red_gauss_stage_3;
	red_gauss_stage_5 <= red_gauss_stage_4;

	green_gauss_stage_1 <= green;
	green_gauss_stage_2 <= green_gauss_stage_1;
	green_gauss_stage_3 <= green_gauss_stage_2;
	green_gauss_stage_4 <= green_gauss_stage_3;
	green_gauss_stage_5 <= green_gauss_stage_4;

	blue_gauss_stage_1 <= blue;
	blue_gauss_stage_2 <= blue_gauss_stage_1;
	blue_gauss_stage_3 <= blue_gauss_stage_2;
	blue_gauss_stage_4 <= blue_gauss_stage_3;
	blue_gauss_stage_5 <= blue_gauss_stage_4;
	
end

// Gaussian Filter
reg [7:0] gauss_red, gauss_green, gauss_blue;

reg [14:0] temp_b1, temp_b2, temp_b3, temp_b4, temp_b5;
reg [14:0] temp_r1, temp_r2, temp_r3, temp_r4, temp_r5;
reg [14:0] temp_g1, temp_g2, temp_g3, temp_g4, temp_g5;

reg [14:0] temp_sum_r, temp_sum_g, temp_sum_b;


always @(*) begin

	if (x < 2) begin
		gauss_red = red;
		gauss_green = green;
		gauss_blue = blue;
	end 

	// 11'h2
	if (x % IMAGE_W > IMAGE_W - 2 ) begin
		gauss_red = Red_stage_5;
		gauss_green = Green_stage_5;
		gauss_blue = Blue_stage_5;
	end 

	else begin

		temp_r1 = 8 * red_gauss_stage_1; 
		temp_r2 = 31 * red_stage_2;
		temp_r3 = 49 * red_stage_3;
		temp_r4 = 31 * red_stage_4;
		temp_r5 = 8 * red_stage_5;
		temp_sum_r = temp_r1 + temp_r2 + temp_r3 + temp_r4 + temp_r5;
		gauss_red = temp_sum_r [14:7];

		temp_g1 = 8 * Green_stage_1; 
		temp_g2 = 31 * Green_stage_2;
		temp_g3 = 49 * Green_stage_3;
		temp_g4 = 31 * Green_stage_4;
		temp_g5 = 8 * Green_stage_5;
		temp_sum_g = temp_g1 + temp_g2 + temp_g3 + temp_g4 + temp_g5;
		gauss_green = temp_sum_g [14:7];

		temp_b1 = 8 * Blue_stage_1; 
		temp_b2 = 31 * Blue_stage_2;
		temp_b3 = 49 * Blue_stage_3;
		temp_b4 = 31 * Blue_stage_4;
		temp_b5 = 8 * Blue_stage_5;
		tmp_sum_b = tmp_b_1 + tmp_b_2 + tmp_b_3 + tmp_b_4 + tmp_b_5;
		gauss_blue = temp_sum_b [14:7];

	end 
end



// Median Filter

wire [7:0] median_red, median_green, median_blue;
reg [7:0] blue_median_stage_1, blue_median_stage_2, blue_median_stage_3, blue_median_stage_4, blue_median_stage_5;
reg [7:0] red_median_stage_1, red_median_stage_2, red_median_stage_3, red_median_stage_4, red_median_stage_5;
reg [7:0] green_median_stage_1, green_median_stage_2, green_median_stage_3, green_median_stage_4, red_median_stage_5;

always @(posedge clk) begin
	red_median_stage_1 <= gauss_red;
	red_median_stage_2 <= red_median_stage_1;
	red_median_stage_3 <= red_median_stage_2;
	red_median_stage_4 <= red_median_stage_3;
	red_median_stage_5 <= red_median_stage_4;

	green_median_stage_1 <= gauss_green;
	green_median_stage_2 <= green_median_stage_1;
	green_median_stage_3 <= green_median_stage_2;
	green_median_stage_4 <= green_median_stage_3;
	green_median_stage_5 <= green_median_stage_4;

	blue_median_stage_1 <= gauss_blue;
	blue_median_stage_2 <= blue_median_stage_1;
	blue_median_stage_3 <= blue_median_stage_2;
	blue_median_stage_4 <= blue_median_stage_3;
	blue_median_stage_5 <= blue_median_stage_4;
end

Median_Filter medRed(
	.x(x),
	.gaussian(gauss_red),
	.reg_stage5(red_median_stage_5),
	.one(red_median_stage_1),
	.two(red_median_stage_2),
	.three(red_median_stage_3),
	.four(red_median_stage_4),
	.five(red_median_stage_5),
	.result(median_red)
);

Median_Filter medGreen(
	.x(x),
	.gaussian(gauss_green),
	.reg_stage5(green_median_stage_5),
	.one(green_median_stage_1),
	.two(green_median_stage_2),
	.three(green_median_stage_3),
	.four(green_median_stage_4),
	.five(green_median_stage_5),
	.result(median_green)
);

Median_Filter medBlue(
	.x(x),
	.gaussian(gauss_blue),
	.reg_stage5(blue_median_stage_5),
	.one(blue_median_stage_1),
	.two(blue_median_stage_2),
	.three(blue_median_stage_3),
	.four(blue_median_stage_4),
	.five(blue_median_stage_5),
	.result(median_blue)
);
*/

// 5 consecutive pixels 
reg red_detect_1, red_detect_2, red_detect_3 , red_detect_4, red_detect_5;
reg blue_detect_1, blue_detect_2, blue_detect_3 , blue_detect_4, blue_detect_5;
reg yellow_detect_1, yellow_detect_2, yellow_detect_3, yellow_detect_4, yellow_detect_5;

initial begin
	red_detect_1 = 0;
	red_detect_2 = 0;
	red_detect_3 = 0;
	red_detect_4 = 0;
	red_detect_5 = 0;

	blue_detect_1 = 0;
	blue_detect_2 = 0;
	blue_detect_3 = 0;
	blue_detect_4 = 0;
	blue_detect_5 = 0;

	yellow_detect_1 = 0;
	yellow_detect_2 = 0;
	yellow_detect_3 = 0;
	yellow_detect_4 = 0;
	yellow_detect_5 = 0;

end

always @(posedge clk)begin
	red_detect_1 <= red_detect;
	red_detect_2 <= red_detect_1;
	red_detect_3 <= red_detect_2;
	red_detect_4 <= red_detect_3;
	red_detect_5 <= red_detect_4;

	blue_detect_1 <= blue_detect;
	blue_detect_2 <= blue_detect_1;
	blue_detect_3 <= blue_detect_2;
	blue_detect_4 <= blue_detect_3;
	blue_detect_5 <= blue_detect_4;


	yellow_detect_1 <= yellow_detect;
	yellow_detect_2 <= yellow_detect_1;
	yellow_detect_3 <= yellow_detect_2;
	yellow_detect_4 <= yellow_detect_3;
	yellow_detect_5 <= yellow_detect_4;
end

/*
// R, G, B: 0 - 360
wire [7:0] cmax, cmin, delta, sat, val;
wire [8:0] hue_temp, hue;

// division by 256
assign r = red >> 8; 
assign b = blue >> 8;
assign g = green >> 8;

assign cmax = ((r >= g) & (r >= b)) ? r : ((g >= b) & (g >= r)) ? g : b; 
assign cmin = ((r <= g) & (r <= b)) ? r : ((g <= b) & (g <= r)) ? g : b;
assign delta = cmax - cmin;

assign hue_temp = (delta == 0) ? 0 : (cmax == r) ? (60 * (g - b) / delta)
                                   : (cmax == g) ? (120 + 60 * (b - r) / delta)
                                   : (240 + 60 * (r - g) / delta);

assign hue = (hue_temp < 0) ? hue_temp + 360 : hue_temp;
assign sat = (cmax == 0) ? 0 : delta / cmax;
assign val = cmax; 

wire red_detect, blue_detect, yellow_detect;
assign red_detect = red[7] & ~green[7] & ~blue[7];
assign blue_detect = (hue > 175) & (hue < 260) & sat > 4/10 & val > 8/10;
assign yellow_detect = (hue > 35) & (hue < 63) & sat > 4/10 & val > 8/10; 

*/
// RGB -> HSV conversion:
// H: 0 - 360, S: 0 - 1, V: 0 - 1
wire [7:0] r, g, b;
wire [7:0] cmax, cmin, delta, sat, val;
wire [8:0] hue_temp, hue;

assign r = red / 255; 
assign b = blue / 255;
assign g = green / 255;

assign cmax = ((r >= g) & (r >= b)) ? r : ((g >= b) & (g >= r)) ? g : b;
assign cmin = ((r <= g) & (r <= b)) ? r : ((g <= b) & (g <= r)) ? g : b;
assign delta = cmax - cmin;

assign hue_temp = (delta == 0) ? 0 : (cmax == r) ? (60 * (g - b) / delta)
                                   : (cmax == g) ? (120 + 60 * (b - r) / delta)
                                   : (240 + 60 * (r - g) / delta);

assign hue = (hue_temp < 0) ? hue_temp + 360 : hue_temp;
assign sat = (cmax == 0) ? 0 : delta / cmax;
assign val = cmax; 

// HSV
wire red_detect, blue_detect, yellow_detect;
assign red_detect = red[7] & ~green[7] & ~blue[7];
assign blue_detect = (hue > 175) & (hue < 260) & sat > 4/10 & val > 8/10;
assign yellow_detect = (hue > 35) & (hue < 63) & sat > 4/10 & val > 8/10; 

// Find boundary of cursor box

// Highlight detected areas
// wire [23:0] red_high, blue_high, yellow_high;


wire red_high, blue_high, yellow_high;

assign red_high = red_detect_1 && red_detect_2 && red_detect_3 && red_detect_4 && red_detect_5;
assign blue_high = blue_detect_1 && blue_detect_2 && blue_detect_3 && blue_detect_4 && blue_detect_5;
assign yellow_high = yellow_detect_1 && yellow_detect_2 && yellow_detect_3 && yellow_detect_4 && yellow_detect_5;

// Greyscale conversion
assign grey = green[7:1] + red[7:2] + blue[7:2]; 
// Grey = green/2 + red/4 + blue/4

// Highlighting detected areas for new image
wire [23:0] detectedAreaRGB;
assign detectedAreaRGB  = red_high ? {8'hff, 8'h0, 8'h0} : 
                          blue_high ? {8'h0, 8'h0, 8'hff} :
						  yellow_high ? {8'hff, 8'hff, 8'h0} :
                          {grey, grey, grey};

/*
// Highlighting detected areas for new image
wire [23:0] detectedAreaRGB;
assign detectedAreaRGB  = red_detect ? {8'hff, 8'h0, 8'h0} : 
                          blue_detect ? {8'h0, 8'h0, 8'hff} :
						  yellow_detect ? {8'hff, 8'hff, 8'h0} :
                          {grey, grey, grey};
*/

// Show bounding box without line intersections
wire [23:0] new_image;
wire bb_active_red, bb_active_blue, bb_active_yellow;
assign bb_active_red = ((x == left_red | x == right_red) & (y <= bottom_red) & (y >= top_red)) | ((y == top_red | y == bottom_red) & (x <= right_red) & (x >= left_red));
assign bb_active_blue = ((x == left_blue | x == right_blue) & (y <= bottom_blue) & (y >= top_blue)) | ((y == top_blue | y == bottom_blue) & (x <= right_blue) & (x >= left_blue));
assign bb_active_yellow = ((x == left_yellow | x == right_yellow) & (y <= bottom_yellow) & (y >= top_yellow)) | ((y == top_yellow | y == bottom_yellow) & (x <= right_yellow) & (x >= left_yellow));

// Show bounding box with lines intersections 
// assign bb_active_red = (x == left_red) | (x == right_red) | (y == top_red) | (y == bottom_red);
// assign bb_active_blue = (x == left_blue) | (x == right_blue) | (y == top_blue) | (y == bottom_blue);
// assign bb_active_yellow = (x == left_yellow) | (x == right_yellow) | (y == top_yellow) | (y == bottom_yellow);

assign new_image = bb_active_red ? {8'hff, 8'h0, 8'h0} 
				 : bb_active_blue ? {8'h0, 8'h0, 8'hff} 
				 : bb_active_yellow ? {8'hff, 8'hff, 8'h0} 
				 : detectedAreaRGB;

// Switch output pixels depending on mode switch
// Don't modify the start-of-packet word - it's a packet discriptor
// Don't modify data in non-video packets
assign {red_out, green_out, blue_out} = (mode & ~sop & packet_video) ? new_image : {red, green, blue};

// Count valid pixels to get the image coordinates. Reset and detect packet type on Start of Packet.
// Increment x and y pixels for processing.

reg [10:0] x, y;
reg packet_video;
always@(posedge clk) begin
	if (sop) begin
		x <= 11'h0;                           // start (x, y) at (0, 0)
		y <= 11'h0;
		packet_video <= (blue[3:0] == 3'h0);  // ?
	end
	else if (in_valid) begin
		if (x == IMAGE_W-1) begin   // read the entire row of image pixels x = 0 ~ IMAGE_W - 1 then read the next row above
			x <= 11'h0;
			y <= y + 11'h1;
		end
		else begin
			x <= x + 11'h1;
		end
	end
end


// Find first and last red pixels

reg [10:0] red_x_min, red_y_min, red_x_max, red_y_max;
always@(posedge clk) begin
	if (red_detect & in_valid) begin	
	
	// Update bounds when the pixel is blue
		if (x < red_x_min) red_x_min <= x;
		if (x > red_x_max) red_x_max <= x;
		if (y < red_y_min) red_y_min <= y;
		red_y_max <= y;
	end
	
	if (sop & in_valid) begin	
	
	// Reset bounds on start of packet
		red_x_min <= IMAGE_W-11'h1;
		red_x_max <= 0;
		red_y_min <= IMAGE_H-11'h1;
		red_y_max <= 0;
	end
end

// Find first and last blue pixels
reg [10:0] blue_x_min, blue_y_min, blue_x_max, blue_y_max;
always@(posedge clk) begin
	if (blue_detect & in_valid) begin	
	
	// Update bounds when the pixel is blue
		if (x < blue_x_min) blue_x_min <= x;
		if (x > blue_x_max) blue_x_max <= x;
		if (y < blue_y_min) blue_y_min <= y;
		blue_y_max <= y;
	end
	
	if (sop & in_valid) begin	
	
	// Reset bounds on start of packet
		blue_x_min <= IMAGE_W-11'h1;
		blue_x_max <= 0;
		blue_y_min <= IMAGE_H-11'h1;
		blue_y_max <= 0;
	end
end

// Find first and last yellow pixels
reg [10:0] yellow_x_min, yellow_y_min, yellow_x_max, yellow_y_max;
always@(posedge clk) begin
	if (yellow_detect & in_valid) begin	
	
	// Update bounds when the pixel is yellow
		if (x < yellow_x_min) yellow_x_min <= x;
		if (x > yellow_x_max) yellow_x_max <= x;
		if (y < yellow_y_min) yellow_y_min <= y;
		yellow_y_max <= y;
	end
	if (sop & in_valid) begin	
	
	// Reset bounds on start of packet
		yellow_x_min <= IMAGE_W-11'h1;
		yellow_x_max <= 0;
		yellow_y_min <= IMAGE_H-11'h1;
		yellow_y_max <= 0;
	end
end

// Process bounding box at the end of the frame.
reg [2:0] msg_state;
// reg [10:0] left, right, top, bottom;

reg [10:0] left_red, right_red, top_red, bottom_red;
reg [10:0] left_blue, right_blue, top_blue, bottom_blue;
reg [10:0] left_yellow, right_yellow, top_yellow, bottom_yellow;

reg [7:0] frame_count;

always@(posedge clk) begin
	if (eop & in_valid & packet_video) begin  // Ignore non-video packets
		
		// Latch edges for display overlay on next frame
		
		left_red <= red_x_min;
		right_red <= red_x_max;
		top_red <= red_y_min;
		bottom_red <= red_y_max;

		left_blue <= blue_x_min;
		right_blue <= blue_x_max;
		top_blue <= blue_y_min;
		bottom_blue <= blue_y_max;
		
		left_yellow <= yellow_x_min;
		right_yellow <= yellow_x_max;
		top_yellow <= yellow_y_min;
		bottom_yellow <= yellow_y_max;
		
		
		/*
		left <= x_min;
		right <= x_max;
		top <= y_min;
		bottom <= y_max;
		*/
		
		// Start message writer FSM once every MSG_INTERVAL frames, if there is room in the FIFO
		frame_count <= frame_count - 1;
		
		if (frame_count == 0 && msg_buf_size < MESSAGE_BUF_MAX - 3) begin
			msg_state <= 3'b001;
			frame_count <= MSG_INTERVAL-1;
		end
	end
	
	// Cycle through message writer states once started
	if (msg_state != 3'b000) 
		begin
			if (msg_state == 3'b111) msg_state <= 3'b000;
			else msg_state <= msg_state + 3'b001;
		end

end
	
// Generate output messages for CPU
reg [31:0] msg_buf_in; 
wire [31:0] msg_buf_out;
reg msg_buf_wr;
wire msg_buf_rd, msg_buf_flush;
wire [7:0] msg_buf_size;
wire msg_buf_empty;

`define START_MSG_ID "RBB"

// output

always@(*) begin	// Write words to FIFO as state machine advances
	case(msg_state)
		3'b000: begin
			msg_buf_in = 32'b0;
			msg_buf_wr = 1'b0; 									// outputs nothing     
		end
		3'b001: begin
			msg_buf_in = `START_MSG_ID;							// Message ID
			msg_buf_wr = 1'b1;									// write to buffer
		end
		3'b010: begin
			msg_buf_in = {5'b0, red_x_min, 5'b0, red_y_min};	// Top left coordinate - RED
			msg_buf_wr = 1'b1;
		end
		3'b011: begin
			msg_buf_in = {5'b0, red_x_max, 5'b0, red_y_max}; 	// Bottom right coordinate -RED
			msg_buf_wr = 1'b1;
		end
		3'b100: begin
			msg_buf_in = {5'b0, blue_x_min, 5'b0, blue_y_min};	// Top left coordinate - BLUE
			msg_buf_wr = 1'b1;
		end
		3'b101: begin
			msg_buf_in = {5'b0, blue_x_max, 5'b0, blue_y_max}; 	// Bottom right coordinate - BLUE
			msg_buf_wr = 1'b1;
		end
		3'b110: begin
			msg_buf_in = {5'b0, yellow_x_min, 5'b0, yellow_y_min};	// Top left coordinate - YELLOW
			msg_buf_wr = 1'b1;
		end
		3'b111: begin
			msg_buf_in = {5'b0, yellow_x_max, 5'b0, yellow_y_max}; 	// Bottom right coordinate - YELLOW
			msg_buf_wr = 1'b1;
		end
	endcase
end


// Output message FIFO
MSG_FIFO	MSG_FIFO_inst (
	.clock (clk),
	.data (msg_buf_in),
	.rdreq (msg_buf_rd),
	.sclr (~reset_n | msg_buf_flush),
	.wrreq (msg_buf_wr),
	.q (msg_buf_out),
	.usedw (msg_buf_size),
	.empty (msg_buf_empty)
	);


// Streaming registers to buffer video signal
STREAM_REG #(.DATA_WIDTH(26)) in_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(sink_ready),
	.valid_out(in_valid),
	.data_out({red,green,blue,sop,eop}),
	.ready_in(out_ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop})
);

STREAM_REG #(.DATA_WIDTH(26)) out_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(out_ready),
	.valid_out(source_valid),
	.data_out({source_data,source_sop,source_eop}),
	.ready_in(source_ready),
	.valid_in(in_valid),
	.data_in({red_out, green_out, blue_out, sop, eop})
);


/////////////////////////////////
/// Memory-mapped port		 /////
/////////////////////////////////

// Addresses
`define REG_STATUS    			0
`define READ_MSG    			1
`define READ_ID    				2
`define REG_BBCOL				3

// Status register bits
// 31:16 - unimplemented
// 15:8 - number of words in message buffer (read only)
// 7:5 - unused
// 4 - flush message buffer (write only - read as 0)
// 3:0 - unused


// Process write

reg  [7:0]   reg_status;
reg	[23:0]	bb_col;

always @ (posedge clk)
begin
	if (~reset_n)
	begin
		reg_status <= 8'b0;
		bb_col <= BB_COL_DEFAULT;
	end
	else begin
		if(s_chipselect & s_write) begin
		   if      (s_address == `REG_STATUS)	reg_status <= s_writedata[7:0];
		   if      (s_address == `REG_BBCOL)	bb_col <= s_writedata[23:0];
		end
	end
end


// Flush the message buffer if 1 is written to status register bit 4
assign msg_buf_flush = (s_chipselect & s_write & (s_address == `REG_STATUS) & s_writedata[4]);


// Process reads
reg read_d; //Store the read signal for correct updating of the message buffer

// Copy the requested word to the output port when there is a read.
always @ (posedge clk)
begin
   if (~reset_n) begin
	   s_readdata <= {32'b0};
		read_d <= 1'b0;
	end
	
	else if (s_chipselect & s_read) begin
		if   (s_address == `REG_STATUS) s_readdata <= {16'b0, msg_buf_size, reg_status};
		if   (s_address == `READ_MSG) s_readdata <= {msg_buf_out};
		if   (s_address == `READ_ID) s_readdata <= 32'h1234EEE2;
		if   (s_address == `REG_BBCOL) s_readdata <= {8'h0, bb_col};
	end
	
	read_d <= s_read;
end

// Fetch next word from message buffer after read from READ_MSG
assign msg_buf_rd = s_chipselect & s_read & ~read_d & ~msg_buf_empty & (s_address == `READ_MSG);

/*

module Median_Filter(
	input[10:0] x,
	input [7:0] gaussian,
	input[7:0] reg_stage5,
	input[7:0] one,
	input[7:0] two,
	input[7:0] three,
	input[7:0] four,
	input[7:0] five,
	output[7:0] result
);



*/	

endmodule

