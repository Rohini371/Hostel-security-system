#include <SPI.h>
\
#include <MFRC522.h>
#include <LiquidCrystal.h>
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define SS_PIN 10
#define RST_PIN 9
int led_green = 0; 
int led_red = 1;
int buzzer = 8;

MFRC522 mfrc522(SS_PIN, RST_PIN); 

void setup() {

  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Scan Your Card"); 

  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(led_green, OUTPUT);
  pinMode(led_red, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.println("Place your card on reader...");
  Serial.println();
  
}

void loop() {

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();


  
 if (content.substring(1) == "F9 5A 56 15" || content.substring(1) == "F2 9D 86 52" || content.substring(1) == "F1 2E C0 4B") //change here the UID of card/cards or tag/tags that you want to give access
  {
    Serial.println("Access Granted");
    Serial.println("Door Unlocked");
    delay(500);
    lcd.clear();
    lcd.print("Success");
    delay(500);
    lcd.setCursor(0,1);
    lcd.print("Door UnLocked");
    digitalWrite(buzzer,LOW);
    digitalWrite(led_red,LOW);
    digitalWrite(led_green,HIGH);
    delay(2000);
    lcd.clear();
    lcd.print("Scan Your Card");
  }

else
{
    lcd.clear();
    lcd.print("Invalid Tag");
    delay(500);
    lcd.setCursor(0,1);
    lcd.print("Access denied");
    Serial.println(" Access denied");
    digitalWrite(buzzer,HIGH);
    digitalWrite(led_red,HIGH);
    digitalWrite(led_green,LOW);
    delay(2000);
    lcd.clear();
    lcd.print("Scan Your Card");
 }
}
