/*------------- BNO055 IMU ---------------*/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);
struct IMU_data {
  // float values[7];
  float pitch_angle;
  float roll_angle;
  float yaw_angle;
  float pitch_rate;
  float roll_rate;
  float yaw_rate;
  float IMU_temp;
};
IMU_data IMU_results;

//SETUP SERVOS
#include <Servo.h>          //import servo library
Servo right_aileron_servo;  //create servo objects
Servo left_aileron_servo;
Servo elevator_servo;
Servo throttle_servo;
Servo rudder_servo;
Servo left_flap_servo;
Servo right_flap_servo;
Servo left_elevon_servo;
Servo right_elevon_servo;

//------------ADAFRUIT ULTIMATE GPS-------------//
#include <Adafruit_GPS.h>
#define GPSSerial Serial2
Adafruit_GPS GPS(&GPSSerial);
#define GPSECHO false
uint32_t timer = millis();

struct GPS_data {
  float latitude;
  float longitude;
  int altitude;
  int speed;  //  m/s
  byte sats;
  int heading;
};
GPS_data GPS_results;

//------SETUP RC--------//
#define PPM_PIN          2      // GPIO connected to receiver PPM
#define MAX_CHANNELS     10     // upper bound on channels to capture
#define SYNC_PULSE_MIN   3000   // µs: gap longer than this = end of frame
#define CHANNEL_MIN      800    // µs: minimum plausible channel pulse
#define CHANNEL_MAX      2200   // µs: maximum plausible channel pulse
const int channelAmount = 6;
// volatile uint16_t RC_channels[channelAmount];
// volatile uint16_t prev_RC_channels[channelAmount] = { 1000, 1000, 1000, 1000, 1000, 1000 };  //for low pass filtering
// // volatile uint8_t currentChannel = 0;
// // volatile uint32_t lastTime = 0;                                          //an array to store ppm values:1000us to 2000us

int ppmChannels[MAX_CHANNELS];
volatile uint8_t  channelIndex = 0;
volatile uint8_t  numChannels  = 0;
volatile bool     frameReady   = false;
volatile uint32_t lastEdgeTime = 0;



int RC_angle[8];                                                   //an array to receive RC input converted to angle range
int GPIO_values[channelAmount] = {
  90,
  90,
  0,
  90,
  90,
  90,
};  //to hold pwm values for servo outputs
int prev_GPIO_values[channelAmount] = {
  90,
  90,
  0,
  90,
  90,
  90,
};
float elevon_mixing_gain = 0.5; //the elevon mixing gain for flying wings.

//---------TELEMETRY-----------//
// #define TELEMETRY_BUFFER_SIZE 128
// char telemetryBuffer[TELEMETRY_BUFFER_SIZE];
// int bufferIndex = 0;

float prevTime = millis();  //to measure timing;
float DT;

float roll_desired;
float max_roll = 45;  //maximum allowable roll angle
float roll_error;
float P_roll;
float I_roll;
float D_roll;
float prev_roll_I;
float prev_roll_error;
float Kp_roll = 2;
float Ki_roll = 0;
float Kd_roll = 0;

float pitch_desired;
float max_pitch = 40;   //maximum allowable pitch angle
float min_pitch = -40;  //minimum allowable pitch angle
float pitch_error;
float P_pitch;
float I_pitch;
float D_pitch;
float prev_pitch_I;
float prev_pitch_error;
float Kp_pitch = 4;
float Ki_pitch = 0;
float Kd_pitch = 0;


//CRUISE CONTROL PARAMETERS
float P_cruise;
float I_cruise;
float D_cruise;
float U_cruise;

/*PID GAINS*/
float k_p_cruise = 3;  //proportional gain for cruise control PID
float k_i_cruise = 3;  //integral gain for cruise control PID
float k_d_cruise = 0.0;  //derivative gain for cruise control PID

/*PARAMETERS TO BE UPDATED AFTER EACH LOOP*/
float prev_I_cruise = 0;  //previous integral value
float V_err_prev = 0;     //previous cruise airspeed error used in the derivative part of the PID controller

//CONTROLLER INPUT AND FEEDBACK
int Vt;      //this is target airspeed (must be integer); used to map throttle input to velocity range
int V_aspd;  // measured current airspeed
int V_err;   //velocity error, E(s) = R(s) - Y(s)

/*****VELOCITY RANGE*****/
int V_max = 20;  //maximum airspeed in m/s
int V_min = 8;   //minimum or stall airspeed in m/s

bool cruise_resetter = false;  //this parameter is used to reset the cruise control PID parameters

