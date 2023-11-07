#include <Ultrasonic.h>

#include <Servo.h>

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define VA 6
#define VB 5

#define SERVO 3 
Servo servo; 

#define ECHO A0
#define TRIG A1
Ultrasonic sensor(TRIG, ECHO);

int d = 0;
boolean decidindo = false;
void setup() {
 pinMode(IN1,1);
 pinMode(IN2,1);
 pinMode(IN3,1);
 pinMode(IN4,1);
 pinMode(VA,1);
 pinMode(VB,1);

 servo.attach(SERVO);
 servo.write(90);
 
 velocidadeA(100);
 velocidadeB(100);
 
 Serial.begin(9600);
}
char decisao(){
  decidindo = true;
  int desq, ddir = 0;
  servo.write(0);
  delay(200);
  desq = sensor.read();
  Serial.println(desq);
  servo.write(180);
  delay(200);
  ddir = sensor.read();
  Serial.println(desq);
  if(desq > ddir && desq > 20){
     virar_esquerda();
  }else if(ddir > desq && ddir > 20){
    virar_direita();
  }else{
    virar180();
  }
  servo.write(90);
  frente();
  delay(200);
  decidindo = false;
}
void virar_esquerda(){
  digitalWrite(IN1,1);
  digitalWrite(IN2,0);
  digitalWrite(IN3,1);
  digitalWrite(IN4,0);
  delay(500);
}
void virar_direita(){
  digitalWrite(IN1,0);
  digitalWrite(IN2,1);
  digitalWrite(IN3,0);
  digitalWrite(IN4,1);
  delay(500);
}
void frente(){
  digitalWrite(IN1,0);
  digitalWrite(IN2,1);
  digitalWrite(IN3,1);
  digitalWrite(IN4,0);
}
void parar(){
  digitalWrite(IN1,0);
  digitalWrite(IN2,0);
  digitalWrite(IN3,1);
  digitalWrite(IN4,1);
}
void voltar(){
  digitalWrite(IN1,1);
  digitalWrite(IN2,0);
  digitalWrite(IN3,0);
  digitalWrite(IN4,1);
}
void virar180(){
  digitalWrite(IN1,0);
  digitalWrite(IN2,1);
  digitalWrite(IN3,0);
  digitalWrite(IN4,1);
  delay(1000);
}
void velocidadeA(int v){
  analogWrite(VA,v);
}
void velocidadeB(int v){
  analogWrite(VB,v);
}



void loop() {
  if(!decidindo){
    d = sensor.read();
  }  
  if(d < 20){
    parar();
    decisao();
  }
}
