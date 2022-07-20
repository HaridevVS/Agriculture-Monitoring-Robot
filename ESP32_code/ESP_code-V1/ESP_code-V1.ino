#define ENA 25
#define IN1 26
#define IN2 27
#define IN3 14
#define IN4 12
#define ENB 13

bool forward = 0;
bool backward = 0;
bool left = 0;
bool right = 0;
int Speed=0;
int power=0;

#include "WiFi.h"
#include <BH1750.h>
BH1750 lightMeter;
#include <Wire.h>
#include <SPI.h>
#include <ESP32Servo.h>
#include <analogWrite.h>
#include "DHT.h"
#define DHTPIN 23
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define BLYNK_TEMPLATE_ID "TMPLDaV3cZgL"
#define BLYNK_DEVICE_NAME "Agriculture Monitoring Robot"
#define BLYNK_AUTH_TOKEN "AGGuivP3VkoXP7UJk0oQTCV7eglHgUTk"

const int AirValue = 790;   
const int WaterValue = 390;  
const int SensorPin = 34;
int soilMoistureValue;
int soilmoisturepercent=0;

Servo servo1;
Servo servo2;
int pos;
int servo1Pin = 4;
int servo2Pin = 32;

int arm=0;

#define BLYNK_PRINT Serial

#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

BlynkTimer timer;
 
const char* ssid = "Realme";
const char* pass =  "qwerty123";
int pump,fan;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Blynk.virtualWrite(V3, h); 
  Blynk.virtualWrite(V2, t);
}
void sendSoil(){
  soilMoistureValue = analogRead(SensorPin);
  soilmoisturepercent = constrain ( map(soilMoistureValue, 1000, 4095, 100, 0), 0, 100);
  Blynk.virtualWrite(V4,soilmoisturepercent);
  }
void sendUptime(){
  
  uint16_t lux = lightMeter.readLightLevel();
  Blynk.virtualWrite(V1, lux);
}
BLYNK_WRITE(V12) {
  Speed = param.asInt();
  Serial.println(Speed);
}
BLYNK_WRITE(V6)
{
  int pump = param.asInt();
  Serial.println(pump);
} 
BLYNK_WRITE(V7)
{
  int fan = param.asInt();
  Serial.println(fan);
} 
BLYNK_WRITE(V0) {
  power = param.asInt();
}
void setup() {
 
  Serial.begin(9800);
  pinMode(SensorPin,INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to "); Serial.println(ssid);
 
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
 
    if ((++i % 16) == 0)
    {
      Serial.println(F(" still trying to connect"));
    }
  }
 
  Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, sendUptime);
  Wire.begin();

  lightMeter.begin();
  dht.begin();

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo1.setPeriodHertz(50); 
  servo1.attach(servo1Pin);
  servo2.setPeriodHertz(50); 
  servo2.attach(servo2Pin);  
 
}
BLYNK_WRITE(V8) {
  forward = param.asInt();
}
 
BLYNK_WRITE(V9) {
  backward = param.asInt();
}
 
BLYNK_WRITE(V10) {
  left = param.asInt();
}
 
BLYNK_WRITE(V11) {
  right = param.asInt();
}

BLYNK_WRITE(V14) {
  pos = param.asInt();
  int i;
  for(i=0;i<=pos;i+=3)
  {
    servo1.write(pos);    // tell servo to go to position in variable 'pos'
    delay(20);
    }
}

BLYNK_WRITE(V13) {
  arm = param.asInt();
  if(arm==1)
  {
  int i;
  servo2.write(0);
  for(i=0;i<=180;i+=3)
  {
    servo2.write(i);    // tell servo to go to position in variable 'pos'
    delay(80);
    }
    delay(2000);
    sendSoil();
    delay(2000);
  for(i=180;i>0;i-=3)
  {
    servo2.write(i);
    delay(80);
  }  
  servo2.write(0); 
  } 
  
}
 

 
void smartcar() {
  if (forward == 1) {
    carforward();
    Serial.println("carforward");
  } else if (backward == 1) {
    carbackward();
    Serial.println("carbackward");
  } else if (left == 1) {
    carturnleft();
    Serial.println("carfleft");
  } else if (right == 1) {
    carturnright();
    Serial.println("carright");
  } else if (forward == 0 && backward == 0 && left == 0 && right == 0) {
    carStop();
    Serial.println("carstop");
  }
}
void carforward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carbackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carturnleft() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carturnright() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
 
void loop() {
  
  Blynk.run();
  if(power==1){
   timer.run();
  }
  if(power==1){
  smartcar();
  }
  delay(10); 
  }
