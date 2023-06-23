#include <SPI.h> 

#include <MFRC522.h> 


int RST_PIN = 9; 
int SS_PIN = 10; 
bool hareket = false; 
bool aktif = false;
float bekleme1 = 10; 
int bekleme2 = 10; 
float sayac = bekleme2; 

MFRC522 rfid(SS_PIN, RST_PIN); 
byte ID[4] = { 153, 81, 19, 163  };

 

void setup() {

  Serial.begin(9600); 
  SPI.begin(); 
  rfid.PCD_Init(); 
  pinMode(8, INPUT); 
  pinMode(7, OUTPUT); 
  pinMode(6, INPUT); 
}

void loop() {
  if (digitalRead(6) == HIGH) { //Butona basıldığında
    digitalWrite(7, HIGH);
    delay(100);
    digitalWrite(7, LOW); 
    delay(bekleme1 * 1000); 
    aktif = true; 
  }
  if (digitalRead(8) == HIGH && aktif == true) { //Hareket algılanırsa ve alarm aktifse
    digitalWrite(7, HIGH);
    delay(100);
    digitalWrite(7, LOW);
    hareket = true; 
    while (hareket) { 
      if (sayac <= 0) {
        digitalWrite(7, HIGH); //Süre biterse buzzerın ötmeisni sağlıyoruz.
      } else { //Süre bitmediyse
        delay(100);  
        sayac -= 0.1; 
      }
      Serial.println(sayac);
      if (rfid.PICC_IsNewCardPresent()) { //Okuyucuya bir kart yaklaştrılmışsa

        if (rfid.PICC_ReadCardSerial()) { //Kartın ID'si okunuyorsa

          if (rfid.uid.uidByte[0] == ID[0] &&
            rfid.uid.uidByte[1] == ID[1] &&
            rfid.uid.uidByte[2] == ID[2] &&
            rfid.uid.uidByte[3] == ID[3]) { //Kartın ID'sini yetkili ID ile karşılaştırıyoruz.
            hareket = false;
            aktif = false;
            digitalWrite(7, HIGH);
            delay(100);
            digitalWrite(7, LOW); 

          }
        }
      }
      rfid.PICC_HaltA(); 
    }
    digitalWrite(7, LOW); 
    sayac = bekleme2; 
  }
}