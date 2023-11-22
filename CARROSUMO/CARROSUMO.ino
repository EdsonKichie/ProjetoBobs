#include <AFMotor.h>
#include <Ultrasonic.h>

#define pino_TRIG A4
#define pino_ECHO A3
#define pino_IRT 10
#define pino_IRD 9

int distancia;
int sensor_dianteiro;
int sensor_traseiro;
int distancia_ataque = 15;
int vel_padrao = 150;
int vel_ataque = 255;

Ultrasonic ultrassonico(pino_TRIG, pino_ECHO);
AF_DCMotor motor1(1);
AF_DCMotor motor2(3);

void setup() {
  pinMode(pino_IRD, INPUT);
  pinMode(pino_IRT, INPUT);
  delay(3000);
  motor1.setSpeed(vel_padrao);
  motor2.setSpeed(vel_padrao);
  Serial.begin(9600);
}



void ler_sensores() {
  distancia = ultrassonico.read();
  sensor_dianteiro = digitalRead(pino_IRD);
  sensor_traseiro = digitalRead(pino_IRT);
}
void frente() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  delay(1);
}
void voltar() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  delay(500);
}
void parar() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(1);
}
void atacar() {
  motor1.run(FORWARD);
  motor4.run(FORWARD);
  delay(1);
}
void manobrar(int tempo) {
  if (random(1, 3) == 1) {
    motor1.run(FORWARD);
    motor4.run(BACKWARD);
    delay(1);
  } else {
     motor1.run(FORWARD);
     motor4.run(BACKWARD);
}
  delay(tempo);
}


void loop() {
  ler_sensores();
  Serial.println(distancia);
  if(sensor_dianteiro == 1){
    parar();
    voltar();
    parar();
    manobrar(250);
    parar();
  }
  if(sensor_traseiro == 1){
    parar();
    motor1.setSpeed(vel_ataque);
    motor2.setSpeed(vel_ataque);
    atacar();
    delay(500);
    motor1.setSpeed(vel_padrao);
    motor2.setSpeed(vel_padrao);
  }
  if(distancia <= distancia_ataque){
    while (sensor_dianteiro == 0){
      motor1.setSpeed(vel_ataque);
      motor2.setSpeed(vel_ataque);
      atacar();
      ler_sensores();
    }
    motor1.setSpeed(vel_padrao);
    motor2.setSpeed(vel_padrao);
  }
  frente();
}
