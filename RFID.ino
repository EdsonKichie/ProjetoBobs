#if 0
#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"
PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);
#elif 1
#include <PN532_HSU.h>
#include <PN532.h>
PN532_HSU pn532hsu(Serial1);
PN532 nfc(pn532hsu);
#else
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);
#endif
String tagId = "None", dispTag = "None";
byte nuidPICC[4];
void setup(void){
  Serial.begin(9600);
  Serial.println("* Teste Modulo PN532 NFC RFID *");
  nfc.begin();
  //Verifica a conexao do modulo PN532
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata)
  {
    Serial.print("Placa PN53x nao encontrada...");
    while (1); // halt
  }
  //Conexao ok, mostra informacoes do firmware
  Serial.print("Encontrado chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware versao: "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  // configure board to read RFID tags
  nfc.SAMConfig();
  Serial.println("Aguardando cartao ISO14443A...");
  Serial.println("");
}
void loop()
{
  readNFC();
}
String tags[] = {"165.252.132.1","83.63.136.149"};
void readNFC(){
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
    for(int i = 0; i <2; i++){
       if(tagId.equals(tags[i])){
           ok = true;
           digitalWrite(7,1);
           delay(200);
           digitalWrite(7,0);
       }
    }

   if(ok){
    Serial.print(F("Tag: "));
    Serial.println(tagId);
    Serial.print(F(": Autorizado"));
    ok = false;
   }else{
    Serial.print(F("Tag: "));
    Serial.println(tagId);
    Serial.print(F(": Não Autorizado"));
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
