#include <AFMotor.h>
#include <Ultrasonic.h>
#define pino_IRT 10
#define pino_IRD 9

int distancia;                             
int sensor_traseiro;
int sensor_dianteiro;
int vel_padrao = 100;
int vel_ataque = 255;

Ultrasonic ultrassonico(A4, A3);
AF_DCMotor motor1(1);
AF_DCMotor motor2(3);

void setup() {
  pinMode(pino_IRD, INPUT);
  pinMode(pino_IRT, INPUT);
  motor1.setSpeed(vel_padrao);
  motor2.setSpeed(vel_padrao);
  Serial.begin(9600);
  delay(3000);
}
void ler_sensores() {
  distancia = ultrassonico.read();
  sensor_dianteiro = digitalRead(pino_IRD);
  sensor_traseiro = digitalRead(pino_IRT);
  Serial.println(distancia);
}
void frente(int tempo) {
  motor1.setSpeed(vel_padrao);
  motor2.setSpeed(vel_padrao);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  delay(tempo);
}
void voltar(int tempo) {
  motor1.setSpeed(vel_padrao);
  motor2.setSpeed(vel_padrao);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  delay(tempo);
}
void parar() {
  motor1.setSpeed(vel_padrao);
  motor2.setSpeed(vel_padrao);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}
void atacar() {
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}
void manobrar(int tempo) {
  if (random(0,2) == 0) {
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
  } else {
     motor1.run(FORWARD);
     motor2.run(BACKWARD);
  }
  delay(tempo);
}


void loop() {
  ler_sensores();
  if(distancia > 15 && sensor_dianteiro == 0){
    manobrar(200);
    while(sensor_dianteiro == 0){
       atacar();
       ler_sensores();
    }
  }
  ler_sensores();
  if(sensor_dianteiro == 1){
     parar();
     manobrar(random(12,18)*10);
     voltar(random(6,8)*100);
  }
  ler_sensores();
  if(sensor_traseiro == 1){
     parar();
     manobrar(random(4,8)*10);
     frente(random(6,8)*100);
  }
  ler_sensores();
  manobrar(random(1,3)*10);
  frente(20);
}
