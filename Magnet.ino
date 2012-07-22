#include <AFMotor.h>
#include <SoftwareSerial.h>

AF_DCMotor motor(4); //Magnet Coil
SoftwareSerial XbeeSerial(2, 3); // RX, TX for Xbee

char inputChar;

void setup() {
  Serial.begin(9600);  // Serial Connection to RFID reader
  
  XbeeSerial.begin(19200); // Serial Connection to Xbee

  // Set 'motor' speed. AKA magnet signal strength and turn it off
  motor.setSpeed(255);
  motor.run(RELEASE);
}

void loop() {
  //XbeeSerial.println(".");  //I'm alive
  //If exists read command character from Xbee
  while(XbeeSerial.available()){
    inputChar = XbeeSerial.read()
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
   
  
   
   //End of main loop
}


void magnet_on(){
  motor.setSpeed(255);
  motor.run(FORWARD);
  delay(10);
}

void magnet_off(){
  motor.run(BACKWARD);
  delay(100);
  motor.run(RELEASE);
  delay(10);
}
