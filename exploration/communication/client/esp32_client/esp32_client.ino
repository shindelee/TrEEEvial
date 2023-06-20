#include <WiFi.h>
#include <WebSocketClient.h>
// #include <ArduinoJson.h>

#define RX_PIN 16
#define TX_PIN 17

// Define the baud rate
// #define BAUD_RATE 115200

const char *ssid = "iPhone";
const char *password = "12345678";
char path[] = "/";
char host[] = "18.234.103.77:5000";
int seq_no;
int counter = 0;
int array_count[7] = {0, 1, 1, 0, 1, 0, 0};
int alive_count = 0;
bool start = true;
int message_counter = 0;

// global variables
WebSocketClient webSocketClient;
WiFiClient client;

// funcitons
void initWiFi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// create web socket
void initWebSocket()
{
  if (client.connect("18.234.103.77", 5000))
  {
    Serial.println("Connected");
  }
  else
  {
    Serial.println("Connection failed.");
    while (1)
    {
      // Hang on failure
    }
  }
}

// websocket handshake
void handshake()
{
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client))
  {
    Serial.println("Handshake successful");
    seq_no = 0;
  }
  else
  {
    Serial.println("Handshake failed.");
    while (1)
    {
      // Hang on failure
    }
  }
}

void setup()
{
  Serial.begin(115200);
  // Start the UART communication with the baud rate set on the FPGA. Adjust the baud rate, data format, and RX pin as needed.
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
    delay(10);
    initWiFi();
    delay(50);
    initWebSocket();
    handshake();
}

void loop()
{
  int error = 1;
  int x = counter;
  int y = counter+1;
  int f= array_count[counter];
  int l= array_count[counter+1];
  int r= array_count[counter+2];
  uint32_t numbers[14];
  
//  if (Serial1.available() >= 4)
//  {
//        byte m1 = Serial1.read();    // read the bytes into byte variables 'b1' to 'b4'
//        byte m2 = Serial1.read();  
//        byte m3 = Serial1.read();
//        byte m4 = Serial1.read();
//
//        uint32_t message;
//        message = m1 | (m2 << 8) | (m3 << 16) | (m4 << 24); 
//
//        if(message == 5390914)
//        {
//            if (Serial1.available() >= 24)
//            {
//                uint32_t temp;
//                uint32_t hexadeci[7];
//                char terminal[7][9];
//                byte b1, b2, b3, b4;
//
//                for (int i = 0; i < 6; i++)
//                {
//                    b1 = Serial1.read(); // read the bytes into byte variables 'b1' to 'b4'
//                    b2 = Serial1.read();
//                    b3 = Serial1.read();
//                    b4 = Serial1.read();
//
//                    hexadeci[i] = b1 | (b2 << 8) | (b3 << 16) | (b4 << 24); // Combine the bytes into a single 32-bit integer
//
////                    if (hexadeci[0] == 5390914){
////                      error = true;
////                      Serial.println("Error? " + String(error));
////                      break;
////                    }
//                    
//                    // char buffer[9];  // Create a character buffer to hold the hexadecimal representation
//                    sprintf(terminal[i], "%08X", hexadeci[i]); // Convert the long value to hexadecimal
//
//                    // Serial.print("Hexadecimal value: 0x");
//                    // Serial.println(buffer);
//
//                    // To extract bits 10 through 0, we use a bitwise AND with a mask where these bits are 1 and the others are 0.
//                    numbers[2 * i] = hexadeci[i] & 0x7FF;
//
//                    // To extract bits 27 through 16, we again use a bitwise AND, then shift the result right 16 places.
//                    numbers[2 * i + 1] = (hexadeci[i] & 0x0FFF0000) >> 16;
//                }
//
//
//
//                  for (int i = 0; i < 6; i++)
//                  {
//                    Serial.print(terminal[i]);
//                    Serial.print(" ");
//                  }
//
//                  Serial.print("\n");
//
//                  for (int j = 0; j < 12; j++)
//                  {
//                    Serial.print(numbers[j]);
//                    Serial.print(" ");
//                }
//            }
//        }
//   }
//   delay(500);

  String message1 = "{\"x\":\"" + String(x) + "\",\"y\":\"" + String(y) + "\",\"f\":\"" + String(f) + "\",\"l\":\"" + String(l) + "\",\"r\":\"" + String(r) +  "\",\"alpha\":\"" + String(20) + +  "\",\"beta\":\"" + String(60) + "\"}";
  // String message2 = "{\"x\":\"" + String(3) + "\",\"y\":\"" + String(7) + "\",\"f\":\""+ String(0) + "\",\"l\":\""+ String(1)+ "\",\"r\":\""+ String(1) + "\",\"seq_no\":\""+ String(seq_no)+ "\"}";
  String received_data;
  bool connection = client.connected();
  Serial.println("Is there an error? "+ String(error));
  if (connection)
  {
    webSocketClient.getData(received_data);
    while (start == false && received_data.length() <= 0)
    { // handle disconnection
      initWebSocket();
      handshake();
      delay(500);
      webSocketClient.sendData(message1);
      Serial.println("Resending message #" + String(message_counter));
      delay(1000);
      webSocketClient.getData(received_data);
    }

    Serial.print("Received data: ");
    Serial.println(received_data);
    delay(2000);

    webSocketClient.sendData(message1); // todo: see if can send bytes
    start = false;
    Serial.println("sent message #" + String(message_counter));
    message_counter = message_counter + 1;
    error = false;
    delay(500);

    counter++;
    if (counter == 5)
    {
      counter = 0;
    }
    
  }

  else
  {
    Serial.println("Server disconnected.Trying to reconnect...");
    initWiFi();
    initWebSocket(); // handle disconnections
    handshake();
    delay(1000);
    error = false;
    while (1)
    {
      // Hang on disconnect.
    }
  }
  // wait to fully let the client disconnect
}
