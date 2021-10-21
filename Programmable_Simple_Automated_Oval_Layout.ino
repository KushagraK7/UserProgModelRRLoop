/*
 * Arduino program to control a model train running in a basic oval loop with the help of a feedback sensor.
 * This code allows the user to set the maxSpeed of the locomotive. Watch the video here:
 * https://youtu.be/JzUwDHfsNyw
 * Made by Tech Build: https://www.youtube.com/channel/UCNy7DyfhSD9jsQEgNwETp9g?sub_confirmation=1
 * 
 * You can find more projects here: https://www.instructables.com/member/KushagraK7/
 * 
 */

int s; //Integer variable to store train's speed in the range from 0 to 255.
int maxSpeed = 100;//Integer variable to store the maximum speed the train will reach(max is 255 and is very fast).
int t = 5; //Time delay(pause, in seconds) between each loop of operation, from start to stop.
int progDelay = 5;//Time delay(in seconds) for which the microcontroller waits for the button to be pressed. 
int T = 0;
int prog = 1;

#define A 8 //'A' and 'B' are direction control pins connected to the motor driver.
#define B 9
#define PWM 10 //Motor speed control pin connected to the 'EN' pin of the motor driver.
#define Sensor A0 //Input pin for connecting the 'sensored' track.
#define pot A1//Input pin for connecting the potentiometer.
#define button A2//Input pin for connecting the pushbutton.
#define LED 13//Using LED on D13 for indication.

void motor_go(){
 if(s>=1&&s<=255){
  digitalWrite(B,LOW);
  digitalWrite(A,HIGH);
  analogWrite(PWM,s);
 }
 if(s<=-1&&s>=-255){
  digitalWrite(A,LOW);
  digitalWrite(B,HIGH);
  analogWrite(PWM,-s);
 }
 if(s==0){
  digitalWrite(B,LOW);
  digitalWrite(A,LOW);
  analogWrite(PWM,s);
 }
 
 
}

void setup() {
  // put your setup code here, to run once:
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(LED,OUTPUT);

  pinMode(Sensor,INPUT_PULLUP);
  pinMode(button,INPUT_PULLUP);

  Serial.begin(9600);

  //pinMode(25, OUTPUT);//Added for Raspberry Pi Pico, sice it has no onboard POWER LED.

  //digitalWrite(25, HIGH);//Using the LED on pin GP25 as a power indicator.

  while(digitalRead(button) == HIGH)
  {
    T++;
    delay(1);

    if(T >= (progDelay*1000))
    {
      prog = 0;
      break;
    } 
  }

  if(prog)
  {
    while(digitalRead(button) ==  LOW);
    digitalWrite(A, LOW);

    while(analogRead(pot) > 10); //Wait for potentiometer to be set at a very low value
                              //to prevent the locomotive from moving. 
    

    digitalWrite(A, HIGH);
    digitalWrite(LED, HIGH);

    while(digitalRead(button) == HIGH)
    {
      Serial.println(analogRead(pot));
      maxSpeed = map(analogRead(pot), 0, 1023, 0, 255);
      analogWrite(PWM, maxSpeed);
    }

    digitalWrite(A, LOW);
    digitalWrite(LED, LOW);

    while(digitalRead(button) == LOW);
    delay(100);
    while(digitalRead(button) == HIGH);
    digitalWrite(LED,HIGH);

    delay(2000);
  }
    digitalWrite(LED,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

  for(s=s;s<=20;s++){ //Starting the train.
    motor_go();
    delay(500);
  }

  delay(5000);

  for(s=s;s<=60;s++){ //Speeding up a bit.
    motor_go();
    delay(500);
  }

  delay(1000);

  while(digitalRead(Sensor)==LOW); //Wait for the train to cross the 'sensored' track.

  for(s=s;s<=maxSpeed;s++){ //Speeding up the train to maximum speed value set in the beginning.
    motor_go();
    delay(500);
  }

  delay(1000);
  while(digitalRead(Sensor)==LOW); //Wait for the train to cross the 'sensored' track.

  delay(2000);

  for(s=s;s!=80;s--){ //Slow down the train a bit.
    motor_go();
    delay(1000);
  }

  delay(4000);
  while(digitalRead(Sensor)==LOW); //Wait for the train to cross the 'sensored' track.

  for(s=s;s!=60;s--){ //Slow down the train furthur.
    motor_go();
    delay(500);
  }

  delay(4000);

  for(s=s;s!=20;s--){ //Slow down the train, preparing to stop.
    motor_go();
    delay(500);
  }
  
  while(digitalRead(Sensor)==LOW); //Wait for the train to cross the 'sensored' track.
  
  for(s=s;s!=0;s--){ //Reduce the voltage on the tracks to just let the locomotive lights turned on.
    motor_go();
    delay(500);
  }


  delay(1000*t); //Wait for the set time before starting all over again.
}

