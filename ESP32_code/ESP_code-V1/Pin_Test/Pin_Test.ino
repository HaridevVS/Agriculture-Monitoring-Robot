int val;
void setup()
{
  Serial.begin(9600);
  pinMode(34,INPUT);
  
 }

 void loop(){
  val=analogRead(34);
  Serial.println(val);
  delay(1000);
  }
