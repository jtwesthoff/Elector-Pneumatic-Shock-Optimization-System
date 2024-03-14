#include <ArduinoJson.h>

// Analog pins for the pressure sensors H1-4(harness nuber)
const int sensorPinH1 = A3;
const int sensorPinH2 = A7;
const int sensorPinH3 = A11;
const int sensorPinH4 = A15;

//Harness Fills and Exhausts H1-4F/E(fill/exhaust)
const int solenoidPinH1F = 3;
const int solenoidPinH1E = 9;
const int solenoidPinH2F = 4;
const int solenoidPinH2E = 10;
const int solenoidPinH3F = 5;
const int solenoidPinH3E = 11;
const int solenoidPinH4F = 6;
const int solenoidPinH4E = 12;

// Digital pins to power the system
const int powerPinH1 = 28; 
const int powerPinH2 = 29; 
const int powerPinH3 = 30; 
const int powerPinH4 = 31; 

//Switch positions
const int switch1 = 51; //Settings 1-4
const int switch2 = 48; //Settings 5-8
int switch1State;
int switch2State;
int switchState;
int previousSwitchState;

//Buttons
const int button1 = 35;
const int button2 = 47;
const int button3 = 39;
const int button4 = 43;
int button1State;
int button2State;
int button3State;
int button4State;
int button;
int previousButtonState;

//Set desired pressure
float desiredPressure = 75;

//total of buttons and switches
int state;

//Fudge Factors
const int generalFudgeTop = 5;
const int generalFudgeBottom = -5;
const int H1Fudge = 1;
const int H2Fudge = 1;
const int H3Fudge = 1;
const int H4Fudge = 1;

//pressure readings
float pressureH1;
float pressureH2;
float pressureH3;
float pressureH4;

//danger barrier for changing pressures
const int danger = 250;


void setup() {
  Serial.begin(9600); // Initialize serial communication

  //Fill and exhausts set to output
  pinMode(solenoidPinH1F, OUTPUT);
  pinMode(solenoidPinH1E, OUTPUT);
  pinMode(solenoidPinH2F, OUTPUT);
  pinMode(solenoidPinH2E, OUTPUT);
  pinMode(solenoidPinH3F, OUTPUT);
  pinMode(solenoidPinH3E, OUTPUT);
  pinMode(solenoidPinH4F, OUTPUT);
  pinMode(solenoidPinH4E, OUTPUT);

  //Power pins set to output
  pinMode(powerPinH1, OUTPUT);
  pinMode(powerPinH2, OUTPUT);
  pinMode(powerPinH3, OUTPUT);
  pinMode(powerPinH4, OUTPUT);

  //Pressure sensor pins set to input
  pinMode(sensorPinH1, INPUT);
  pinMode(sensorPinH2, INPUT);
  pinMode(sensorPinH3, INPUT);
  pinMode(sensorPinH4, INPUT);

  //Power the system
  digitalWrite(powerPinH1, HIGH);
  digitalWrite(powerPinH2, HIGH);
  digitalWrite(powerPinH3, HIGH);
  digitalWrite(powerPinH4, HIGH);

  //Switch Pins
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);

  //Button Pins
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
}

void loop() {
  readSensorValues(); // Read analog values from pressure sensors
  readSwitchAndButtonStates(); // Read switch and button states
  updateSwitchState(); // Update switch state based on readings
  updateButtonState(); // Update button state based on readings
  updateDesiredPressure(); // Update desired pressure based on switch and button states
  adjustSolenoidValves(); // Adjust solenoid valves based on pressure differences
  serializeAndPrintData(); // Serialize and print JSON data
  delay(100); // Delay before next reading
}

void readSensorValues() {
  int sensorValueH1 = analogRead(sensorPinH1);
  int sensorValueH2 = analogRead(sensorPinH2);
  int sensorValueH3 = analogRead(sensorPinH3);
  int sensorValueH4 = analogRead(sensorPinH4);

  pressureH1 = map(sensorValueH1, 0, 1023, 0, 300);
  pressureH2 = map(sensorValueH2, 0, 1023, 0, 300);
  pressureH3 = map(sensorValueH3, 0, 1023, 0, 300);
  pressureH4 = map(sensorValueH4, 0, 1023, 0, 300);
}

