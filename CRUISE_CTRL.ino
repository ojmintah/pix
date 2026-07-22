void cruise_params_init(bool a){
  // V_err = 0;
  if (a == true){
  prev_I_cruise = 0;  //previous integral value
  V_err_prev = 0;
  I_cruise=0;
  D_cruise=0;
}
cruise_resetter = false;
}

byte pid_u(int thr_in) {  //thr_in is the throttle input from the transmitter
  /*FUNCTION*/
  // cruise_params_init(cruise_resetter);
  Vt = 18;//map(thr_in, 0, 180, V_min, V_max);  //obtain target airspeed from throttle input mapping between v_min and v_maxp
  // Serial.print("desired speed: ");
  // Serial.println(Vt);
  // V = GPS_results.speed;        //
  V_err = Vt - GPS_results.speed;//
  // Serial.print("error: ");
  // Serial.println(V_err);
  //CALCULATING P;
  P_cruise = k_p_cruise * V_err;
  // Serial.print("P cruise: "); Serial.println(P_cruise);
  //CALCULATING I;
  I_cruise = k_i_cruise * (prev_I_cruise + (V_err * DT));
  // Serial.print("I cruise: "); Serial.println(I_cruise,6);
  // Serial.print("prev I cruise: "); Serial.println(prev_I_cruise);
  //CALCULATING D;
  D_cruise = k_d_cruise * ((V_err - V_err_prev) / DT);

  //update PID terms for next loop
  V_err_prev = V_err;
  prev_I_cruise = (prev_I_cruise + (V_err * DT));


  U_cruise = P_cruise + I_cruise + D_cruise;
  U_cruise = max(min(170, U_cruise), 0);
  // Serial.print("u-cruise: ");
  // Serial.println(U_cruise);
  return U_cruise;  //send U to the requested function;
}
// }







