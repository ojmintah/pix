void FBWA(){
  cruise_resetter = true;

  GPIO_values[0] = roll_stabilize();//roll input from stabilizer
  // GPIO_values[1] = RC_angle[1];//pitch directly from rc input
  GPIO_values[1] = pitch_stabilize();
  GPIO_values[3] = RC_angle[3]; //yaw input direct from transmitter
  GPIO_values[4] =160; //set flap servo output  
  GPIO_values[2] = RC_angle[2];


  // //THROTTLE INPUT ASSIGNMENT
  // if (armStatus() == 1){ //if drone is armed
  //   GPIO_values[2] = RC_angle[2]; //write rc in to throttle gpio
  //   //write rc to motor
  // }
  // if (armStatus() == 0){ // if drone is unarmed
  //   GPIO_values[2] = 1; //write 0 to throttle gpio
  // }
}

void FBWA_cruise(){
  GPIO_values[0] = roll_stabilize();//roll input from stabilizer
  GPIO_values[3] = RC_angle[3]; //yaw input direct from transmitter
  GPIO_values[4] = 175; //set flap servo output  
  GPIO_values[1] = altitude_stabilize(1500);
  GPIO_values[2] = (pid_u(RC_angle[10]));//
  
  //THROTTLE INPUT ASSIGNEMENT
  // if (armStatus() == 1){ // if drone is armed
  //   GPIO_values[2] = (pid_u(RC_angle[10]));////////
  //   //  GPIO_values[2] = RC_angle[2];
  //   // Serial.print("  armed");
  // }
  // if (armStatus() == 0){
  //   // Serial.print("   disarmed")
  //   GPIO_values[2] = 5;    //write zero to motor signal
  // }

  // Serial.print("rc pitch input: "); Serial.println(RC_channels[1]);

  //CLIMB AND ALTITUDE
  // if(RC_angle[1]>80 && RC_angle[1]<100){ //if stick is centered
  // // Serial.println("stick centered!");
  //   //then...
  //   // GPIO_values[1] = altitude_stabilize(desired_hold_alt); //maintain the current altitude;
  //   GPIO_values[1] = altitude_stabilize(1500); //maintain the current altitude;
  //   // Serial.print("now alt: "); Serial.println( GPS_results.altitude);
  // }
  // if (RC_angle[1]<80 || RC_angle[1]>100){ //if stick not centered;
  //   GPIO_values[1] = pitch_stabilize(); //climb or descend based on operator input
  // }
}

void Manual_mode(){//gpio inputs is directly from the transmitter:
cruise_resetter = true;
  GPIO_values[0] = RC_angle[0];
  GPIO_values[1] = RC_angle[1];
  GPIO_values[3] = RC_angle[3];
  GPIO_values[4] = 10;//set flap
  GPIO_values[2] = RC_angle[2];
  // if (armStatus() == 0){
  //   // Serial.print(" disarmed");
  //   GPIO_values[2] = 5;    //write zero to motor signal
  // }
  // if (armStatus() == 1){ // if drone is armed
  //   GPIO_values[2] = RC_angle[2];
  //   // Serial.print("  armed")
// }
}



