// Halloween trick ir treat automata
// @RoniBandini, Buenos Aires, Argentina 
// September 2022
// bandini.medium.com

#include <Wire.h> 
#include <Servo.h> 
#include "SoftwareSerial.h"
#include "DFRobot_DF2301Q.h"

// I2C communication
DFRobot_DF2301Q_I2C DF2301Q;

// dfplayer
SoftwareSerial mySerial(10, 11);

Servo myServo; 

int myRandom=0;
int angle=1;
int myDirection=1;

int servoPin=12;
int buttonPin=7;

int buttonValue=0;

int myVolume=25;

int imLeft=0;

# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00  
# define ACTIVATED LOW

void setup ()
{
  mySerial.begin (9600);
  Serial.begin(115200);

  setVolume(myVolume);
  playFirst(); // dummy  
  
  myServo.attach(servoPin); 
  
  center();
  
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.println("Halloween started");  
  Serial.println("Roni Bandini, October 2023");  
  
  // Init the sensor
  while( !( DF2301Q.begin() ) ) {
    Serial.println("Communication with device failed, check the connection");
    delay(3000);
  }
  
  DF2301Q.setVolume(10);
  DF2301Q.setMuteMode(1);   
  DF2301Q.setWakeTime(60);
  delay(1000);
  DF2301Q.playByCMDID(1);
  
}
 
void loop ()
{
 
 buttonValue = digitalRead(buttonPin);
 
 if (buttonValue==1){    

  uint8_t CMDID = 0;
  CMDID = DF2301Q.getCMDID();
  
  if(0 != CMDID) {
    
    Serial.print("Command = ");
    Serial.println(CMDID);

    if (CMDID==9 or CMDID==23){ // trick or treat             
      delay(800);
      Serial.print("Are you a kid or an adult?");
      playSound(1); 
      } 
    
    if (CMDID==5){ // kid
      Serial.print("You can find some candy in the drawer");
      playSound(2); // you can find some candy in the drawer
      delay(2000);
      playSound(7); // happy music
      dance(110);
    } 
    
    if (CMDID==6){ // adult
      Serial.print("Are you ready to enter a world of pain?"); // Big Lebowski tribute :)
      playSound(3);
    }  
    
    if (CMDID==7){ // yes
      Serial.print("Bring the trick");
      playSound(4);  
      delay(1500);
      playSound(6); // creepy music    
      delay(1000); 
      dance(20);
      center();           
    }

    if (CMDID==8){ // no
      Serial.print("Happy halloween");
      playSound(5); 
      delay(3000);
      playSound(7); // happy music
      dance(50);     
    }
    
  }
  
  }

 delay(200);
 
 
}

void left(){
  myServo.write(0);  
}

void smallLeft(){
  myServo.write(100);
  imLeft=1;
}

void smallRight(){
  myServo.write(150);
  imLeft=0;
}

void right(){
  myServo.write(160);
}

void dance(int myIterations){
  int i=1;
  
  while (i<myIterations){
    myServo.write(80);
    delay(100);
    myServo.write(110);
    delay(100);
    i++;
    }

    delay(2000);
}

void center(){
  myServo.write(90);
  delay(2000);
}

void playSound(int myId){  
  delay(500);
  execute_CMD(0x0F,0x01,(byte)myId);  
  delay(500);
}

void playFirst()
{
  execute_CMD(0x3F, 0, 0);  
  delay(500);
  setVolume(myVolume);
  delay(500);
}

void pause()
{
  execute_CMD(0x0E,0,0);
  delay(500);
}

void play()
{
  execute_CMD(0x0D,0,1); 
  delay(500);
}

void playNext()
{
  execute_CMD(0x01,0,1);
  delay(500);
}

void playPrevious()
{
  execute_CMD(0x02,0,1);
  delay(500);
}

void setVolume(int volume)
{
  execute_CMD(0x06, 0, volume);  
  delay(2000);
}

void execute_CMD(byte CMD, byte Par1, byte Par2)
{
    word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
    byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
    Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
    for (byte k=0; k<10; k++)
    {
      mySerial.write( Command_line[k]);
    }
}
