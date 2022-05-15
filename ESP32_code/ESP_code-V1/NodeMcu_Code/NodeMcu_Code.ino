#define BLYNK_TEMPLATE_ID "TMPLDaV3cZgL"
#define BLYNK_DEVICE_NAME "Agriculture Monitoring Robot"
#define BLYNK_AUTH_TOKEN "AGGuivP3VkoXP7UJk0oQTCV7eglHgUTk"

#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

int ENA = 5;
int IN1 = 4;
int IN2 = 0;

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Realme";
char pass[] = "qwerty123";
int pump;
int fan;

BLYNK_WRITE(V6)
{
  pump = param.asInt();
} 
BLYNK_WRITE(V7)
{
  fan = param.asInt();
}

void setup()
{
  Serial.begin(9600);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENA,OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
       delay(10);
               
       Serial.println("Connecting to ");
       Serial.println(ssid); 
 
       WiFi.begin(ssid, pass); 
       while (WiFi.status() != WL_CONNECTED) 
          {
            delay(500);
            Serial.print(".");
          }
      Serial.println("");
      Serial.println("WiFi connected");

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}
void pumping()
{   
  if(pump>0)
     {
        Serial.println(pump);
        analogWrite(ENA, 1023);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
     } 
  else
    {
       digitalWrite(IN1, LOW);
       digitalWrite(IN2, LOW);
    }        
  }
void loop()
{
  Blynk.run();
  pumping();
  
}
