#include <SoftwareSerial.h>
#include "LiquidCrystal.h"
#include "Keypad.h"
 
#define sensor 12
#define DEFAULT_DELAY 300

int contador = 0;
int contar = true;

const byte LINHAS = 4; // Linhas do teclado
const byte COLUNAS = 4; // Colunas do teclado

int x = 0;                // Holds the LCD x position
int y = 0;                // Holds the LCD y position
int minValue = 0;         // Lower character location for T9 text entry 
int maxValue = 0;         // Max character location for T9 text entry
int keyPressTime = 100;   // Number of loops check of the key
String msg = "";          // Holds the created message
String num = "";          // Holds the mobile number
String alpha = "!@_$%?1 ABC2 DEF3 GHI4 JKL5 MNO6 PQRS7 TUV8 WXYZ9 * 0# "; // Characters for T9 text entry

const char TECLAS_MATRIZ[LINHAS][COLUNAS] = { // Matriz de caracteres (mapeamento do teclado)
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

const byte PINOS_LINHAS[LINHAS] = {9, 8, 7, 6}; // Pinos de conexao com as linhas do teclado
const byte PINOS_COLUNAS[COLUNAS] = {5, 4, 3, 2};      // pin assignments for keypad columns

Keypad teclado_personalizado = Keypad(makeKeymap(TECLAS_MATRIZ), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS); 
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);  // pin assignments for LCD

