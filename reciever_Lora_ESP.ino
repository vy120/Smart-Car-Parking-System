#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6HLjv0Q_z"
#define BLYNK_TEMPLATE_NAME "Project"
#define BLYNK_AUTH_TOKEN "8E2cUPZ9LNaNkOzZdDTDoUFePhz6JzrJ"

#include <LoRa.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define LORA_SS_PIN 15
#define LORA_RST_PIN 2
#define LORA_DI0_PIN 5
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "The Grace 2";// your wifi name
char pass[] = "99999999";// passwifi
int v = 0;


void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  while (!Serial);


  LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DI0_PIN);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Check your connections!");
    while (1);
  }


  Serial.println("LoRa init successful.");
  LoRa.setTxPower(20);
  Lora.receive()
}

void loop() {
  Blynk.run();


  // Gửi dữ liệu từ ESP8266 đến module LoRa
  String dataToSend = String(v);
  LoRa.beginPacket();
  LoRa.print(dataToSend);
  LoRa.endPacket();
  Serial.println("Data sent from ESP8266: " + dataToSend);

  // nhận từ arduino
  if (LoRa.parsePacket()) {
    String receivedData = "";
    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    char delimiter[] = ", ";
    char* token = strtok((char*)receivedData.c_str(), delimiter);


    String data1 = token;
    token = strtok(NULL, delimiter);
    if (data1.toInt() == 1) {
      Blynk.virtualWrite(V0, 1);
    } else {
      Blynk.virtualWrite(V0, 0);
    }
    String data2 = token;
    token = strtok(NULL, delimiter);
    if (data1.toInt() == 1) {
      Blynk.virtualWrite(V1, 1);
    } else {
      Blynk.virtualWrite(V1, 0);
    }
  }
  delay(100);
}
