#define SERIAL_BAUDRATE 57600
// 參考網址： https://lingshunlab.com/book/arduino/arduino-uno-use-water-level-sensor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_BAUDRATE);
}

void loop() {
  // put your main code here, to run repeatedly:

  int val=analogRead(A1);

  Serial.print("val = "); // 串口输出 val 当前的数据
  Serial.print(val);

  if(val < 10) { // 如果 val 小于 10 
    Serial.println(" | dry"); // 说明很干，没有水
  } else if(val < 300) { // 如果 小于 300
    Serial.println(" | water level: ~ 0-1 cm"); // 水位大约在0-1cm ，以下语句如此类推
  } else if(val < 345) {
    Serial.println(" | water level: ~ 1-2 cm");
  } else if(val < 365) {
    Serial.println(" | water level: ~ 2-3 cm");
  } else if(val < 385) {
    Serial.println(" | water level: ~ 3-4 cm");
  } else {
    Serial.println(" | water level: over 4 cm");
  }
  delay(1000); // 等待1秒


}
