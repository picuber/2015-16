/*
*#########################################################################################################################################
*#Disclaimer:																															#
*#---------------------------------------------------------------------------------------------------------------------------------------#
*#You can use this code for any and all educational purposes.																			#
*#If you want to use it for the Student Robotics Contest you will have to send us an email and link our GitHub repository in your code.	#
*#Also we would like you to mention us and/or Team MAI when you talk about your code to others.											#
*#Our email is: mai.studentrobotics@gmail.com																							#
*#########################################################################################################################################
*/

#include <PinChangeInt.h>
#include <Arduino.h>

// We communicate with the power board at 115200 baud.
#define SERIAL_BAUD 115200

#define FW_VER 0

int const trigUSF = A1;  //front - Analogue Pin
int const trigUSL = A2;  //left - Analogue Pin
int const trigUSB = A3;  //back - Analogue Pin
int const trigUSR = A4;  //right - Analogue Pin

int const echoUSF = 10;  //front
int const echoUSL = 11;  //left
int const echoUSB = 12;  //back
int const echoUSR = 13;  //right

int const motorFLA = 2;  //front left pin A
int const motorFLB = 3;  //front left pin B
int const motorBLA = 4;  //back left pin A
int const motorBLB = 5;  //back left pin B
int const motorFRA = 6;  //front right pin A
int const motorFRB = 7;  //front right pin B
int const motorBRA = 8;  //back right pin A
int const motorBRB = 9;  //back right pin B

volatile int counterFL = 0;
volatile int counterBL = 0;
volatile int counterFR = 0;
volatile int counterBR = 0;

uint8_t EnPwmCmd[4]={0x44,0x02,0xbb,0x01};    // distance measure command[copy-paste]
void setup() {
  Serial.begin(SERIAL_BAUD);
  pinMode(trigUSF, OUTPUT);
  digitalWrite(trigUSF, HIGH);
  pinMode(echoUSF, INPUT);
  pinMode(trigUSL, OUTPUT);
  digitalWrite(trigUSL, HIGH);
  pinMode(echoUSL, INPUT);
  pinMode(trigUSB, OUTPUT);
  digitalWrite(trigUSB, HIGH);
  pinMode(echoUSB, INPUT);
  pinMode(trigUSR, OUTPUT);
  digitalWrite(trigUSR, HIGH);
  pinMode(echoUSR, INPUT);
  
  PCintPort::attachInterrupt(motorFLA, isrFLA, CHANGE);
  PCintPort::attachInterrupt(motorFLB, isrFLB, CHANGE);
  PCintPort::attachInterrupt(motorBLA, isrBLA, CHANGE);
  PCintPort::attachInterrupt(motorBLB, isrBLB, CHANGE);
  PCintPort::attachInterrupt(motorFRA, isrFRA, CHANGE);
  PCintPort::attachInterrupt(motorFRB, isrFRB, CHANGE);
  PCintPort::attachInterrupt(motorBRA, isrBRA, CHANGE);
  PCintPort::attachInterrupt(motorBRB, isrBRB, CHANGE);
  for(int i=0;i<4;i++) {
      Serial.write(EnPwmCmd[i]);
  } 
}

int read_pin() {
  while (!Serial.available());
  int pin = Serial.read();
  return (int)(pin - 'a');
}

void command_read() {
  int pin = read_pin();
  // Read from the expected pin.
  int level = digitalRead(pin);
  // Send back the result indicator.
  if (level == HIGH) {
    Serial.write('h');
  } else {
    Serial.write('l');
  }
}

void command_analogue_read() {
  int pin = read_pin();
  int value = analogRead(pin);
  Serial.print(value);
}

void command_write(int level) {
  int pin = read_pin();
  digitalWrite(pin, level);
}

void command_mode(int mode) {
  int pin = read_pin();
  pinMode(pin, mode);
}

void readUS(int trigPin, int echoPin){
  digitalWrite(trigPin, LOW);                // a low pull on pin COMP/TRIG  triggering a sensor reading
  digitalWrite(trigPin, HIGH);               // reading Pin PWM will output pulses
    
  unsigned long DistanceMeasured=pulseIn(echoPin,LOW);
    
  if(DistanceMeasured>=25000) {              // the reading is invalid.
    Serial.println(10000);    
  } else {
    unsigned int DistanceReal = DistanceMeasured/50;           // every 50us low level stands for 1cm
    Serial.print(DistanceReal);
  }  
}

