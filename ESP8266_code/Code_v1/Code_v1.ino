#define BLYNK_TEMPLATE_ID "TMPLDaV3cZgL"
#define BLYNK_DEVICE_NAME "Agriculture Monitoring Robot"
#define BLYNK_AUTH_TOKEN "AGGuivP3VkoXP7UJk0oQTCV7eglHgUTk"

int ENA = 4;
int IN1 = 3;
int IN2 = 1;
int speed1 =0;

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Realme";
char pass[] = "qwerty123";

BlynkTimer timer;
int pump=0;
int fan=0;

BLYNK_WRITE(V6)
{
  pump = param.asInt();
  Serial.println(pump);

} 
BLYNK_WRITE(V7)
{
  fan = param.asInt();
  Serial.println(fan);
}
void setup()
{
  Serial.begin(9600);
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
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENA,OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  delay(10);
}
void pumping()
{   
  if(pump>0)
     {
        if(pump==1){
          speed1=200;
          }
        else if(pump==2){
          speed1=210;}
          else if(pump==3){
            speed1=220;}
            else if(pump==4){
              speed1=230;}
              else{
                speed1=255;}  
        Serial.println(speed1);
        analogWrite(ENA, speed1);
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
  timer.run();
  Serial.println(pump);
  pumping();
  
}
