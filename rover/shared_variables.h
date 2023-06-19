// rover specs
extern const float STEPS_PER_REVOLUTION = 200.0;
extern const float WHEEL_RADIUS = 0.0325;
extern const float wheel_diameter = 2.0 * WHEEL_RADIUS;
extern const float wheel_base = 0.14;
extern const float wheel_circumference = wheel_diameter * PI;

// Define the baud rate
#define BAUD_RATE 115200

//motor pin interface
const int LEFT_STEP_PIN = 32;    // A4
const int LEFT_DIR_PIN = 15;     // D12

const int RIGHT_STEP_PIN = 33;   // A3
const int RIGHT_DIR_PIN = 4;     // D11

const int RX_PIN = 16;           // D9 
const int TX_PIN = 17;           // D8

// sensor pin interface
const int leftSensorPin = 35;   // A5
const int frontSensorPin = 34;
const int rightSensorPin = 39;  // vn

// movement state machine
extern const int TWO_WALLS = 1;
extern const int FRONT_WALL = 2;
extern const int LEFT_WALL = 3;
extern const int RIGHT_WALL = 4;
extern const int NO_WALL = 5;
extern const int STATE_CHANGE = 6;

extern int state;
extern bool in_node;
extern int frontSensorReading;
extern int leftSensorReading;
extern int rightSensorReading;
extern int last_sensor_reading_left;
extern int last_sensor_reading_right;
extern bool wall_on_left;
extern bool wall_on_right;
extern bool wall_in_front;
extern int previous_state;
extern int cur_state;

// wall information for message to be sent to EC2
extern bool start = true;
extern String message_to_send = "";
extern String message_received = "";
extern int x = 0; //x coordinate
extern int y = 0; //y coordinate
extern int l = 0; //left wall
extern int f = 0; //front wall
extern int r = 0; //right wall

// websocket and WiFi stuff
extern const char* ssid     = "iPhone";
extern const char* password = "12345678";
extern char path[] = "/";
extern char host[] = "172.20.10.4:5000";
