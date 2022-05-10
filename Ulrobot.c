#include <Servo.h>
// servo object to control ultrasonic servo
Servo myservo;
// ultrasonic module ECHO to A0
int Echo_Pin=A0;
// ultrasonic module TRIG to A1
int Trig_Pin=A1;
#define Lpwm_pin  6
#define Rpwm_pin  2
const int pinLB=7;
const int pinLF=5;
const int pinRB=4;
const int pinRF=3;
volatile int D_mix;
volatile int D_mid;
volatile int D_max;
volatile int Front_Distance;
volatile int Left_Distance;
volatile int Right_Distance;
volatile int Right_IR_Value;
volatile int Left_IR_Value;
const int Angle = 90;
const int minAngle = 60;
const int maxAngle = 120;
unsigned char normal_speed = 200;
/*Calculate the distance*/
int checkdistance() {
  digitalWrite(Trig_Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_Pin, LOW);
  float distance = pulseIn(Echo_Pin, HIGH) / 58.00;
  delay(10);
  return int(distance);
}

/*detect the left and right distance*/
void Detect_Left_and_Right__distance() {
  myservo.write(maxAngle);
  delay(400);
  Left_Distance = checkdistance();
  delay(600);
  Serial.print("Left_Distance:");
  Serial.println(Left_Distance);
  myservo.write(minAngle);
  delay(400);
  Right_Distance = checkdistance();
  delay(600);
  Serial.print("Right_Distance:");
  Serial.println(Right_Distance);
  myservo.write(Angle);
}

void setup(){
  myservo.attach(A2);
  Serial.begin(9600);
  D_mix = 10;
  D_mid = 20;
  D_max = 100;
  Front_Distance = 0;
  Left_Distance = 0;
  Right_Distance = 0;
  myservo.write(90);
  pinMode(Echo_Pin, INPUT);      
  pinMode(Trig_Pin, OUTPUT);    
  pinMode(pinLB,OUTPUT); 
  pinMode(pinLF,OUTPUT);
  pinMode(pinRB,OUTPUT);
  pinMode(pinRF,OUTPUT); 
  pinMode(Lpwm_pin,OUTPUT);
  pinMode(Rpwm_pin,OUTPUT);
}

void loop(){
  //obtain the value detected by ultrasonic sensor 
  Front_Distance=checkdistance();
  //if the distance is greater than 0 and less than 50  
  if((Front_Distance < 50)&&(Front_Distance > 0))
{
    //stop
    stopp();
    delay(500);
    myservo.write(minAngle);
    delay(500);
    //measure the left distance
    Left_Distance=checkdistance();
    delay(100);
    myservo.write(maxAngle);
    delay(500);
    //measure the right distance
    Right_Distance=checkdistance();
    delay(100);
    //if distance left is greater than right
    if(Left_Distance > Right_Distance)
    {
      //turn left
      rotate_left(normal_speed);
      myservo.write(Angle);
      delay(300); 
      go_forward(normal_speed);
    }
    else //if the right distance is greater than the left
    {
      // turn right
      rotate_right(normal_speed);
      myservo.write(90);
      delay(300); 
      go_forward(normal_speed);
    }
  }
  else
  {
    //go forward
    go_forward(normal_speed);
  }
}


/*go forward*/
void go_forward(unsigned char speed_val)
{
  digitalWrite(pinRB,HIGH); 
  digitalWrite(pinRF,LOW);
  digitalWrite(pinLB,HIGH);
  digitalWrite(pinLF,LOW);
  analogWrite(Lpwm_pin,speed_val);
  analogWrite(Rpwm_pin,speed_val); 
}

/*go backward*/
void go_backward(unsigned char speed_val)
    {
     digitalWrite(pinRB,LOW);  
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,LOW);  
     digitalWrite(pinLF,HIGH);
     analogWrite(Lpwm_pin,speed_val);
     analogWrite(Rpwm_pin,speed_val);
    }
    
/*turn left*/   
void rotate_left(unsigned char speed_val)
{
  digitalWrite(pinRB,HIGH);
  digitalWrite(pinRF,LOW );  
  digitalWrite(pinLB,LOW); 
  digitalWrite(pinLF,HIGH);
  analogWrite(Lpwm_pin,speed_val);
  analogWrite(Rpwm_pin,speed_val);   
}

/*turn right*/
void rotate_right(unsigned char speed_val)
{
  digitalWrite(pinRB,LOW);  
  digitalWrite(pinRF,HIGH);
  digitalWrite(pinLB,HIGH);
  digitalWrite(pinLF,LOW);  
  analogWrite(Lpwm_pin,speed_val);
  analogWrite(Rpwm_pin,speed_val);
}
  
/*stop*/    
void stopp()
{
  digitalWrite(pinRB,HIGH);
  digitalWrite(pinRF,HIGH);
  digitalWrite(pinLB,HIGH);
  digitalWrite(pinLF,HIGH);
}
