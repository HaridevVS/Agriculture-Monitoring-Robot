#define ENA 25
#define IN1 26
#define IN2 27
#define IN3 14
#define IN4 12
#define ENB 13

const int freq = 3000;
const int pwm1 = 0;
const int pwm2 = 1;
const int resolution = 8;

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
const int SensorPin = 5;
int soilMoistureValue;
int soilmoisturepercent=0;

Servo servo1;
Servo servo2;
int pos = 0;
int arm =0;
int servo1Pin = 4;
int servo2Pin =32;

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
  Serial.println(soilMoistureValue);
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
  timer.setInterval(1000L, sendSoil);
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, sendUptime);
  Wire.begin();

  lightMeter.begin();
  dht.begin();

  ledcAttachPin(ENA, pwm1);
  ledcSetup(pwm1, freq, resolution);
  ledcAttachPin(ENB, pwm2);
  ledcSetup(pwm2, freq, resolution);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo1.setPeriodHertz(50);    // standard 50 hz servo
  servo1.attach(servo1Pin, 500, 2400);
  servo2.setPeriodHertz(50);    // standard 50 hz servo
  servo2.attach(servo2Pin, 500, 2400);
  
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
    delay(10);
    }
}
BLYNK_WRITE(V13) {
  arm = param.asInt();
  if(arm==1)
  {
    Serial.println(arm);
  int j;
  for(j=0;j<=180;j+=3)
  {
    servo2.write(j);    // tell servo to go to position in variable 'pos'
    delay(50);
    }    
  for(j=180;j>0;j-=3)
  {
    servo2.write(j);    // tell servo to go to position in variable 'pos'
    delay(50);
    }
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
  ledcWrite(pwm1, Speed);
  ledcWrite(pwm2, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carbackward() {
  ledcWrite(pwm1, Speed);
  ledcWrite(pwm2, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carturnleft() {
  ledcWrite(pwm1, Speed);
  ledcWrite(pwm2, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

}
void carturnright() {
  ledcWrite(pwm1, Speed);
  ledcWrite(pwm2, Speed);
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
  delay(1000); 
  }
