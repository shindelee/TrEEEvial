#define RX_PIN 16
#define TX_PIN 17

// Define the baud rate
#define BAUD_RATE 115200

// #include <String>

/* Initialize pins for communucation with vision

 * IO19 - Arduino D5 Vision RX
 * I018 - Arduino D6 Vision TX
 * GND - GNDM
 
*/

void setup() {

  // Start the serial communication with the baud rate for the Serial Monitor
  Serial.begin(115200);
  
  // Start the UART communication with the baud rate set on the FPGA. Adjust the baud rate, data format, and RX pin as needed.
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

  // SERIAL_8N1 configuration refers to 8 data bits, no parity bit, and 1 stop bit, 
  // which is the most common configuration.
  
}

void loop() {
  if (Serial1.available() >= 4) {  // Check if there are at least 4 bytes available to read
   
    byte b1 = Serial1.read();    // read the bytes into byte variables 'b1' to 'b4'
    byte b2 = Serial1.read();  
    byte b3 = Serial1.read();
    byte b4 = Serial1.read();

    uint32_t val = b1 | (b2 << 8) | (b3 << 16) | (b4 << 24); // Combine the bytes into a single 32-bit integer

    Serial.println(val, HEX);  // Print the combined value
    delay(1000);
  }
}
