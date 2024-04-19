void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(A0, INPUT);

}


// 需要準備額外的電阻，否則很容易燒壞目前的板子
// 參考網址：https://blog.jmaker.com.tw/arduino-photoresistor/

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);

  if(sensorValue <900) {
    digitalWrite(7,HIGH); 
  } else {
    digitalWrite(7,LOW); 
  }

  if(sensorValue < 800) {
    digitalWrite(8,HIGH); 
  } else {
    digitalWrite(8,LOW); 
  }

  if(sensorValue < 700) {
    digitalWrite(12,HIGH); 
  } else {
    digitalWrite(12,LOW); 
  }

  delay(10);

}
