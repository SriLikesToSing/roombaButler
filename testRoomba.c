#include <SoftwareSerial.h>
#include <Streaming.h>


//roomba stuff
int rxPin = 10; //connected to pin 4 (TXD) of iRobot Create 2
int txPin = 11; //connected to pin 3 (RXD) of iRobot Create 2
int ddPin = 5;  //connected to pin 5 (BRC) of iRobot Create 2
int SWITCH = 1;


//IR stuff
int LED = 13; // Use the onboard LED
int isObstaclePin = 2;  // This is our input pin
int isObstaclePin2 = 3;
int right = HIGH;  // HIGH MEANS NO OBSTACLE
int left = HIGH;
int counter = 0;
bool initiation = true;






//Set up a new Software Serial Port
SoftwareSerial softSerial = SoftwareSerial(rxPin, txPin);

void wakeUp(void) {
  Serial.println("Wake up!");
  digitalWrite(ddPin, HIGH);
  delay(100);
  digitalWrite(ddPin, LOW);
  delay(500);
  digitalWrite(ddPin, HIGH);
  delay(2000);
}

void driveLeft(void) {
  Serial.print("Arc left... ");
  softSerial.write(145);
  softSerial.write((byte)0x00);
  softSerial.write(0xc8);
  softSerial.write((byte)0x00);
  softSerial.write(0x01);
}

void driveRight(void) {
  Serial.print("Arc right... ");
  softSerial.write(145);
  softSerial.write((byte)0x00);
  softSerial.write(0x01);
  softSerial.write((byte)0x00);
  softSerial.write(0xc8);
}

void driveStraight(void) {
  Serial.print("Arc straight... ");
  softSerial.write(145);
  softSerial.write((byte)0x00);
  softSerial.write(0xc8);
  softSerial.write((byte)0x00);
  softSerial.write(0xc8);
}

void driveBack(void) {
  softSerial.write(137);
  softSerial.write((byte)255);
  softSerial.write((byte)56);
  softSerial.write((byte)0);
  softSerial.write((byte)0);
}

void driveStop(void) {
  Serial.print("Arc stop... ");
  softSerial.write(145);
  softSerial.write((byte)0x00);
  softSerial.write((byte)0x00);
  softSerial.write((byte)0x00);
  softSerial.write((byte)0x00);
}


void setup() {

  //IR setupp
  pinMode(LED, OUTPUT);
  pinMode(isObstaclePin, INPUT);
  Serial.begin(9600);

  // Defining PinModes
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(ddPin, OUTPUT);
  softSerial.begin(19200);


  Serial.println("Wake up!");
  digitalWrite(ddPin, HIGH);
  delay(100);
  digitalWrite(ddPin, LOW);
  delay(500);
  digitalWrite(ddPin, HIGH);
  delay(2000);

  Serial.println("Set baud rate.");
  for (int i = 0; i < 3; i++) {
    digitalWrite(ddPin, LOW);
    delay(200);
    digitalWrite(ddPin, HIGH);
    delay(200);
  }

  Serial.println("Start OI in Safe Mode.");
  softSerial.write(128); //Starts the OI
  softSerial.write(131); //Set mode to Safe
}


void COMMAND_ONE() {

  while (1) {

    right = digitalRead(isObstaclePin);
    left = digitalRead(isObstaclePin2);

    if (right == LOW && left == HIGH) {
      driveLeft();
    } else if (right == HIGH && left == LOW) {
      driveRight();
    } else if (right == HIGH && left == HIGH) {
      driveStop();
      return;

      //once drive stop == 2 then restart the entire system
    } else if (right == LOW && left == LOW) {
      driveStraight();
    }
  }

}

void COMMAND_TWO() {
  int count = 1;
  while (1) {

    right = digitalRead(isObstaclePin);
    left = digitalRead(isObstaclePin2);

    if(right == LOW && left == HIGH) {
      driveRight();
    }else if (right == HIGH && left == LOW) {
      driveLeft();
    }else if (right == LOW && left == LOW) {
      driveStraight();
    }else if (right == HIGH && left == HIGH) {
      driveStop();
      if(count == 0){
        //drive forwards to massaging spot
        count++;
        continue;
      }
      if(count == 1){
        driveStop();
        delay(3000);
        for(int x=0; x<10; x++){
          driveBack();
          delay(300);
          driveStraight();
          delay(300);
        }
        count++;
      }else if(count == 2){
        //drive backwards back to the spot
        return;
      }
      
    }
  }
}


void loop() {

  /*
     while(BT.available()){
       delay(10);
       //...

       if(state == "food"){
         SWITCH = 0;
       }else if(state == "massage"){
         SWITCH = 1;
       }else{
         SWITCH = -1;
       }

     }
 */

  

  if (SWITCH == 0) {
    COMMAND_ONE();
  } else if (SWITCH == 1) {
    COMMAND_TWO();
  } else {
    exit(0);
  }
}



 
