byte roll_stabilize(){
  byte U_roll;
  if (RC_angle[0] <= 180 && RC_angle[0] >= 0) {
    roll_desired = map(RC_angle[0], 0, 180, -max_roll, max_roll); 

    roll_error = roll_desired - IMU_results.roll_angle;
    // Serial.print("roll error: ");
    // Serial.println(roll_error);
    // Serial.print("roll angle: "); Serial.println(IMU_results.roll_angle);
    // Serial.print("desired angle: "); Serial.println( roll_desired );

    P_roll = Kp_roll * roll_error;

    I_roll = Ki_roll * (prev_roll_I + (roll_error*DT));

    D_roll = Kd_roll * ((roll_error - prev_roll_error)/DT);

    U_roll = P_roll + I_roll + D_roll + 90;
    // Serial.print("P roll: "); Serial.println(P_roll);
    // Serial.print("I roll: "); Serial.println(I_roll);
    // Serial.print("D_roll: "); Serial.println(D_roll);


  //   //ANTI INTEGRATOR WINDUP


  //UPDATE VALUES
    prev_roll_I = prev_roll_I + (roll_error*DT);
    prev_roll_error = roll_error;
    // GPIO_values[0] =  U_roll;
    
  }
  // Serial.print("U roll: "); Serial.println(U_roll);
  U_roll = max(0, min(U_roll, 180)); //limit U_roll between 0 and 180
      // Serial.print("U_roll: "); Serial.println(U_roll);

  // Serial.print("U_roll: "); Serial.println(U_roll);
  return U_roll;
   
}

//////////////////////////////////////////////////////////////////////

byte pitch_stabilize(){
  byte U_pitch;
  if (RC_angle[1] <= 180 && RC_angle[1] >= 0) {

    pitch_desired = map(RC_angle[1], 0, 180, min_pitch, max_pitch); 

    // Serial.print("pitch in: "); Serial.println(RC_angle[1]);
    // Serial.print("desired pitch: "); Serial.println(pitch_desired);
    // Serial.print("pitch angle: "); Serial.println(IMU_results.pitch_angle);

    pitch_error = pitch_desired - IMU_results.pitch_angle;
     Serial.print("pitch error: ");
    Serial.println(pitch_error);

    P_pitch = Kp_pitch * pitch_error;

    I_pitch = Ki_pitch * (prev_pitch_I + (pitch_error*DT));

    D_pitch = Kd_pitch * ((pitch_error - prev_pitch_error)/DT);

    U_pitch = P_pitch + I_pitch + D_pitch + 90;
    Serial.print("P pitch: "); Serial.println(P_pitch);
    Serial.print("I pitch: "); Serial.println(I_pitch);
    Serial.print("D pitch: "); Serial.println(D_pitch);

  //   //ANTI INTEGRATOR WINDUP

  //UPDATE VALUES
    prev_pitch_I = prev_pitch_I + (pitch_error*DT);
    prev_pitch_error = pitch_error;
    // GPIO_values[1] =  U_pitch;
  }
  U_pitch = max(0, min(U_pitch, 180)); //limit U_pitch between 0 and 180;
      // Serial.print("U pitch: "); Serial.println(U_pitch);
            Serial.print("U pitch: "); Serial.println(U_pitch);


  return U_pitch;
}

float Kp_alt_stab =1.3;
float Ki_alt_stab = 0.0000001;
float Kd_alt_stab = 0;
float prev_I_alt = 0;
float U_alt_stab; 
int prev_H_err = 0;
int H_err;
float P_alt_stab;
float I_alt_stab;
float D_alt_stab;
int desired_hold_alt = 1500;//this is the altitude we want to hold the plane at

//THIS FUNCTION HOLDS ALTITUDE USING PITCH CONTROL
byte altitude_stabilize(int alt){//take a desired altitude as input

  H_err = alt - GPS_results.altitude;// get error
  
  P_alt_stab = Kp_alt_stab * H_err;

  I_alt_stab = Ki_alt_stab * (prev_I_alt + (H_err * DT));

  D_alt_stab = Kd_alt_stab * ((H_err - prev_H_err)/DT);

  U_alt_stab = P_alt_stab + I_alt_stab + D_alt_stab + 90;
  // Serial.print("current alt: "); Serial.print(GPS_results.altitude); Serial.print("\n");
  // Serial.print("alt err: "); Serial.print(H_err);  Serial.print("\n");
  // Serial.print("P: "); Serial.print(P_alt_stab); Serial.print("\n");
  // Serial.print("I "); Serial.print(I_alt_stab); Serial.print("\n");
  // Serial.print("D "); Serial.print(D_alt_stab); Serial.print("\n");
  // Serial.print("U "); Serial.print(U_alt_stab); Serial.print("\n");

  U_alt_stab = max(60, min(U_alt_stab, 60)); //based on the altitude add or subtract 60 from 90 and send to the elevator

  U_alt_stab = U_alt_stab + 90; //keep elevator angle between 30 and 150 degrees
  // Serial.print("U returned "); Serial.print(U_alt_stab); Serial.print("\n");



  //UPDATE VALUES
  prev_I_alt = prev_I_alt + (H_err * DT);

  prev_H_err = H_err;

  return U_alt_stab;
}