void setup() {
 
  
  //ATTACH SERVO PINS
  // right_aileron_servo.attach(13);  //create servo objects
  left_aileron_servo.attach(6);
  elevator_servo.attach(3);
  throttle_servo.attach(4);
  rudder_servo.attach(5);
  left_flap_servo.attach(11);
  right_flap_servo.attach(10);
  left_elevon_servo.attach(12);
  right_elevon_servo.attach(13);
  PWM_write();

  Serial.begin(115200);

  //BNO055 setup
  Serial.println("Adafruit GPS library basic parsing test!");
  Wire.setSDA(20);
  Wire.setSCL(21);
  Wire1.begin();
  // Serial.begin(115200);
  delay(5000);

  /*BNO055 IMU SETUP*/
  Serial.println("Orientation Sensor Test");
  Serial.println("");
  /* Initialise the sensor */
  if (!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1)
      ;
  }
  delay(1000);
  bno.setExtCrystalUse(true);

  //-----------TELEMTRY_SETUP------------//
  Serial1.begin(57600);  // UART telemetry radio / device

  // //ppm rc setup
  // pinMode(PPM_PIN, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(PPM_PIN), get_RC, RISING);
  // Serial.println("PPM Reader Started");
  // delay(1000);
}

unsigned int last_telem = millis();
void loop() {
  get_IMU(); //get IMU data

  
  // Serial.println("hw3!");

  // // get_RC_to_angle();                          //get RC data
  DT = (millis() - prevTime) / 1000;  //calc time interval;
  prevTime = millis();                //reset timer value
  
  if ((millis() - last_telem) > 100) {
    // Serial.print("elev1 ");
    // Serial.println(90 + (GPIO_values[0] + GPIO_values[1] -180)*0.5);
    // Serial.print("elev2 ");
    // Serial.println(90 + (GPIO_values[0] - GPIO_values[1] )*0.5);
    // Serial.print("GPIO_values[0] ");Serial.println(GPIO_values[0] );
    // Serial.print("GPIO_values[1] ");Serial.println(GPIO_values[1] );
    sendTelem();//transmit telemtry data
    // Serial.println(elevon_mixing_gain*(GPIO_values[0] - 90) + 90);
    //send telem or do something
    // printRC_values();    
    // print_RC_angle();
    // printGPIO_values();
    // printIMU_data();
    // printRC_values();
    // Serial.println(prev_RC_channels);
    // print_prev_RC_channels();
    last_telem = millis();
  }
  flightMode_checker();  
  // // GPIO_refine();             //this function does the flying
  PWM_write();                        //write pwm values to servos
}

//OPERATION ON SECOND CORE;
void setup1() {
  pinMode(PPM_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PPM_PIN), ppmISR, RISING);

  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);  // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);
}

void loop1() {
  getGPS();
  process_RC();
}
//angularOrientation();
// getGPS();
// if(Serial2.available()){
//   String line = Serial2.readStringUntil('\n');
//   Serial.println(line);
// }
//   char c = GPS.read();
// // If a sentence is received, check if it's a valid fix
// if (GPS.newNMEAreceived()) {
//   if (!GPS.parse(GPS.lastNMEA())) {
//     return; // Skip this loop if the sentence isn't valid
//   }
// }

// // Check if we have a valid GPS fix
// if (GPS.fix) {
//   Serial.print("Location: ");
//   Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
//   Serial.print(", ");
//   Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);

//   Serial.print("Speed (knots): "); Serial.println(GPS.speed);
//   Serial.print("Angle: "); Serial.println(GPS.angle);
//   Serial.print("Altitude: "); Serial.println(GPS.altitude);
//   Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
// }

//     delay(10);

//   // put your main code here, to run repeatedly:

// }


// /*********
//   Rui Santos
//   Complete project details at https://RandomNerdTutorials.com/raspberry-pi-pico-i2c-scanner-arduino/

//   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
//   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// *********/

// #include <Wire.h>

// void setup() {
//   Wire.setSDA(20);
//   Wire.setSCL(21);


//   Wire.begin();
//   Serial.begin(115200);
//   Serial.println("\nI2C Scanner");
// }

// void loop() {
//   byte error, address;
//   int nDevices;
//   Serial.println("Scanning...");
//   nDevices = 0;
//   for(address = 1; address < 127; address++ ) {
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();
//     if (error == 0) {
//       Serial.print("I2C device found at address 0x");
//       if (address<16) {


//         Serial.print("0");
//       }
//       Serial.println(address,HEX);
//       nDevices++;
//     }
//     else if (error==4) {
//       Serial.print("Unknow error at address 0x");
//       if (address<16) {
//         Serial.print("0");
//       }
//       Serial.println(address,HEX);
//     }
//   }
//   if (nDevices == 0) {
//     Serial.println("No I2C devices found\n");
//   }
//   else {
//     Serial.println("done\n");
//   }
//   delay(5000);
// }