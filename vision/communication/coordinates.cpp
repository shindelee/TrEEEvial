#define RX_PIN 16
#define TX_PIN 17

// Define the baud rate
#define BAUD_RATE 115200

void setup()
{

    // Start the serial communication with the baud rate for the Serial Monitor
    Serial.begin(115200);

    // Start the UART communication with the baud rate set on the FPGA. Adjust the baud rate, data format, and RX pin as needed.
    Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

    // SERIAL_8N1 configuration refers to 8 data bits, no parity bit, and 1 stop bit,
    // which is the most common configuration.
}

void loop()
{
    byte m1, m2, m3, m4;
    byte f1, f2, f3, f4;
    if (Serial1.available() >= 4){
        m1 = Serial1.read();    // read the bytes into byte variables 'b1' to 'b4'
        m2 = Serial1.read();  
        m3 = Serial1.read();
        m4 = Serial1.read();

        uint32_t message;
        message = m1 | (m2 << 8) | (m3 << 16) | (m4 << 24); 

        if(message == 5390914){
            if (Serial1.available() >= 24)
            {
                uint32_t temp;
                uint32_t hexadeci[7];
                char terminal[7][9];
                uint32_t numbers[14];
                byte b1, b2, b3, b4;

                for (int i = 1; i < 6; i++)
                {
                    b1 = Serial1.read(); // read the bytes into byte variables 'b1' to 'b4'
                    b2 = Serial1.read();
                    b3 = Serial1.read();
                    b4 = Serial1.read();

                    hexadeci[i] = b1 | (b2 << 8) | (b3 << 16) | (b4 << 24); // Combine the bytes into a single 32-bit integer

                    // char buffer[9];  // Create a character buffer to hold the hexadecimal representation
                    sprintf(terminal[i], "%08X", hexadeci[i]); // Convert the long value to hexadecimal

                    // Serial.print("Hexadecimal value: 0x");
                    // Serial.println(buffer);

                    // To extract bits 10 through 0, we use a bitwise AND with a mask where these bits are 1 and the others are 0.
                    numbers[2 * i] = hexadeci[i] & 0x7FF;

                    // To extract bits 27 through 16, we again use a bitwise AND, then shift the result right 16 places.
                    numbers[2 * i + 1] = (hexadeci[i] & 0x0FFF0000) >> 16;
                }

                for (int i = 0; i < 6; i++)
                {
                    Serial.print(terminal[i]);
                    Serial.print(" ");
                }

                Serial.print("\n");

                for (int j = 0; j < 12; j++)
                {
                    Serial.print(numbers[j]);
                    Serial.print(" ");
                }

            /*
            // Reference Purposes:
            red_x_min = numbers[2];
            red_y_min = numbers[3];
            red_x_max = numbers[4];
            red_y_max = numbers[5];
            blue_x_min = numbers[6];
            blue_y_min = numbers[7];
            blue_x_max = numbers[8];
            blue_y_max = numbers[9];
            yellow_x_min = numbers[10];
            yellow_y_min = numbers[11];
            yellow_x_max = numbers[12];
            yellow_y_max = numbers[13];
            */
            }
        }
    }
}