void isrFLA(){
  int a = digitalRead(motorFLA);
  int b = digitalRead(motorFLB);
  if(a == HIGH && b == LOW || a == LOW && b == HIGH){
    counterFL++;
  } else{//if(a == HIGH && b == HIGH || a == LOW && b == LOW)
    counterFL--;
  }
}

void isrFLB(){
  int a = digitalRead(motorFLA);
  int b = digitalRead(motorFLB);
  if(a == HIGH && b == HIGH || a == LOW && b == LOW){
    counterFL++;
  } else {//if(a == HIGH && b == LOW || a == LOW && b == HIGH)
    counterFL--;
  }
}

void isrBLA(){
  int a = digitalRead(motorBLA);
  int b = digitalRead(motorBLB);
  if(a == HIGH && b == LOW || a == LOW && b == HIGH){
    counterBL++;
  } else{//if(a == HIGH && b == HIGH || a == LOW && b == LOW)
    counterBL--;
  }
}

void isrBLB(){
  int a = digitalRead(motorBLA);
  int b = digitalRead(motorBLB);
  if(a == HIGH && b == HIGH || a == LOW && b == LOW){
    counterBL++;
  } else {//if(a == HIGH && b == LOW || a == LOW && b == HIGH)
    counterBL--;
  }
}

void isrFRA(){
  int a = digitalRead(motorFRA);
  int b = digitalRead(motorFRB);
  if(a == HIGH && b == LOW || a == LOW && b == HIGH){
    counterFR++;
  } else{//if(a == HIGH && b == HIGH || a == LOW && b == LOW)
    counterFR--;
  }
}

void isrFRB(){
  int a = digitalRead(motorFRA);
  int b = digitalRead(motorFRB);
  if(a == HIGH && b == HIGH || a == LOW && b == LOW){
    counterFR++;
  } else {//if(a == HIGH && b == LOW || a == LOW && b == HIGH)
    counterFR--;
  }
}

void isrBRA(){
  int a = digitalRead(motorBRA);
  int b = digitalRead(motorBRB);
  if(a == HIGH && b == LOW || a == LOW && b == HIGH){
    counterBR++;
  } else{//if(a == HIGH && b == HIGH || a == LOW && b == LOW)
    counterBR--;
  }
}

void isrBRB(){
  int a = digitalRead(motorBRA);
  int b = digitalRead(motorBRB);
  if(a == HIGH && b == HIGH || a == LOW && b == LOW){
    counterBR++;
  } else {//if(a == HIGH && b == LOW || a == LOW && b == HIGH)
    counterBR--;
  }
}

void motorStatusFL(){
   Serial.print(counterFL);
   counterFL=0;
}

void motorStatusBL(){
   Serial.print(counterBL);
   counterBL=0;
}

void motorStatusFR(){
   Serial.print(counterFR);
   counterFR=0;
}

void motorStatusBR(){
   Serial.print(counterBR);
   counterBR=0;
}

void loop() {
  // Fetch all commands that are in the buffer
  while (Serial.available()) {
    int selected_command = Serial.read();
    // Do something different based on what we got:
    switch (selected_command) {
      case 'a':
        command_analogue_read();
        break;
      case 'r':
        command_read();
        break;
      case 'l':
        command_write(LOW);
        break;
      case 'h':
        command_write(HIGH);
        break;
      case 'i':
        command_mode(INPUT);
        break;
      case 'o':
        command_mode(OUTPUT);
        break;
      case 'p':
        command_mode(INPUT_PULLUP);
        break;
      case 'v':
        Serial.print("SRcustom:");
        Serial.print(FW_VER);
        break;
      case 'b':
        readUS(trigUSF, echoUSF);
        break;
      case 'c':
        readUS(trigUSL, echoUSL);
        break;
      case 'd':
        readUS(trigUSB, echoUSB);
        break;
      case 'e':
        readUS(trigUSR, echoUSR);
        break;
      case 'u':
        counterFL = 0;
        counterBL = 0;
        counterFR = 0;
        counterBR = 0;
        break;
      case 'w':
        motorStatusFL();
        break;
      case 'x':
        motorStatusBL();
        break;
      case 'y':
        motorStatusFR();
        break;
      case 'z':
        motorStatusBR();
        break;
      default:
        // A problem here: we do not know how to handle the command!
        // Just ignore this for now.
        break;
    }
    Serial.print("\n");
  }
}
