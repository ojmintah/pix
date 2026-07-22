void get_IMU() {
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  
  int8_t temp = bno.getTemp();//temperature sensor
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  imu::Vector<3> magnetometer = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
  imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu::Vector<3> accelerometer = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> linearaccel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  imu::Vector<3> gravity = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
  imu::Quaternion quat = bno.getQuat();

  //assign pitch roll and yaw
  IMU_results.pitch_angle = event.orientation.y;//append pitch angle to orientation array
  IMU_results.roll_angle = -event.orientation.z;//append roll angle to orientation array
  IMU_results.yaw_angle = event.orientation.x;//append yaw angle

  //assign pitch rate, roll rate and yaw rate
  IMU_results.pitch_rate = gyro.y();
  IMU_results.roll_rate = gyro.z();
  IMU_results.yaw_rate = gyro.x();
   
  //assign IMU temperature
  IMU_results.IMU_temp = bno.getTemp();
  // Serial.print("temperature: ");
  // Serial.println(bno.getTemp());
  //IMU data format...values in specific indexes below. Do not uncomment line below. needed only for reference
  //IMU_data = [pitch_angle, roll_angle, yaw_angle, pitch_rate, roll_rate, yaw_rate, IMU_temp]; //needed imu data



  // IMU_results.values[]
  // return IMU_data;
  // Serial.println(sizeof(IMU_results));
  // return  IMU_results;
}
void printIMU_data(){
  Serial.print("pitch angle: ");      Serial.println(IMU_results.pitch_angle);
  Serial.print("roll angle: ");       Serial.println(IMU_results.roll_angle);
  Serial.print("yaw angle: ");        Serial.println(IMU_results.yaw_angle);
  Serial.print("pitch rate: ");       Serial.println(IMU_results.pitch_rate);
  Serial.print("roll rate: ");        Serial.println(IMU_results.roll_rate);
  Serial.print("yaw rate: ");         Serial.println(IMU_results.yaw_rate);
  Serial.print("IMU temperature: ");  Serial.println(IMU_results.IMU_temp);
}


  // Serial.print('\n');
  // Serial.print("pitch: ");
  // Serial.print(pry[0]);https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/arduino-code#
  // Serial.print('\t');
  // Serial.print("roll: ");
  // Serial.print(pry[1]);
  // Serial.print('\t');
  // Serial.print("yaw: ");
  // Serial.println(pry[2]);
  //delay(10);//wait for 10ms
  
    /* Display the floating point data */
  // Serial.print("X: ");
  // Serial.print(event.orientation.x, 4);
  // Serial.print("\tY: ");
  // Serial.print(event.orientation.y, 4);
  // Serial.print("\tZ: ");
  // Serial.print(event.orientation.z, 4);
  // Serial.println("");
  // delay(100);
/* Display the floating point data */
// Serial.print("X: ");
// Serial.print(euler.x());
// Serial.print(" Y: ");
// Serial.print(euler.y());
// Serial.print(" Z: ");
// Serial.print(euler.z());
// Serial.println("");