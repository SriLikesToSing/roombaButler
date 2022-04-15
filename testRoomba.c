#include <SoftwareSerial.h>
#include <Streaming.h>


//roomba stuff
int rxPin = 10; //connected to pin 4 (TXD) of iRobot Create 2
int txPin = 11; //connected to pin 3 (RXD) of iRobot Create 2
int ddPin = 5;  //connected to pin 5 (BRC) of iRobot Create 2
int SWITCH = 2;


//IR stuff
int LED = 13; // Use the onboard LED
int isObstaclePin = 2;  // This is our input pin
int isObstaclePin2 = 3;
int right = HIGH;  // HIGH MEANS NO OBSTACLE
int left = HIGH;
int counter = 0;
bool initiation = true;

//IR Sensor
int rxPinBlue = 9;
int txPinBlue = 8;
String state;
SoftwareSerial hc06(8,9);

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
  Serial.print("Arc back... ");
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
  Serial.println("IR SETUP");
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

  //cycling bytes to change baud rate
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



   //initialize bluetooth
  Serial.begin(9600);
  //initialize bluetooth serial port
  hc06.begin(9600);
  String state;
}

void loop() {
  
  //Write data from HC06 to Serial Monitor
  while(hc06.available()){
    delay(10);
    char c = hc06.read();
    state += c;
  }
  
  if(hc06.available()){
    Serial.println("It's printing from here boizo");
    Serial.write(hc06.read());
  }
  
  //Write from Serial Monitor to HC06
  if (Serial.available()){
    hc06.write(Serial.read());
  }  

  if(state == "food please"){
    Serial.println("BRUH");
    int count = 0;
    bool firstTime = true;
    while(1){
    //intro sequence

    if(firstTime == true){
      driveStraight();
      delay(1000);
      driveRight();
      delay(2000);
      firstTime = false;
      continue;
    }
    
    right = digitalRead(isObstaclePin);
    left = digitalRead(isObstaclePin2);   

    if(right == LOW && left == HIGH){
      driveRight();
    }else if (right == HIGH && left == LOW) {
      driveLeft();
    }else if (right == LOW && left == LOW) {
      driveStraight();
    }else if (right == HIGH && left == HIGH) {
     
      if(count == 0){
        driveStop();
        delay(7000);
        driveRight();
        delay(5000);
        count++;
        continue;
      }else if(count == 1){
        driveStop();
        delay(2000);
        driveBack();
        delay(5000);
        driveStop();
        break;
        }
      }
    }
  }

  if(state == "massage me"){
  int count = 0;
  
  while(1){
    
  right = digitalRead(isObstaclePin);
  left = digitalRead(isObstaclePin2);   
    

    if(right == LOW && left == HIGH){
      driveRight();
    }else if (right == HIGH && left == LOW) {
      driveLeft();
    }else if (right == LOW && left == LOW) {
      driveStraight();
    }else if (right == HIGH && left == HIGH) {
      
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
          delay(600);
          driveStraight();
          delay(600);
        }
        driveStop();
        delay(2000);
        driveBack();
        delay(5300);
        driveStop();
        break;
       } 
      }
    }
  }
 state = "";
}




 
