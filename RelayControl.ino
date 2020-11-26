#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "-----";
char ssid[] = "-----";
char pass[] = "-----";

BlynkTimer timer;
WidgetBridge bridge1(V3);

BLYNK_CONNECTED() {
  Blynk.syncAll();
  bridge1.setAuthToken("-----");
}

void RelayStatus()
{
  if(digitalRead(D1) == 0){
    Blynk.virtualWrite(V1, "Kombi AÇIK");
    Blynk.setProperty(V1, "color", "#00FF00");
    bridge1.virtualWrite(V1,1);
  }
  else if(digitalRead(D1) == 1){
    Blynk.virtualWrite(V1, "Kombi KAPALI");
    Blynk.setProperty(V1, "color", "#FF0000");
    bridge1.virtualWrite(V1,0);
  }
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(D1, OUTPUT);
  digitalWrite(D1, HIGH); //Başlangıçta Kapalı
  timer.setInterval(1000L, RelayStatus);
}

void loop()
{
  Blynk.run();
  timer.run();
}