byte charUp[8] = {  // arrow up character for LCD
  B00100,
  B01110,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte charDown[8] = {  // arrow down character for LCD
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B01110,
  B00100
};
byte charUpDown[8] = {  // arrow up and down character for LCD
  B00100,
  B01110,
  B11111,
  B00000,
  B00000,
  B11111,
  B01110,
  B00100
};

byte menuLevel = 0;     // Level 0: no menu display, display anything you like
                        // Level 1: display main menu
                        // Level 2: display sub menu
                        // Level 3: display sub menu of sub menu
byte menu = 1;          // holds the menu level
byte sub = 1;           // holds the sub menu level













void setup()
{
  pinMode(sensor,INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);               // initialized the LCD as 16 characters with 2 lines
  updateLevel_0();                // display the HOME screen
}


void loop() {
  char leitura_teclas = teclado_personalizado.getKey(); // Atribui a variavel a leitura do teclado
  
  while(!digitalRead(sensor)){
    if(contar){
      contador++;
      contar = false;
    }
  }
  contar = true;
  delay(10);

  processkey();
  
}


void updateLevel_0() {
  lcd.clear();
  lcd.println("  TELA INICIAL  ");
  lcd.setCursor(0, 1);
  lcd.println("  pressione 'D' ");
}

void processkey() {
  char key = teclado_personalizado.getKey();
  if (isAlpha(key)) {   // check if key press is a letter
    processKeys(key);   // process it according to keys
  } 
}


void enterNUM() {
  char key;
  lcd.clear();
  x = 0;
  y = 0;
  num = "";
  do {
    key = teclado_personalizado.getKey();
    if (isDigit(key)) {                 // verify if the key press is a number
      num = num + key;
      lcd.setCursor(x,y);
      lcd.print(key);                   // print it to the lcd
      x++;                              // increment the x position
      if (x > 15)                       // if the lcd reached the rightmost position
      {                                 // then wrap to the next line
        x = 0;
        y = 1;
      }
    } else {                              // key press is not a number
      if (key == '*') {                   // if *, means backspace
        if ( (x > 0) || (y > 0) ) {       // prevent backspace when no character yet
          x = x - 1;                      // go back to previous character position
          lcd.setCursor(x,y);             // set the new lcd position
          lcd.print("*");                 // write *, which means for editing
          num.remove(num.length() - 1);   // remove the last character from the string
        }
      }
    }
  } while (key != 'D');                   // exit the loop when # is pressed
                                          // means entering number is complete
  lcd.setCursor(0, 0);
  lcd.print("Codigo enviado");
  lcd.setCursor(0, 1);
  lcd.print(num);
  delay(800);
  lcd.clear();
  
}


void processKeys(char keyPressed) {
  switch (menuLevel) {
    case 0:                     // Level 0, home screen
      switch ( keyPressed ) {
        case 'D':               // Enter
          menu = 1;
          menuLevel = 1;        // go to main menu
          delay(DEFAULT_DELAY);
          break;
        default:
          break;
      }
      break;
    case 1:                     // Level 1, main menu
      switch ( keyPressed ) {
        case 'D':               // Enter
          sub = 1;
          menuLevel = 2;        // go to sub menu
          updateLevel_2();      // show sub menu
          delay(DEFAULT_DELAY);
          break;
        default:
          break;
      }
      break;
    case 2:                     // Level 2, sub menu
      switch ( keyPressed ) {
        case 'D':               // Enter
          if        (sub == 1) {  // Create SMS
            lcd.clear();
            lcd.print("Digite o codigo");
            delay(1000);
            enterNUM();
            menuLevel = 2;        // go to sub menu
            updateLevel_2();      // show sub menu
          } 
          delay(DEFAULT_DELAY);
          if(contar){
            lcd.clear();
            lcd.print("Pratos:");
            lcd.setCursor(0, 1);
            lcd.print(contador);
          }
          break;
        case 'C': // Back
          menuLevel = 0;        // go to home screen
          updateLevel_0();      // show home screen
          delay(DEFAULT_DELAY);
          break;
        default:
          break;
      }
      break;
    case 3:                     // Level 3, sub menu of sub menu
      switch ( keyPressed ) {
        case 'C': // Back
          menuLevel = 0;        // go to home screen
          updateLevel_0();      // show home screen
          delay(DEFAULT_DELAY);
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}



void updateLevel_1 () {
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">CODIGO         ");
      break;
    case 4:
      menu = 3;
      break;
  }
}





void updateLevel_2 () {
  switch (menu) {

    case 2:                                 // Relay 2
      switch (sub) {
        case 0:
          break;
        case 1:
          lcd.clear();
          lcd.print(" Contacts:      ");
          lcd.setCursor(0, 1);
          lcd.print("  Save Contacts ");
          lcd.setCursor(15, 1);
          lcd.write((byte)1);     // down arrow
          break;
        case 2:
          lcd.clear();
          lcd.print(" Contacts:      ");
          lcd.setCursor(0, 1);
          lcd.print("  Edit Contacts ");
          lcd.setCursor(15, 1);
          lcd.write((byte)2);     // up and down arrow
          break;
        case 3:
          lcd.clear();
          lcd.print(" Contacts:      ");
          lcd.setCursor(0, 1);
          lcd.print("  Del Contacts");
          lcd.setCursor(15, 1);
          lcd.write((byte)0);     // up arrow
          break;
        default:
          break;
      }
      break;
    case 3:                               // Relay 3
      switch (sub) {
        case 0:
          break;
        case 1:
          lcd.clear();
          lcd.print(" Others:        ");
          lcd.setCursor(0, 1);
          lcd.print("  Send AT       ");
          lcd.setCursor(15, 1);
          lcd.write((byte)1);     // down arrow
          break;
        case 2:
          lcd.clear();
          lcd.print(" Others:        ");
          lcd.setCursor(0, 1);
          lcd.print("  Send ATA      ");
          lcd.setCursor(15, 1);
          lcd.write((byte)2);     // up and down arrow
          break;
        case 3:
          lcd.clear();
          lcd.print(" Others:        ");
          lcd.setCursor(0, 1);
          lcd.print("  Send ATH      ");
          lcd.setCursor(15, 1);
          lcd.write((byte)0);     // up arrow
          break;
        default:
          break;
      }
      break;
    case 4:
      sub = 3;
      break;
  }
}






void updateLevel_3 () {
  switch (menu) {
    case 0:
      break;
    case 1:
      lcd.clear();
      lcd.print(" Relay 1:");
      lcd.setCursor(0, 1);
      lcd.print("  Val 1 = ");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Relay 2:");
      lcd.setCursor(0, 1);
      lcd.print("  Val 2 = ");
      break;
    case 3:
      lcd.clear();
      lcd.print(" Relay 3:");
      lcd.setCursor(0, 1);
      lcd.print("  Val 3 = ");
      break;
    case 4:
      sub = 3;
      break;
  }
}





void executeAction () {
  switch (menu) {
    case 0:
      break;
    case 1:
      lcd.clear();
      lcd.print(" Executing # 1");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Executing # 2");
      break;
    case 3:
      lcd.clear();
      lcd.print(" Executing # 3");
      break;
    case 4:
      sub = 3;
      break;
  }
}
