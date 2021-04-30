/**/int tempSensePin = A0;  //This is the Arduino Pin that will read the sensor outputint temSensorInput;    //The variable we will use to store the sensor inputdouble temp;        //The variable we will use to store temperature in degrees. int ldrPin = A1;int ldrInput;int motor_pin_1 = 8;int motor_pin_2 = 10;int motor_pin_3 = 9;int motor_pin_4 = 11;// Number of steps per internal motor revolution const float STEPS_PER_REV = 32;  //  Amount of Gear Reductionconst float GEAR_RED = 64; // Number of steps per geared output rotationconst float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED;int pos = 0;void setup() {  pinMode(motor_pin_1, OUTPUT);  pinMode(motor_pin_2, OUTPUT);  pinMode(motor_pin_3, OUTPUT);  pinMode(motor_pin_4, OUTPUT);  pinMode(tempSensePin, INPUT);  pinMode(ldrPin, INPUT);    Serial.begin(9600); //Start the Serial Port at 9600 baud (default)}void loop() {  long motor_delay = setSpeed(100, STEPS_PER_REV);  //step(STEPS_PER_OUT_REV / 8, STEPS_PER_REV, 0, motor_delay);  /*delay(2000);  motor_delay = setSpeed(100, STEPS_PER_REV);  pos = step(-STEPS_PER_OUT_REV / 4, STEPS_PER_REV, 0, motor_delay);  delay(2000);*/    temp = getTemp();  Serial.print("Current Temperature: ");  Serial.println(temp);    ldrInput = analogRead(ldrPin);  Serial.print("Light Value: ");  Serial.println(ldrInput);    if(ldrInput < 100){    Serial.print(pos);    if(pos == 8){      pos = 0;      step(-STEPS_PER_OUT_REV / 8, STEPS_PER_REV, 0, motor_delay);    }else if(pos == 4) {      pos = 0;      step(-STEPS_PER_OUT_REV / 4, STEPS_PER_REV, 0, motor_delay);          }  }  if(ldrInput > 100 && ldrInput < 400){    Serial.println(pos);    if(pos == 0){      Serial.println("opening");      pos = 8;      step(STEPS_PER_OUT_REV / 8, STEPS_PER_REV, 0, motor_delay);    }else if(pos == 4){      Serial.println("closing");      pos = 8;      step(-STEPS_PER_OUT_REV / 8, STEPS_PER_REV, 0, motor_delay);    }  }  if(ldrInput > 400){    if(pos == 0){      pos = 4;      step(STEPS_PER_OUT_REV / 4, STEPS_PER_REV, 0, motor_delay);    }else if(pos == 8){      pos = 4;      step(STEPS_PER_OUT_REV / 8, STEPS_PER_REV, 0, motor_delay);    }  }  if(temp > 80.0){    if(pos == 4){      step(-STEPS_PER_OUT_REV / 8, STEPS_PER_REV, 0, motor_delay);      pos = 8;          }    else if(pos == 0){      Serial.print("nothing");    }  }  delay(5000);}/* * Sets the speed in revs per minute */long setSpeed(int whatSpeed, float number_of_steps){  long step_delay = 60L * 1000L * 1000L / number_of_steps / whatSpeed;  return step_delay;}/* * Moves the motor steps_to_move steps.  If the number is negative, * the motor moves in the reverse direction. */void step(int steps_to_move, int number_of_steps, int step_number, int step_delay){  int steps_left = abs(steps_to_move);  // how many steps to take  int direction;  long last_step_time = micros();  // determine direction based on whether steps_to_mode is + or -:  if (steps_to_move > 0) { direction = 1; }  if (steps_to_move < 0) { direction = 0; }  // decrement the number of steps, moving one step each time:  while (steps_left > 0)  {    unsigned long now = micros();    // move only if the appropriate delay has passed:    if (now - last_step_time >= step_delay)    {      // get the timeStamp of when you stepped:      last_step_time = now;      // increment or decrement the step number,      // depending on direction:      if (direction == 1)      {        step_number++;        if (step_number == number_of_steps) {          step_number = 0;        }      }      else      {        if (step_number == 0) {          step_number = number_of_steps;        }        step_number--;      }      // decrement the steps left:      steps_left--;      // step the motor to step number 0, 1, ..., {3 or 10}        stepMotor(step_number % 4);    }  }  //return step_number;}/* * Moves the motor forward or backwards. */void stepMotor(int thisStep){  switch (thisStep) {    case 0:  // 1010      digitalWrite(motor_pin_1, HIGH);      digitalWrite(motor_pin_2, LOW);      digitalWrite(motor_pin_3, HIGH);      digitalWrite(motor_pin_4, LOW);    break;    case 1:  // 0110      digitalWrite(motor_pin_1, LOW);      digitalWrite(motor_pin_2, HIGH);      digitalWrite(motor_pin_3, HIGH);      digitalWrite(motor_pin_4, LOW);    break;    case 2:  //0101      digitalWrite(motor_pin_1, LOW);      digitalWrite(motor_pin_2, HIGH);      digitalWrite(motor_pin_3, LOW);      digitalWrite(motor_pin_4, HIGH);    break;    case 3:  //1001      digitalWrite(motor_pin_1, HIGH);      digitalWrite(motor_pin_2, LOW);      digitalWrite(motor_pin_3, LOW);      digitalWrite(motor_pin_4, HIGH);    break;  }}double getTemp(){  temSensorInput = analogRead(tempSensePin);    //read the analog sensor and store it  double temperature = (double)temSensorInput / 1024;       //find percentage of input reading  temperature = temperature * 5;                 //multiply by 5V to get voltage  temperature = temperature - 0.5;               //Subtract the offset   temperature = temperature * 100;               //Convert to degrees   temperature = (temperature * 1.8) + 32;          //Convert to Fahrenheit  return temperature;}