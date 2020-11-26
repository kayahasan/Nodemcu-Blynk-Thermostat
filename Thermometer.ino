#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
char auth[] = "------";
char ssid[] = "------";
char pass[] = "------";

#define DHTPIN 0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
WidgetBridge bridge1(V1);


int KombiDurum = 0;
int IstenenSicaklik = 0;
float MaxIstenen = 0;
float MinIstenen = 0;
float h = 0;
float t = 0;
int om = 0;
int omonoff = 0;

BLYNK_CONNECTED() {
  Blynk.syncAll();
  bridge1.setAuthToken("IJE8QhmLVNQj1hg6bED4uxinWpp63qm2");
}

BLYNK_WRITE(V10)
{
  IstenenSicaklik = param.asInt();
  MaxIstenen = IstenenSicaklik + 0.5;
  MinIstenen = IstenenSicaklik - 0.5;
}

BLYNK_WRITE(V1) //Canlı kombi durumu
{
  KombiDurum = param.asInt();
}

BLYNK_WRITE(V3) //Otomatik or Manuel
{
  om = param.asInt();
}

BLYNK_WRITE(V4) //V3 to Açık Kapalı
{
  omonoff = param.asInt();
}

void sendSensor()
{
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}

void SetTemp()
{
  if(om == 1)
  {
    if(KombiDurum == 1)
    {
      if(t >= MaxIstenen)
      {
        bridge1.digitalWrite(D1,HIGH);
        Serial.println("Kombi Çalışıyodu, baktım ev çok ısınıyo Kapattım");
      }else
      {
        bridge1.digitalWrite(D1,LOW);
        Serial.println("Kombi Çalışıyodu, baktım ev hala soğuk bırak çalışsın mk");
      }
    }else
    {
      if(t <= MinIstenen)
      {
        bridge1.digitalWrite(D1,LOW);
        Serial.println("Kombi Kapalıydı, baktım ev soğuk aç dedim amk");
      }else
      {
        bridge1.digitalWrite(D1,HIGH);
        Serial.println("Kombi kapalıydı, baktım çokta sıkıntı yok kapalı kalsın didim");
      }
    }
  }
  else
  {
    if(omonoff == 1)
    {
      bridge1.digitalWrite(D1, LOW);
      Serial.println("Manuel Aç Didiler");
    }else
    {
      bridge1.digitalWrite(D1, HIGH);
      Serial.println("Manuel Kapat Didiler");
    }
  }
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, SetTemp);
}
 
void loop()
{
  Blynk.run();
  timer.run();
}
