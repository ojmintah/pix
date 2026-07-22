String receivedData = "";

void getTelem() {
  while (Serial1.available()) {
    char c = Serial1.read();

    if (c == ';') {   // End of line
      Serial.print("Received: ");
      Serial.println(receivedData);
      receivedData = "";  // Clear buffer
    } else {
      receivedData += c;  // Append character
    }
    // Serial.println(c);
  }
}

void sendTelem(){
  // DRONE_ID,airspeed,altitude,ground_speed,pitch,roll,heading,travel_heading,battery_voltage,gps_status,gps_satellites,latitude,longitude,temperature\n
  // int g[4] = {2,3,5,6};
  // Serial1.write(g[0]);
  // Serial1.print("MAV_1");
  // Serial1.print(0);
  // Serial1.print(GPS_results.altitude);
  //   Serial1.println(GPS_results.speed );
  //     Serial1.println(GPS_results.speed );



  // Serial1.print(GPS_results.altitude);
  // Serial1.print(GPS_results.altitude);
  // Serial1.print(GPS_results.altitude);
  // Serial1.print(GPS_results.altitude);
  // Serial1.print(GPS_results.altitude);
  // Serial1.print(",");
  // String output = "Values: " + String(a) + ", " + String(b) + ", " + String(c);
  // Serial.println(output);


   Serial1.print("MAV_1");          Serial1.print(",");
  Serial1.print(0);      Serial1.print(",");
  Serial1.print(GPS_results.altitude);      Serial1.print(",");
  Serial1.print(GPS_results.speed);   Serial1.print(",");
  Serial1.print(0);         Serial1.print(",");
  Serial1.print(0);          Serial1.print(",");
  Serial1.print(GPS_results.heading);          Serial1.print(",");
  Serial1.print(GPS_results.heading);    Serial1.print(",");
  Serial1.print(14.8);       Serial1.print(",");
  Serial1.print("fixed");        Serial1.print(",");
  Serial1.print(GPS_results.sats);       Serial1.print(",");
  Serial1.print(GPS_results.latitude, 7);      Serial1.print(",");
  Serial1.print(GPS_results.longitude, 7);     Serial1.print(",");
  Serial1.print(IMU_results.IMU_temp, 1);
  Serial1.print("\n");
  // Serial.println("herle");

  // delay();  // 5 Hz
  
}
