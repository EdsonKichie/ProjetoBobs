#include <SoftwareSerial.h>
#include <PN532_SWHSU.h>
#include <PN532.h>




#include "LiquidCrystal.h"
#include "Keypad.h"



#define sensor 12
int contador = 0;
int contar = true;


#define DEFAULT_DELAY 300

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
  lcd.createChar(0, charUp);      // arrow up character
  lcd.createChar(1, charDown);    // arrow down character
  lcd.createChar(2, charUpDown);  // arrow up and down character
  updateLevel_0();                // display the HOME screen





 
}




















void loop() {
  char leitura_teclas = teclado_personalizado.getKey(); // Atribui a variavel a leitura do teclado

  if (leitura_teclas) { // Se alguma tecla foi pressionada
    Serial.println(leitura_teclas); // Imprime a tecla pressionada na porta serial
  }
  

  
  while(!digitalRead(sensor)){
    if(contar){
      contador++;
      contar = false;
    }
  }


  
  Serial.println("Pratos:");
  Serial.println(contador);

  contar = true;
  delay(10);
  


  processkey();

  
  if (lcd.print(num)){
    lcd.clear();
    lcd.print("Pratos:");
    lcd.setCursor(0, 1);
    lcd.print(contador);
  }





  
}

void processkey() {
  char key = teclado_personalizado.getKey();
  if (isAlpha(key)) {   // check if key press is a letter
    processKeys(key);   // process it according to keys
  } 
}

void parseKey(int minValue, int maxValue, char keyPress) {
  int ch = minValue;
  char key = keyPress;
  if (keyPress == '*') {              // if *, means backspace
    if ( (x > 0) || (y > 0) ) {       // prevent backspace when no character yet
      x = x - 1;                      // go back to previous character position
      lcd.setCursor(x,y);             // set the new lcd position
      lcd.print("*");                 // write *, which means for editing
      msg.remove(msg.length() - 1);   // remove the last character from the string
    }
  } else {
    for (int i = 0; i < keyPressTime; i++) {
      if (key == keyPress) {          // make sure that same key is press
        lcd.setCursor(x, y);          // set the lcd position
        lcd.print(alpha[ch]);         // print the character according to the character position
        ch++;                         // increment character position
        if (ch > maxValue) {          // if the character counter reached the max value
          ch = minValue;              // reset to min value
          i = 0;                      // reset the loop counter
        }
      }
      key = teclado_personalizado.getKey();  // get the keypress
      delay(10);                    // delay for some time
    }
    x++;                    // increment the x position
    msg += alpha[ch - 1];   // add the character to the variable msg
    if (x > 15) {           // if the lcd reached the rightmost position
      y = 1;                // then wrap to the next line
      x = 0;                // in first character in the left              
    }
  }
}

void enterMSG() {
  char key;       
  lcd.clear();  // clear the LCD display
  x = 0;        // init the x position to zero
  y = 0;        // init the y position to zero
  msg = "";     // clear the msg variable
  
  do {
    key = teclado_personalizado.getKey();
    if        (key == '1') {    // if a key is pressed, 
      parseKey(0, 7, key);        // compare it to the alpha string array
    } else if (key == '2') {
      parseKey(8, 12, key);
    } else if (key == '3') {
      parseKey(13, 17, key);
    } else if (key == '4') {
      parseKey(18, 22, key);
    } else if (key == '5') {
      parseKey(23, 27, key);
    } else if (key == '6') {
      parseKey(28, 32, key);
    } else if (key == '7') {
      parseKey(33, 38, key);
    } else if (key == '8') {
      parseKey(39, 43, key);
    } else if (key == '9') {
      parseKey(44, 49, key);
    } else if (key == '0') {
      parseKey(52, 54, key);
    } else if (key == '*') {
      parseKey(50, 51, key);
    } else if (key == '#') {
      // do nothing
    }
  } while (key != 'D');       // exit the loop when # is pressed
  
  lcd.setCursor(0, 0);        // these are for verification only
  lcd.print("created msg");   // feel free to modify it and
  lcd.setCursor(0, 1);        // adapt to your specific requirements
  lcd.print(msg);
  delay(2000);
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

void sendSMS() {            // this is for demonstration only
                            // you need to modify this and adapt
                            // to your specific application
  lcd.clear();
  lcd.print("Sending SMS");
  delay(500);
  lcd.clear();
  lcd.print("Done Sending");
  delay(500);
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
        case 'A': // Up
          break;
        case 'B': // Down
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
    case 1:                     // Level 1, main menu
      switch ( keyPressed ) {
        case 'D':               // Enter
          sub = 1;
          menuLevel = 2;        // go to sub menu
          updateLevel_2();      // show sub menu
          delay(DEFAULT_DELAY);
          break;
        case 'A':               // Up
          menu--;
          delay(DEFAULT_DELAY);
          break;
        case 'B':               // Down
          menu++;
          delay(DEFAULT_DELAY);
          break;
        case 'C': // Back
          menuLevel = 0;        // hide menu, go back to level 0
          updateLevel_0();      // show home screen
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
          } else if (sub == 2) {  // Send SMS
            sendSMS();
            menuLevel = 2;        // go to sub menu
            updateLevel_2();      // show sub menu
          } else if (sub == 3) {  // Read SMS
            menuLevel = 3;        // go to sub menu of sub menu
            updateLevel_3();      // show sub menu of sub menu
          } else if (sub == 4) {  // Delete SMS
            executeAction();
            delay(1000);
            menuLevel = 2;        // go to sub menu
            updateLevel_2();      // show sub menu
          }
          delay(DEFAULT_DELAY);
          break;
        case 'A':               // Up
          sub--;
          updateLevel_2();
          delay(DEFAULT_DELAY);
          break;
        case 'B':               // Down
          sub++;
          updateLevel_2();      // show main menu
          delay(DEFAULT_DELAY);
          break;
        case 'C':               // Back
          menuLevel = 1;        // go back to level 1
          updateLevel_1();      // show main menu
          delay(DEFAULT_DELAY);
          break;
        default:
          break;
      }
      break;
    case 3:                     // Level 3, sub menu of sub menu
      switch ( keyPressed ) {
        case 'D':               // Enter
          //menuLevel = 2;        // go back to level 2
          //updateLevel_2();      // show sub menu
          delay(DEFAULT_DELAY);
          break;
        case 'A':               // Up
          updateLevel_3();      // show sub menu
          delay(DEFAULT_DELAY);
          break;
        case 'B':               // Down
          updateLevel_3();      // show sub menu
          delay(DEFAULT_DELAY);
          break;
        case 'C':               // Back
          menuLevel = 2;        // go back to main menu
          updateLevel_2();      // show main menu
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

void updateLevel_0() {
  lcd.clear();
  lcd.println("  PASSE CARTAO  ");
  lcd.setCursor(0, 1);
  lcd.println("ou pressione 'D'");
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
