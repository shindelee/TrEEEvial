module uart_transmitter (
  input wire clk,            // System clock
  input wire reset,          // Reset signal
  input wire tx_data_ready,  // Asserted when there is data to send
  input wire [7:0] tx_data,  // 8-bit data to send
  output reg tx              // UART output (connects to RX of another UART)
);

  localparam IDLE = 2'b00;
  localparam START_BIT = 2'b01;
  localparam DATA_BITS = 2'b10;
  localparam STOP_BIT = 2'b11;

  reg [1:0] state = IDLE;    // State of the state machine
  reg [15:0] baud_counter;   // Baud rate counter
  reg [3:0] bit_counter;     // Bit counter for data bits
  reg [7:0] tx_reg;          // Holding register for data to send

  always @(posedge clk or posedge reset) begin
    if (reset) begin
      tx <= 1'b1;            // UART idle state is high
      baud_counter <= 0;
      bit_counter <= 0;
      state <= IDLE;
    end else begin
      case (state)
        IDLE: begin
          if (tx_data_ready) begin
            tx_reg <= tx_data;
            baud_counter <= 104;  // Assuming 9600 baud and 100MHz clock
            state <= START_BIT;
          end
        end
        START_BIT: begin
          if (baud_counter == 0) begin
            tx <= 1'b0;  // Start bit is low
            baud_counter <= 104;
            state <= DATA_BITS;
          end else begin
            baud_counter <= baud_counter - 1;
          end
        end
        DATA_BITS: begin
          if (baud_counter == 0) begin
            tx <= tx_reg[bit_counter];  // Send next data bit
            baud_counter <= 104;
            bit_counter <= bit_counter + 1;
            if (bit_counter == 8) begin
              state <= STOP_BIT;
            end
          end else begin
            baud_counter <= baud_counter - 1;
          end
        end
        STOP_BIT: begin
          if (baud_counter == 0) begin
            tx <= 1'b1;  // Stop bit is high
            baud_counter <= 104;
            bit_counter <= 0;
            state <= IDLE;
          end else begin
            baud_counter <= baud_counter - 1;
          end
        end
      endcase
    end
  end

endmodule

/*
This example assumes a 100MHz clock and a baud rate of 9600. 
The baud_counter value of 104 is derived from (clock_speed / baud_rate) - 1, or (100 * 10^6 / 9600) - 1.

The UART transmission starts with a start bit (low), followed by 8 data bits (LSB first), and ends with a stop bit (high). 
When there's no data to send, the tx output stays high (idle state).

*/