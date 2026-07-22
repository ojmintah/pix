// int arm_size = 40;
// int arm_array[40] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


// byte armStatus() {
//   byte arm = 0;
//   byte disarm = 0;
//   for (byte i = 0; i < arm_size; i++) {
//     // delay(1);
//     // delayMicroseconds(500);
//     process_RC();
//     get_RC_to_angle();
//     arm_array[i] = RC_angle[4];
//     // Serial.print("arm_arr "); Serial.print(i+1);Serial.print(" "); Serial.print(arm_array[i]); Serial.print("\n");
//   }

//   for (byte i = 0; i < arm_size; i++) {
//     if (arm_array[i] > 150) {
//       arm = arm + 1;
//     }
//     if (arm_array[i] < 30) {
//       disarm = disarm + 1;
//     }
//   }
//   Serial.print("arm: ");
//   Serial.println(arm);
//   Serial.print("disarm: ");
//   Serial.println(disarm);
//   byte a;
//   // Serial.print("RC  angle ch5: "); Serial.print(RC_angle[4]);Serial.print("\n");
//   // Serial.print("RC  ch5: "); Serial.print(RC_channels[4]);Serial.print("\n");

//   if (disarm > arm) {  //disarmed
//     // Serial.println("                       disarmed");
//     a = 0;  // 0 for disarmed
//     Serial.println("                DISARMED");
//   } else {
//     a = 1;
//     Serial.println("                                ARMED!");
//   }
//   // if(arm>disarm) {
//   //   a = 1;
//   //   Serial.println("                ARMED!");
//   // }//armed
//   // Serial.println("   armed");
//   return a;  //1 for armed
// }

byte flightMode_checker() {
  byte f;
  if (RC_angle[5] < 10) {  //mode 1: FBWA
    // Serial.println("   FBWA");
    FBWA();
    f = 1;  //1 for FBWA
  }
  //   if((RC_angle[5]> 30) && (RC_angle[5]<150)){//mode 2: FBWA + cruise_control
  //   // Serial.println("   FBWA_CRUISE");
  //   f = 2;  //2 for FBWA with cruise control
  //   FBWA_cruise();
  // }
  if (RC_angle[5] > 170) {  //mode 3: manual...for now...change to loiter later
    // Serial.println("   MANUAL");
    FBWA_cruise();
    f = 3;  //3 for manual mode...for now...will be changed to loiter
  }
  return f;
}



void PWM_write() {
  right_aileron_servo.write(GPIO_values[0]);  //right aileron
  left_aileron_servo.write(GPIO_values[0]);   //left aileron
  elevator_servo.write(GPIO_values[1]);       //elevator
  throttle_servo.write(GPIO_values[2]);       //propulsion
  rudder_servo.write(GPIO_values[3]);         //rudder
  left_flap_servo.write(GPIO_values[4]);      //flap
  right_flap_servo.write(GPIO_values[4]);     //flap
  left_elevon_servo.write(90 + (GPIO_values[0] - GPIO_values[1] )*0.5);
  right_elevon_servo.write(90 + (GPIO_values[0] + GPIO_values[1] -180)*0.5 );   

  // left_elevon_servo.write(90 + (GPIO_values[0] + GPIO_values[1] -180)*0.5 );
  // right_elevon_servo.write(90 + (GPIO_values[0] - GPIO_values[1] )*0.5);
}



void GPIO_refine() {
  for (byte i = 0; i < channelAmount; i++) {
    if (i < 4) {
      if (abs(GPIO_values[i] - prev_GPIO_values[i]) > 30) {
        GPIO_values[i] = prev_GPIO_values[i];
      }
    }
  }
}

void printGPIO_values() {
  for (byte i = 0; i < channelAmount; i++) {
    Serial.print("GPIO ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(GPIO_values[i]);
    Serial.print("    ");
  }
  Serial.println();
  // Serial.print("GPIO 1: ");   Serial.println(GPIO_values[0]);
  // Serial.print("GPIO 2: ");   Serial.println(GPIO_values[1]);
  // Serial.print("GPIO 3: ");   Serial.println(GPIO_values[2]);
  // Serial.print("GPIO 4: ");   Serial.println(GPIO_values[3]);
  // Serial.print("GPIO 5: ");   Serial.println(GPIO_values[4]);
  // Serial.print("GPIO 6: ");   Serial.println(GPIO_values[5]);
  // Serial.print("GPIO 7: ");   Serial.println(GPIO_values[6]);
  // Serial.print("GPIO 8: ");   Serial.println(GPIO_values[7]);
}

// int throttleController(int ECS_pwm){}//

// byte elevon_mixing() {
//   //set the mixing gain. Ardupilot default is 0.5
// }
