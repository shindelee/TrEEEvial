module uart_transmitter (
    input wire clk,             // System clock
    input wire reset,           // Active-high reset
    input wire tx_data_ready,   // Asserted when tx_data contains valid data
    input wire [7:0] tx_data,   // Byte to be sent
    output reg tx               // UART output (connects to RX of the receiving device)
);

    localparam IDLE = 2'b00;
    localparam START = 2'b01;
    localparam DATA = 2'b10;
    localparam STOP = 2'b11;

    localparam CLOCK_FREQ = 50000000;  // Clock frequency in Hz
    localparam BAUD_RATE = 9600;       // Baud rate
    localparam TICKS_PER_BIT = CLOCK_FREQ / BAUD_RATE;

    reg [7:0] shift_reg;
    reg [3:0] bit_counter;
    reg [31:0] tick_counter;
    reg [1:0] state;

    always @(posedge clk or posedge reset) begin
        if (reset) begin
            state <= IDLE;
            tx <= 1'b1;  // Idle state for UART is high
        end else begin
            case (state)
                IDLE: begin
                    if (tx_data_ready) begin
                        state <= START;
                        shift_reg <= tx_data;
                        bit_counter <= 3'b000;
                        tick_counter <= TICKS_PER_BIT;
                        tx <= 1'b0;  // Start bit
                    end
                end
                START: begin
                    if (tick_counter == 0) begin
                        state <= DATA;
                        tick_counter <= TICKS_PER_BIT;
                    end else begin
                        tick_counter <= tick_counter - 1;
                    end
                end
                DATA: begin
                    if (tick_counter == 0) begin
                        tx <= shift_reg[0];
                        shift_reg <= shift_reg >> 1;
                        bit_counter <= bit_counter + 1;
                        tick_counter <= TICKS_PER_BIT;
                        if (bit_counter == 7) begin
                            state <= STOP;
                        end
                    end else begin
                        tick_counter <= tick_counter - 1;
                    end
                end
                STOP: begin
                    if (tick_counter == 0) begin
                        tx <= 1'b1;  // Stop bit
                        state <= IDLE;
                    end else begin
                        tick_counter <= tick_counter - 1;
                    end
                end
            endcase
        end
    end
endmodule

