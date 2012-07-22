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
   
  //Read from RFID reader (ID-12)
  byte i = 0;
  byte val = 0;
  byte code[6];
  byte checksum = 0;
  byte bytesread = 0;
  byte tempbyte = 0;

  if(Serial.available() > 0) {
    if((val = Serial.read()) == 2) {                  // check for header 
      bytesread = 0; 
      while (bytesread < 12) {                        // read 10 digit code + 2 digit checksum
        if( Serial.available() > 0) { 
          val = Serial.read();
          if((val == 0x0D)||(val == 0x0A)||(val == 0x03)||(val == 0x02)) { // if header or stop bytes before the 10 digit reading 
            break;                                    // stop reading
          }

          // Do Ascii/Hex conversion:
          if ((val >= '0') && (val <= '9')) {
            val = val - '0';
          } else if ((val >= 'A') && (val <= 'F')) {
            val = 10 + val - 'A';
          }

          // Every two hex-digits, add byte to code:
          if (bytesread & 1 == 1) {
            // make some space for this hex-digit by
            // shifting the previous hex-digit with 4 bits to the left:
            code[bytesread >> 1] = (val | (tempbyte << 4));

            if (bytesread >> 1 != 5) {                // If we're at the checksum byte,
              checksum ^= code[bytesread >> 1];       // Calculate the checksum... (XOR)
            };
          } else {
            tempbyte = val;                           // Store the first hex digit first...
          };

          bytesread++;                                // ready to read next digit
        } 
      } 

      // Output to Serial:
      if (bytesread == 12) {                          // if 12 digit read is complete
//        Serial.print("5-byte code: ");
//        for (i=0; i<5; i++) {
//          if (code[i] < 16) Serial.print("0");
//          Serial.print(code[i], HEX);
//          Serial.print(" ");
//        }
//        Serial.println();
//
//        Serial.print("Checksum: ");
//        Serial.print(code[5], HEX);
//        Serial.println(code[5] == checksum ? " -- passed." : " -- error.");
//        Serial.println();
      }

      bytesread = 0;
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
