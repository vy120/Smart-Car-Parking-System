#define LORA_SS_PIN 10
#define LORA_RST_PIN 9
#define LORA_DI0_PIN 8

#include <SPI.h>
#include <LoRa.h>
#include <String.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //Change the HEX address
#include <Servo.h>

const unsigned int Tr1 = 7;
const unsigned int E1 = 6;
const unsigned int Tr2 = 1;
const unsigned int E2 = 0;
boolean a = false;
int s1 = 0;
int s2 = 0;
Servo myservo1;


int IR1 = 2; //cảm biến 1 nối với cổng 2 con truoc dây mát màu xanh
int IR2 = 4; //cảm biến 2 nối với cổng 4 vong con sau dây nóng màu đỏ

int Slot = 2;           //Enter Total number of parking Slots
int flag1 = 0;
int flag2 = 0;
void setup() {
  // bãi giữ xe
  lcd.init();
  lcd.backlight();
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(Tr1, OUTPUT);
  pinMode(E1, INPUT);
  pinMode(Tr2, OUTPUT);
  pinMode(E2, INPUT);
  myservo1.attach(3);
  myservo1.write(100);
  lcd.setCursor (0, 0);
  lcd.print("     ARDUINO    ");
  lcd.setCursor (0, 1);
  lcd.print(" PARKING SYSTEM ");
  delay (2000);
  lcd.clear();
  // hết bãi giữ
// Serial.begin(9600);
  //while (!Serial);
  LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DI0_PIN);
  if (!LoRa.begin(433E6)) {
    while (1);
  }
  LoRa.setTxPower(20);
}

void loop() {
  a = false;
  if (digitalRead (IR1) == LOW && flag1 == 0) { // LOW : nhận được cảm biết
    if (Slot > 0) {
      flag1 = 1;
      if (flag2 == 0) {
        myservo1.write(0); //open
        a = true;
       // Slot = Slot - 1;
      }

    } else {
      lcd.setCursor (0, 0);
      lcd.print("    SORRY :(    ");
      lcd.setCursor (0, 1);
      lcd.print("  Parking Full  ");
      delay (3000);
      lcd.clear();
    }
  }
  //fl1=1
  if (digitalRead (IR2) == LOW && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0) {
      myservo1.write(0); //open
      a = true;
     // Slot = Slot + 1;    
    }
  }

//  if (flag1 == 1 && flag2 == 1) {
//
//    delay (2000); // khoang thoi gian chờ xe đi qua
//
//    myservo1.write(100); //close
//    flag1 = 0, flag2 = 0;
//    if (digitalRead (IR2) == LOW) {
//      flag2 = 1;
//    }
//    if (digitalRead (IR1) == LOW) {
//      flag1 = 1;
//    }
//  }

  if (a == true) {
    delay (3000); // khoang thoi gian chờ xe đi qua

    myservo1.write(100); //close
    flag1 = 0, flag2 = 0;
    if (digitalRead (IR2) == LOW) {
      flag2 = 1;
    }
    if (digitalRead (IR1) == LOW) {
      flag1 = 1;
    }
  }

  if (digitalRead (IR2) == HIGH) {
    flag2 = 0;
  }

  if (digitalRead (IR1) == HIGH) {
    flag1 = 0;
  }

  lcd.setCursor (0, 0);
  lcd.print("    WELCOME!    ");
  lcd.setCursor (0, 1);
  lcd.print("Slot Left: ");
  lcd.print(Slot);
  String s = "Slot Left: " + String(Slot);

  digitalWrite(Tr1, LOW);
  delayMicroseconds(2);
  digitalWrite(Tr1, HIGH);
  delayMicroseconds(10);
  digitalWrite(Tr1, LOW);
  const unsigned long duration1 = pulseIn(E1, HIGH);
  int distance1 = duration1 / 29 / 2;
  delay(50);


  digitalWrite(Tr2, LOW);
  delayMicroseconds(2);
  digitalWrite(Tr2, HIGH);
  delayMicroseconds(10);
  digitalWrite(Tr2, LOW);
  const unsigned long duration2 = pulseIn(E2, HIGH);
  int distance2 = duration2 / 29 / 2;
  delay(50);
  
  if (distance1 <= 10 && s1 == 0) {
    s1 = 1;
    Slot = Slot - 1;
  } 
  if(distance1 > 10 && s1 == 1){
    s1 = 0;
    Slot = Slot + 1;
  }
  if (distance2 <= 10 && s2 == 0) {
    s2 = 1;
    Slot = Slot -1;
  }
  if(distance2 > 10 && s2 == 1) {
    s2 = 0 ;
    Slot = Slot +1;
  }
  String dataToSend = String(s1) + ", " + String(s2);
  LoRa.beginPacket();
  LoRa.print(dataToSend);
  LoRa.endPacket();
  delay(100);
}
