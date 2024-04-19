#define SERIAL_BAUDRATE 57600

int sensor = A2;  //訊號線
int LED = 7;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(SERIAL_BAUDRATE);
 pinMode(sensor, INPUT); 
 pinMode(LED, OUTPUT); 

}

void loop() {

  // put your main code here, to run repeatedly:
  int moving = digitalRead(sensor); //讀取D9是否有偵測到物體移動
  if(moving==1){ //如果有物體移動
    Serial.println("有東西在動！");  
    digitalWrite(LED, HIGH);  
    
  }

}
