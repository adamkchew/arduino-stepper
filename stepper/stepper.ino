/****************************************************************************** 
SparkFun Easy Driver Basic Demo
Toni Klopfenstein @ SparkFun Electronics
March 2015
https://github.com/sparkfun/Easy_Driver

Simple demo sketch to demonstrate how 5 digital pins can drive a bipolar stepper motor,
using the Easy Driver (https://www.sparkfun.com/products/12779). Also shows the ability to change
microstep size, and direction of motor movement.

Development environment specifics:
Written in Arduino 1.6.0

This code is beerware; if you see me (or any other SparkFun employee) at the local, and you've found our code helpful, please buy us a round!
Distributed as-is; no warranty is given.

Example based off of demos by Brian Schmalz (designer of the Easy Driver).
http://www.schmalzhaus.com/EasyDriver/Examples/EasyDriverExamples.html
******************************************************************************/
//Declare pin functions on Redboard
#define LED 13
#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define EN  6

#define LIMIT1 7
#define LIMIT2 8

//Declare variables for functions
int i;
int j;

//1 for 10 seconds
//2 for 20 seconds
//3 for 40 seconds
//4 for 10 minutes
//5 for 20 minutes
//6 for 30 minutes
//7 for 1 hours
//8 for 6 hours

int hyper = 3;
int speed = 1;

bool enabled = false;

void setup() {
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);

  pinMode(LIMIT1, INPUT);
  pinMode(LIMIT2, INPUT);

  digitalWrite(EN, HIGH); //Setting the motor to set off by default

  Serial.begin(9600); //Open Serial connection for debugging
  Serial.println("Starting. Click on both limit switches at the same time to toggle");
  Serial.println();
}

//Main loop
void loop() {

  //Pushing both limits will toggle on and off
  if(digitalRead(LIMIT1) == LOW && digitalRead(LIMIT2) == LOW) {
    Serial.print("Toggle has been hit will set enabled as ");
    Serial.println(!enabled);

    toggleStepper();
  }

  //Safe and no limit has been hit yet
  if(enabled && digitalRead(LIMIT1) == HIGH && digitalRead(LIMIT2) == HIGH) {
    Serial.println("Stepper motor is currently moving");
    
    step();

  //Limit 1 has been hit and should change direction
  } else if(enabled && digitalRead(LIMIT1) == LOW && digitalRead(LIMIT2) == HIGH) {
    Serial.println("Limit 1 been hit and will reverse the direction");
    
    toggleDirection();
    
    longStep();

  //Limit 2 has been hit and should change direction
  } else if(enabled && digitalRead(LIMIT1) == HIGH && digitalRead(LIMIT2) == LOW) {
    Serial.println("Limit 2 been hit and will reverse the direction");
    
    toggleDirection();
    
    longStep();
  }
}

//Toggle the motor to turn on or off
void toggleStepper() {
  delay(2000);

  enabled = !enabled; //Toggle the on and off

  if(enabled) {
    delay(1000);

    //Toggle the speed settings by holding down the limits longer
    while(digitalRead(LIMIT1) == LOW && digitalRead(LIMIT2) == LOW) {
      Serial.print("Setting hyper at ");
      Serial.println(hyper);
  
      hyper = hyper + 1;
      if(hyper >= 9) hyper = 1; //Resetting the hyper back to original
  
      for(i = 0; i < hyper; i++) {
        digitalWrite(LED,HIGH); //Trigger one step forward
        delay(150);
        digitalWrite(LED,LOW); //Pull step pin low so it can be triggered again
        delay(150);
      }
     
      delay(1000);
    }

    if(hyper == 1) { // 10 seconds
      digitalWrite(MS1, HIGH);
      digitalWrite(MS2, LOW);
      speed = 1;   
    }
    if(hyper == 2) { // 20 seconds
      digitalWrite(MS1, LOW);
      digitalWrite(MS2, HIGH);    
      speed = 1;  
    }
    if(hyper == 3) { // 40 seconds
      digitalWrite(MS1, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
      digitalWrite(MS2, HIGH);   
      speed = 1;
    }
    if(hyper == 4) { // 10 minutes
      digitalWrite(MS1, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
      digitalWrite(MS2, HIGH);   
      speed = 20;
    }
    if(hyper == 5) { // 20 minutes
      digitalWrite(MS1, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
      digitalWrite(MS2, HIGH);   
      speed = 40;
    }
    if(hyper == 6) { // 30 minutes
      digitalWrite(MS1, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
      digitalWrite(MS2, HIGH);   
      speed = 60;
    }
    if(hyper == 7) { // 1 hours
      digitalWrite(MS1, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
      digitalWrite(MS2, HIGH);   
      speed = 120;
    }
    if(hyper == 8) { // 6 hours
      digitalWrite(MS1, HIGH); //Pull MS1, and MS2 high to set logic to 1/8th microstep resolution
      digitalWrite(MS2, HIGH);   
      speed = 720;
    }
    
    digitalWrite(EN, LOW);
  } else {
    digitalWrite(EN, HIGH);
  }
}

void step() {
  for(i= 0; i < 50; i++) {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(speed);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(speed);
  }
}

void longStep() {
  for(i= 0; i < 1000; i++) {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(speed);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(speed);
  }
}


void toggleDirection() {
  delay(2000);

  if(digitalRead(dir) == LOW) {
    digitalWrite(dir, HIGH);
  } else  {
    digitalWrite(dir, LOW);
  }
}
