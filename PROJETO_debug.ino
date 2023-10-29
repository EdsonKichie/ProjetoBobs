#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>

#define sensorPin 12

int contador = 0;
bool counting = false;
bool codeEntered = false;
String userCode = "";
bool showInitialScreen = true;

const int LINHAS = 4;
const int COLUNAS = 4;

char TECLAS_MATRIZ[LINHAS][COLUNAS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte PINOS_LINHAS[LINHAS] = {9, 8, 7, 6};
byte PINOS_COLUNAS[COLUNAS] = {5, 4, 3, 2};

Keypad teclado = Keypad(makeKeymap(TECLAS_MATRIZ), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Endereço I2C 0x27, 16 colunas e 2 linhas

void setup() {
  pinMode(sensorPin, INPUT);
  
  Serial.begin(9600);
  lcd.begin(16, 2);

  lcd.print("RECOLHEDOR INTELIGENTE. Pressione 'D'");
}

void loop() {
  char key = teclado.getKey(); // Lê o teclado

  if (showInitialScreen) {
    lcd.clear();
    lcd.print("RECOLHEDOR INTELIGENTE. Pressione 'D'");
    showInitialScreen = false;
  }

  if (key == 'D' && !codeEntered) {
    lcd.clear();
    lcd.print("INSIRA SEU CODIGO");
    codeEntered = true;
    userCode = "";
  }

  if (codeEntered && key != NO_KEY) {
    if (key == '#') {
      lcd.clear();
      lcd.print("Contagem iniciada.");
      counting = true;
    } else {
      userCode += key;
    }
  }

  if (counting && digitalRead(sensorPin)) {
    contador++;
    delay(1000);
  }

  if (counting && key == 'D') {
    lcd.clear();
    lcd.print("Contagem finalizada. Pressione 'D' para reiniciar.");
    counting = false;
    codeEntered = false;
    userCode = "";
    showInitialScreen = true;
  }
}
