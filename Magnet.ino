#include <AFMotor.h>
#include <SoftwareSerial.h>
#include <Servo.h> 

AF_DCMotor motor(4); //Magnet Coil

SoftwareSerial XbeeSerial(3, 2); // RX, TX for Xbee
char inputChar;
int StatusLEDPin = 13;

Servo rotationServo;
int rotPos = 90;

unsigned long now = 0;
unsigned long timeLastServo = 0;

void setup() {
  pinMode(StatusLEDPin, OUTPUT);
  digitalWrite(StatusLEDPin, LOW);
  
  Serial.begin(9600);  // Serial Connection to RFID reader
  
  XbeeSerial.begin(19200); // Serial Connection to Xbee

  rotationServo.attach(9);

  // Set 'motor' speed. AKA magnet signal strength and turn it off
  motor.setSpeed(255);
  motor.run(RELEASE);
}

void loop() {
  Serial.println(".");  //I'm alive
  delay(1000);
  
  now = millis();
  
  //If exists read command character from Xbee
  while(XbeeSerial.available()){
    inputChar = XbeeSerial.read();
    Serial.println(inputChar);
    switch(inputChar){
       
      case 'F': //Magnet off
      case 'f':
        magnet_off();
        break;
         
      case 'O': //Magnet on
      case 'o':
        magnet_on();
        break;
    }
  }
   
  if((now - timeLastServo) > 15){
    rotationServo.write(rotPos);
    timeLastServo = now;
  }
   
  //End of main loop
}


void magnet_on(){
  digitalWrite(StatusLEDPin, HIGH);
  motor.run(FORWARD);
  delay(10);
}

void magnet_off(){
  digitalWrite(StatusLEDPin, LOW);
  motor.run(BACKWARD);
  delay(100);
  motor.run(RELEASE);
  delay(10);
}
