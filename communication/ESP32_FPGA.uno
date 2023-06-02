#define RX_PIN 19
#define TX_PIN 18

#include <String>

/* Initialize pins for communucation with vision

 * IO19 - Arduino D5 Vision RX
 * I018 - Arduino D6 Vision TX
 * GND - GNDM
 
*/

// Params
std::string VisionStatus;

void setup() {

  // Start the serial communication with the baud rate for the Serial Monitor
  Serial.begin(115200);
  
  // Start the UART communication with the baud rate set on the FPGA. Adjust the baud rate, data format, and RX pin as needed.
  FPGA_Serial.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  
}

/*
void processVision(Vbuff[7]){

}
*/

void loop() {
  if (FPGA_Serial.available()) {  // Check if there is data available to read
    char c = FPGA_Serial.read();  // Read the incoming byte
    Serial.println(c);  // Print the received byte to the Serial Monitor
  }
}

/*

FPGA_Serial.begin(9600, SERIAL_8N1, RXD2, TXD2); initializes UART communication with a baud rate of 9600, 8 data bits, no parity, and 1 stop bit (8N1 format). The RXD2 and TXD2 constants represent the GPIO numbers where the FPGA is connected. You should replace these with the actual GPIO numbers you're using.
FPGA_Serial.available() checks if there are any new characters available to read on the UART line.
FPGA_Serial.read() reads the newest character from the UART line.

*/
