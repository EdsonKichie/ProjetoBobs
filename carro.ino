#include <Ultrasonic.h>

#include <Servo.h>
int D1,D2 = 0;
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define VA 6
#define VB 5

#define SERVO 3
Servo SM; 

#define ECHO A0
#define TRIG A1
Ultrasonic US(TRIG, ECHO);

void setup() {
 pinMode(IN1,1);
 pinMode(IN2,1);
 pinMode(IN3,1);
 pinMode(IN4,1);
 pinMode(VA,1);
 pinMode(VB,1);

 SM.attach(SERVO);
 SM.write(90);
 frente();
 
 Serial.begin(9600);
}
bool d = true;
void loop() {
int dist = US.read();

if(d && dist < 20){
  d = false;
  parar();
  decisao();
}
if(!d){
   if(D1 > D2){
    virar_esquerda();
   }else{
      virar_direita();
   }
   d=true;
    velocidadeA(100);
    velocidadeB(100);
    frente();
}
}




int decisao(){
  SM.write(180);
  delay(1000);
  D1 = US.read();
  Serial.println(D1);
  
  SM.write(0);
  delay(1000);
  D2 = US.read();
  Serial.println(D2);
  SM.write(90);
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
void velocidadeA(int v){
  analogWrite(VA,v);
}
void velocidadeB(int v){
  analogWrite(VB,v);
}
