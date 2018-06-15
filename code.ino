// This #include statement was automatically added by the Particle IDE.
#include <LiquidCrystal_I2C_Spark.h>

// This #include statement was automatically added by the Particle IDE.
#include <MFRC522.h>

#define SS_PIN A2
#define RST_PIN D2

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
LiquidCrystal_I2C *lcd;

// YOUR RFID chip data
char RUID[] = "000000";

int relay = D3;

//color pin
int green = D5;
int red = D6;

int buzz = D4;
int mag = D7;


void setup()
{
    mfrc522.setSPIConfig();
    mfrc522.PCD_Init(); // Init MFRC522 card
    
    
    // Liquidcrystal lcd
    lcd = new LiquidCrystal_I2C(0x27, 16, 2);
    lcd->init();
    lcd->backlight(); // turn on backlight
    lcd->clear();
    
    //Output pins
    pinMode(relay, OUTPUT); 
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT); 
    pinMode(buzz, OUTPUT);
    pinMode(mag, INPUT);
    
    

}

void loop()
{
    enableMagnet(); // eneable magnet sensor
    getTime(); // get current date and time
    Rfid();
    
  
}


void Rfid(){
    digitalWrite(relay, HIGH); // HIGH will be off on Relay since i use NO
     // Look for new cards
  if ( mfrc522.PICC_IsNewCardPresent()) {
    // Serial.println("New card present...");
    if ( mfrc522.PICC_ReadCardSerial()) {
      // Dump debug info about the card. PICC_HaltA() is automatically called.
      //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

      String UID = "";
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        UID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        UID += String(mfrc522.uid.uidByte[i], HEX);
      }
      
      //checks if UID == RUID then open up door
      //with 5s delay on lock and 15 sec delay until alarm turn on
      if(UID == RUID){
        digitalWrite(relay,LOW);
        Particle.publish("my-events", "Access granted", PRIVATE);
        messageGranted();
        digitalWrite(relay,HIGH);
        disableMagnet();
      } 
      else{
        Particle.publish("my-events", "Access denied", PRIVATE);
        messageDenied();
      }

          mfrc522.PICC_HaltA();
          Serial.print("UID: ");
          Serial.println(UID);
          Particle.publish("rfid-read", UID, 5, PRIVATE); // publish UID to rest of system
        }

    }
}

// get time to post on lcd
void getTime(){
    Particle.syncTime();
    waitUntil(Particle.syncTimeDone);
    Time.zone(+1);
    lcd->setCursor(1,0);
    lcd->print(Time.format(Time.now(), "%D %H:%M"));
}

// LCD post message if denied access by rfid
void messageGranted(){
        digitalWrite(relay,LOW);
        lcd->setCursor(0,1);
        lcd->print(" Access granted!");
        digitalWrite(green, HIGH);
        delay(5000);
        digitalWrite(green, LOW);
        lcd->clear();
}

// LCD post message if access granted by rfid
void messageDenied(){
        lcd->setCursor(0,1);
        lcd->print(" Access denied!");
        digitalWrite(red, HIGH);
        delay(4000);
        digitalWrite(red, LOW);
        lcd->clear();
    
}
//eneable magnet sensor 
void enableMagnet(){
    if(digitalRead(mag)== HIGH){
        digitalWrite(buzz, HIGH);
    }
    else{
        digitalWrite(buzz, LOW);
    }
}
//Disable magnet sensor and msg to lcd how long it will be off
void disableMagnet(){
    digitalWrite(buzz, LOW);
    getTime();
    lcd->setCursor(0,1);
    lcd->print("10 Sec alarm on!");
    delay(10000);
    lcd->clear();
}
