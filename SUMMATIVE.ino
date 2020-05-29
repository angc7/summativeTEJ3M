//Personal Fan: Summative Project
//Angela Chen TEJ3M
//January 24, 2020

#include <IRremote.h>
#include <TimerFreeTone.h>
#include <Servo.h>

//initializing pin numbers
const int sensorL = 13;
const int m1L = 12;
const int m2L = 11;
const int m3L = 10;
const int triggerPin = 9;
const int echoPin = 8;
const int servo2Pin = 7;
const int fanPin = 6;
const int servo1Pin = 5;
const int buzzerPin = 4;
const int remoteL = 3;
const int remotePin = 2;

//analog ping
const int potPin = 5;

//other global variables 
int tempL;
int choice;
int tempChoice;
int potValue;
int distance;
int duration;

//configuring servo motors and IR receiver
Servo Servo1;
Servo Servo2;
IRrecv irrecv(remotePin);
decode_results results;

void setup() {
  // put your setup code here, to run once:
  //attaching servo motors, LEDs, IR remotes, etc to their respective pins
  Servo1.write(0);
  Servo2.write(0);
  pinMode(fanPin, OUTPUT);
  pinMode (m1L, OUTPUT);
  pinMode (m2L, OUTPUT);
  pinMode (m3L, OUTPUT);
  pinMode (sensorL, OUTPUT);
  pinMode (remoteL, OUTPUT);
  pinMode (triggerPin, OUTPUT);
  pinMode (echoPin, INPUT);
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  Servo1.attach(servo1Pin);
  Servo2.attach(servo2Pin);
}

void loop() {

//flow change (conditional statement) continuously loops to see if a signal has been received by the IR remote 
//if yes, then the code will continue, the white LEd will flash and buzzer will beep. If not, it means the user has not pressed a button and no further action is needed.
  
  if (irrecv.decode(&results)) {

    TimerFreeTone(buzzerPin, 300, 100);
    digitalWrite(remoteL, HIGH);
    delay(100);
    digitalWrite(remoteL, LOW);
    Serial.println(results.value, HEX);
 
    irrecv.resume();

  //flow change (conditional statement) checks to see if any of the buttons pressed are any of the mode buttons (1,2 or 3)
  //if yes, the respective light will light up and the mode will start running.
   
    if (results.value == 0x9716BE3F || results.value == 0x3D9AE3F7 || results.value == 0x6182021B) {
      
      digitalWrite(tempL, LOW);
    //flow change (switch statement) checks to see using signal value which button corresponds to which light, then call the corresponding mode method
    //else, it will check to see if it was a fan speed button pressed
    //the impotance of having this conditional statement is that if a button other than 1 2 or 3 is clicked, the corresponding mode light will still remain on.
    
      switch (results.value) {
        case 0x9716BE3F: //Keypad button "1"
          digitalWrite(m1L, HIGH); 
          tempL = m1L;
          choice = 1;
          mode1();

        case 0x3D9AE3F7: //Keypad button "2"
          digitalWrite(m2L, HIGH);
          tempL = m2L;
          choice = 2;
          mode2();
         

        case 0x6182021B: //Keypad button "3"
          digitalWrite(m3L, HIGH);
          tempL = m3L;
          choice = 3;
          mode3();
          
      }
    } else {
      // flow change (switch statement) checks based on IR signal value to see which voltage should be powered to the DC motor, controlling the speed of the fan.
      //if the button pressed isn't 1 to 6, it is ignored.
      switch (results.value) {
        case 0x8C22657B: //Keypad button "4"
          analogWrite(fanPin, 50);

        case 0x488F3CBB: //Keypad button "5"
          analogWrite(fanPin, 58);

        case 0xFF5AA5: //Keypad button "6"
          analogWrite(fanPin, 66);
      }
  }

}
}

void mode1() {
//flow change (conditional repitition) this mode will only run while this is the mode chosen and while the IR recevier has not received another signal
  while (choice == 1 && !irrecv.decode(&results)) {

//continuous rotation
    Servo1.write(180);
    delay(1000);
    Servo1.write(0);
    delay(1000);
  }

}

void mode2() {
//flow change (conditional repitition) this mode will only run while this is the mode chosen and while the IR recevier has not received another signal
  while (choice == 2 && !irrecv.decode(&results)) {

    //user can change direction that fan is facing using the potentiometer
    potValue = analogRead(potPin);

    //reducing the value of the potentiometer to a ratio of 0 to 180, a value that the potentiometer can read
    potValue = map(potValue, 0, 1023, 0, 180);
    Servo1.write(potValue);
    delay(500);
  }

}

void mode3() {
//flow change (conditional repitition) this mode will only run while this is the mode chosen and while the IR recevier has not received another signal
  while (choice == 3 && !irrecv.decode(&results)) {
    
    int position = 0;
    //servo motor goes right to left
    for (int i = 0; i < 18; i++) {
      Servo2.write(position);
      delay(500);
      digitalWrite(triggerPin, HIGH);
      delayMicroseconds (10);
      digitalWrite(triggerPin, LOW);
      duration = pulseIn (echoPin, HIGH);
      distance = (duration * 0.034) / 2;

  //flow change (conditional statement) checks to see if something has been detected within 5cm, if not, the servo keeps going, if yes, fan will rotate to where ultrasonic sensor was
      if (distance < 5) {
        digitalWrite(sensorL, HIGH);
        TimerFreeTone(buzzerPin, 500, 100);
        delay(100);
        Serial.println("Detected!");
        digitalWrite(sensorL, LOW);
        Servo1.write(position);
        delay(500);
      }
      position += 10;
      i++;
    }
//servo motor goes left to right
    position = 180;
    for (int i = 0; i < 18; i++) {
      Servo2.write(position);
      delay(500);
      digitalWrite(triggerPin, HIGH);
      delayMicroseconds (10);
      digitalWrite(triggerPin, LOW);
      duration = pulseIn (echoPin, HIGH);
      distance = (duration * 0.034) / 2;
//flow change (conditional statement) checks to see if something has been detected within 5cm, if not, the servo keeps going, if yes, fan will rotate to where ultrasonic sensor was
      if (distance < 5) {
        digitalWrite(sensorL, HIGH);
        TimerFreeTone(buzzerPin, 500, 100);
        delay(100);
        Serial.println("Detected!");
        digitalWrite(sensorL, LOW);
        Servo1.write(position);
        delay(500);
      }
      position -= 10;
      i++;
    }
  }

}
