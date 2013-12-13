#include <Wire.h>

#include <Adafruit_MotorShield.h> // modified code at https://github.com/robbyoconnor/Adafruit_Motor_Shield_V2_Library (added short braking support)


#include <SoftwareSerial.h>

// I have adapted Ye's code to use the Adafruit Motor Shield since I was having issues
// I use different methods for range sensing

const int pingPin = 11;  //Ping pin
const int warningPin = 12; // turn on if we get too close
long distance = 0;      //Distance holder

// create a motor shield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// 2 motors exist on m1 and m3 (right and left respectively)
Adafruit_DCMotor *right = AFMS.getMotor(1);
Adafruit_DCMotor *left = AFMS.getMotor(3);

void setup()
{
  Serial.begin(9600);
  AFMS.begin(); // create with the default frequency 1.6KHz
  // set initial speed to max
  right->setSpeed(255);
  right->run(FORWARD); 
  right->run(RELEASE); // turn on motor 

  left->setSpeed(255);
  left->run(FORWARD); 
  left->run(RELEASE); // turn on motor 

  pinMode(pingPin, OUTPUT);
  pinMode(warningPin, OUTPUT);

}

void loop()
{

  distance = getDist();
  delay(200);
  if(distance<=700){
    digitalWrite(warningPin, HIGH);
    leftBackward();
    rightBackward();
    delay(1000);
    breakWheels();
    digitalWrite(warningPin, LOW);
  }
  else if (Serial.available() > 0) {
    char command = Serial.read();

    switch (command){
    case 'f': //Forward
      Serial.println("Going Forward...");
      left->setSpeed(255);
      left->run(BACKWARD); 

      break; 
    case 'l': //Left
      Serial.println("Going Left...");
      rightForward();
      digitalWrite(LeftBreak, HIGH);
      break; 
    case 'b': //Backwards
      Serial.println("Backward");
      driveBackward();
      break; 
    case 'd': //Right
      Serial.println("Right");
      digitalWrite(RightBreak, HIGH);
      leftForward();
      break; 
    case 'b': //Break
      Serial.println("Break");
      breakWheels();
      break; 

    }

  }

  delay(100);
}


int getDistance(){
  //Send ping out with low first to ensure a clean high ping of 2μs or more.
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  return pulseIn(pingPin, HIGH);

}


void rightForward(){
  right->setSpeed(255);
  right->run(BACKWARD);
}

void leftForward(){
  left->setSpeed(255);
  left->run(BACKWARD); 

}

void rightBackward(){
  right->setSpeed(255);
  right->run(BACKWARD): 

}

void leftBackward(){
  left->setSpeed(255);
  left->run(FORWARD);
}

void brakeWheels() {
  right->brakeMotor(); 
  left->brakeMotor(); 
}



