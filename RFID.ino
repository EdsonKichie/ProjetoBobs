#include <SoftwareSerial.h>
#include <PN532_SWHSU.h>
#include <PN532.h>
#include <LiquidCrystal.h>
#define BRILHO 10
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
SoftwareSerial SWSerial( 2, 3 ); // RX, TX
 
PN532_SWHSU pn532swhsu( SWSerial );
PN532 nfc( pn532swhsu );
String tagId = "None", dispTag = "None";
byte nuidPICC[4];
 
void setup(void){
  imprimir("Ligado","");
  Serial.begin(9600);
  pinMode(12,OUTPUT);
  pinMode(BRILHO,INPUT);
  lcd.begin(16, 2);
  lcd.clear();
  imprimir("Passe o ","cartao");     
  analogWrite(BRILHO,180);
  
  Serial.println("Seja Bem vindo!");
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata)
  {
    Serial.print("Didn't Find PN53x Module");
    while (1); // Halt
  }
  // Got valid data, print it out!
  Serial.print("Found chip PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); 
  Serial.println((versiondata >> 8) & 0xFF, DEC);
  // Configure board to read RFID tags
  nfc.SAMConfig();
  //Serial.println("Waiting for an ISO14443A Card ...");
}
 
 
void loop()
{

  readNFC();
}

String tags[] = {"105.168.152.13", "165.252.132.1"};
void readNFC()
{
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                       // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if (success)
  {
    //Serial.print("UID Length: ");
    //Serial.print(uidLength, DEC);
    //Serial.println(" bytes");
    //Serial.print("UID Value: ");
    for (uint8_t i = 0; i < uidLength; i++)
    {
      nuidPICC[i] = uid[i];
      //Serial.print(" "); Serial.print(uid[i], DEC);
    }
    Serial.println();
    tagId = tagToString(nuidPICC);
    dispTag = tagId;
    Serial.print(F("tagId is : "));
    Serial.println(tagId);
    bool ok = false;
    for(int i = 0; i < 2; i++){
       if(tagId.equals(tags[i])){
           ok = true;
       }
    }

   if(ok){
    Serial.print(F("Tag: "));
    Serial.println(tagId);
    Serial.print(F(": Autorizado"));
    imprimir("","LIBERADO");
    delay(2000);
    lcd.clear();
    delay(500);
    ok = false;
    digitalWrite(12,1);
   }else{
    Serial.print(F("Tag: "));
    Serial.println(tagId);
    Serial.print(F(": Não Autorizado"));
    imprimir("","TENTAR");
    delay(2000);
    lcd.clear();
    delay(500);
   }
    Serial.println("");
    delay(1000);  // 1 second halt
    digitalWrite(12,0);
  }
  else
  {
    // PN532 probably timed out waiting for a card
    Serial.println("Timed out! Waiting for a card...");
  }
}
String tagToString(byte id[4]){
  String tagId = "";
  for (byte i = 0; i < 4; i++)
  {
    if (i < 3) tagId += String(id[i]) + ".";
    else tagId += String(id[i]);
  }
  return tagId;
}
void imprimir(String texto1, String texto2){
  lcd.clear();                           
  lcd.setCursor(0, 0);                   
  lcd.print(texto1);         
  lcd.setCursor(0, 1);                   
  lcd.print(texto2);
}
