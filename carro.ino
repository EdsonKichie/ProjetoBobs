#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define VA 6
#define VB 5

#define SDIR 2
#define SESQ 3

void setup() {
 pinMode(IN1,1);
 pinMode(IN2,1);
 pinMode(IN3,1);
 pinMode(IN4,1);
 pinMode(VA,1);
 pinMode(VB,1);
 pinMode(SDIR,0);
 pinMode(SESQ,0);
 Serial.begin(9600);

 attachInterrupt(digitalPinToInterrupt(SDIR), alinhar_esquerda, 1);
 attachInterrupt(digitalPinToInterrupt(SESQ), alinhar_direita, 1);
}
void alinhar_esquerda(){
  voltar();
}
void alinhar_direita(){
  frente();
}
void frente(){
  digitalWrite(IN1,1);
  digitalWrite(IN2,0);
  digitalWrite(IN3,1);
  digitalWrite(IN4,0);
}
void parar(){
  digitalWrite(IN1,1);
  digitalWrite(IN2,1);
  digitalWrite(IN3,1);
  digitalWrite(IN4,1);
}
void voltar(){
  digitalWrite(IN1,0);
  digitalWrite(IN2,1);
  digitalWrite(IN3,0);
  digitalWrite(IN4,1);
}
void velocidadeA(int v){
  analogWrite(VA,v);
}
void velocidadeB(int v){
  analogWrite(VB,v);
}

void loop() {
  velocidadeA(100);
  velocidadeB(100);
  
}