void readSwitchAndButtonStates() {
  switch1State = digitalRead(switch1);
  switch2State = digitalRead(switch2);

  button1State = digitalRead(button1);
  button2State = digitalRead(button2);
  button3State = digitalRead(button3);
  button4State = digitalRead(button4);
}

void updateSwitchState() {
  if (switch1State == HIGH) {
    switchState = 0;
    previousSwitchState = 0;
  } else if (switch1State == LOW && switch2State == HIGH) {
    switchState = 4;
    previousSwitchState = 4;
  } else if (switch1State == LOW && switch2State == LOW){
    switchState = 9;
    previousSwitchState = 9;
  }
    else{
    switchState = previousSwitchState;
  }
}

void updateButtonState() {
  if (button1State == LOW) {
    button = 1;
    previousButtonState = 1;
  } else if (button1State == HIGH && button2State == LOW) {
    button = 2;
    previousButtonState = 2;
  } else if (button2State == HIGH && button3State == LOW) {
    button = 3;
    previousButtonState = 3;
  } else if (button3State == HIGH && button4State == LOW) {
    button = 4;
    previousButtonState = 4;
  }else{
    button = previousButtonState;
  }
}

void updateDesiredPressure() {
  state = changeState(switchState, button);
  if(pressureH1 < danger && pressureH2 < danger && pressureH3 < danger && pressureH4 < danger){
    switch (state) {
      case 1:
        desiredPressure = 75;
        break;
      case 2:
        desiredPressure = 100;
        break;
      case 3:
        desiredPressure = 130;
        break;
      case 4:
        desiredPressure = 75;
        break;
      case 5:
        desiredPressure = 75;
        break;
      case 6:
        desiredPressure = 75;
        break;
      case 7:
        desiredPressure = 75;
        break;
      case 8:
        desiredPressure = 75;
        break;
      case 9:
        desiredPressure = 100;
        break;
    }
  }
  
}

void adjustSolenoidValves() {
  float pressureDifferenceH1 = desiredPressure - pressureH1 + H1Fudge;
  float pressureDifferenceH2 = desiredPressure - pressureH2 + H2Fudge;
  float pressureDifferenceH3 = desiredPressure - pressureH3 + H3Fudge;
  float pressureDifferenceH4 = desiredPressure - pressureH4 + H4Fudge;

  controlSolenoid(solenoidPinH1F, solenoidPinH1E, pressureDifferenceH1);
  controlSolenoid(solenoidPinH2F, solenoidPinH2E, pressureDifferenceH2);
  //controlSolenoid(solenoidPinH3F, solenoidPinH3E, pressureDifferenceH3);
  //controlSolenoid(solenoidPinH4F, solenoidPinH4E, pressureDifferenceH4);
}

void serializeAndPrintData() {
  StaticJsonDocument<100> jsonDocument;
  jsonDocument["pressureH1"] = pressureH1;
  jsonDocument["pressureH2"] = pressureH2;
  jsonDocument["pressureH3"] = pressureH3;
  jsonDocument["pressureH4"] = pressureH4;
  jsonDocument["desiredPressure"] = desiredPressure;
  jsonDocument["switch"] = switchState;
  jsonDocument["button"] = button;
  jsonDocument["state"] = state;

  String jsonString;
  serializeJson(jsonDocument, jsonString);
  Serial.println(jsonString);
}

void controlSolenoid(int pinF,int pinE, float pressureDifference){
  if(pressureDifference > generalFudgeTop){
    // If pressure is too low, open the solenoid valve
    digitalWrite(pinF, HIGH);
    digitalWrite(pinE, LOW);
  }else if (pressureDifference < generalFudgeBottom) {
    // If pressure is too high, close the solenoid valve
    digitalWrite(pinF, LOW);
    digitalWrite(pinE, HIGH);
  } else {
    // Pressure is within acceptable range, turn off the solenoid valve
    digitalWrite(pinF, LOW);
    digitalWrite(pinE, LOW);
  }
}

int changeState(int switchState, int button){
  int state = switchState + button;
  if (state>=9){
    return 9;
  }
  return state;
}