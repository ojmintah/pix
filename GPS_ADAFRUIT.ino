
void getGPS() // run over and over again
{
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) {

    }//Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    // Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 1 seconds or so, print out the current stats
  if (millis() - timer > 1000) {
    GPS_results.latitude = GPS.latitude;
    GPS_results.longitude = GPS.longitude;
    GPS_results.speed = GPS.speed*0.51444444; //speed is in knots; must convert to m/s;
    GPS_results.altitude = GPS.altitude;
    GPS_results.sats = GPS.satellites;
    GPS_results.heading = GPS.angle;

    // Serial.println(GPS_results.latitude);
    timer = millis(); // reset the timer
    // Serial.print("\nTime: ");
    // if (GPS.hour < 10) { Serial.print('0'); }
    // Serial.print(GPS.hour, DEC); Serial.print(':');
    // if (GPS.minute < 10) { Serial.print('0'); }
    // Serial.print(GPS.minute, DEC); Serial.print(':');
    // if (GPS.seconds < 10) { Serial.print('0'); }
    // Serial.print(GPS.seconds, DEC); Serial.print('.');
    // if (GPS.milliseconds < 10) {
    //   Serial.print("00");
    // } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
    //   Serial.print("0");
    // }
    // Serial.println(GPS.milliseconds);
    // Serial.print("Date: ");
    // Serial.print(GPS.day, DEC); Serial.print('/');
    // Serial.print(GPS.month, DEC); Serial.print("/20");
    // Serial.println(GPS.year, DEC);
    // Serial.print("Fix: "); Serial.print((int)GPS.fix);
    // Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    // if (GPS.fix) {
    //   Serial.print("Location: ");
    //   Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
    //   Serial.print(", ");
    //   Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
    //   Serial.print("Speed (knots): "); Serial.println(GPS.speed);
    //   Serial.print("Angle: "); Serial.println(GPS.angle);
    //   Serial.print("Altitude: "); Serial.println(GPS.altitude);
    //   Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    //   Serial.print("Antenna status: "); Serial.println((int)GPS.antenna);
    // }
  }
}

void printGPS_data(){
  Serial.println("GPS DATA");
  Serial.print("latitude: ");   Serial.println(GPS_results.latitude);
  Serial.print("longitude: ");  Serial.println(GPS_results.longitude);
  Serial.print("altitude:");    Serial.println(GPS_results.altitude);
  Serial.print("speed(m/s): "); Serial.println(GPS_results.speed);
  Serial.print("satellites: "); Serial.println(GPS_results.sats);
  Serial.print("heading: ");    Serial.println(GPS_results.heading);
}