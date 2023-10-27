#define LED1 13
#define LED2 12
#define LED3 11
#define LED4 10
#define BUT1 9
#define BUT2 8

void setup() {
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(BUT1,INPUT);
  pinMode(BUT2,INPUT);
  Serial.begin(9600);
}
int atual = 0;
int led1s = 0;
int led2s = 0;
void loop() {
  atual = millis();
  if( (atual - led1s) > 500){
    led1s = atual;
    digitalWrite(LED1,!digitalRead(LED1));
  }
  if( (atual - led2s) > 1000){
    led2s = atual;
    digitalWrite(LED2,!digitalRead(LED2));
  }

  if(digitalRead(BUT1) == 1){
    digitalWrite(LED3,HIGH);
    digitalWrite(LED4,LOW);
  }
  if(digitalRead(BUT2) == 0){
    digitalWrite(LED4,HIGH);
    digitalWrite(LED3,LOW);
  }
    
}
