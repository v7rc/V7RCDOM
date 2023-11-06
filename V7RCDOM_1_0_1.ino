#include<Servo.h>

// Last updated: 2021-05-08
// 1. 增加Failsafe的功能機制，當失去控制時，回到1500
// 2. 原有坦克模式 

#define numOfServ 6

int servo_pinArray[] = {3,5,6,9,10,11};
Servo servo_channel[numOfServ];

#define SERVO_DEFAULT_VALUE 1500
#define SERVO_DEFAULT_MAX_VALUE 2000
#define SERVO_DEFAULT_MIN_VALUE 1000
#define PACKET_LENGTH 20

#define SERIAL_BAUDRATE 57600
#define DEFAULT_PERIOD_TIME 30
#define LOST_SIGNAL_MAX_TIME 500 // 最大失去信號時間;
#define DEFAULT_LOOP_TIMES 33

const int led = 7;      //舊板子是A0
const int failSaveLED = 8; //舊板子是A1

long startProcessTime = 0;
long endProcessTime = 0;
int currentLostSignalTime = 0;

void setup() {
  
  Serial.begin(SERIAL_BAUDRATE);
  // Serial.println("V7RCDOM START!!");

  //initial Servo attach pin
  for(int i = 0;i< numOfServ;i++){

    // 起始化Servo,並且寫入預設值;    
    initServoPin(&servo_channel[i], servo_pinArray[i]);
    servo_channel[i].write(SERVO_DEFAULT_VALUE);
  
  }

  // Setup LED Port;
  pinMode(led, OUTPUT);
  pinMode(failSaveLED, OUTPUT);

  digitalWrite(failSaveLED, HIGH);

}

char readBuffer[PACKET_LENGTH];
int bufferIndex = 0;
long loopStart, loopEnd;
static boolean ledStatus = LOW;
static int loopProcessCheck = 0;
static int failSafeCheckTime = 0;   // 檢查是否有收到資料;
static bool ifLostSignal = true;

void loop() {

  loopStart = millis();
  

  if (loopProcessCheck > DEFAULT_LOOP_TIMES) {

    // Serial.println("init loopProcessCheck:" + String(loopProcessCheck));
    // Serial.println("loopProcessCheck:" + String(loopProcessCheck));

    if (ledStatus == LOW) {

      ledStatus = HIGH;

    } else {

      ledStatus = LOW;

    }

    digitalWrite(led, ledStatus);
    loopProcessCheck = 0;

  }
  
  int bufferLength = Serial.available();

  while(bufferLength > 0){

    char tempBuffer[bufferLength];
    Serial.readBytes(tempBuffer, bufferLength);
    
    Serial.print("getValue:");
    Serial.println(bufferLength);

    for(int i = 0; i < bufferLength; i ++) {
      
      readBuffer[bufferIndex] = tempBuffer[i];
      bufferIndex ++;

      //Data length = 20
      if(bufferIndex >= PACKET_LENGTH) {

        //convert char[] to String
        if(readBuffer[19]=='#'){
          
          String command = String(readBuffer);
          //Command Process
          V7RCCommand(command);
          
          Serial.println("processCommand");
          ifLostSignal = false;
          failSafeCheckTime = 0;  
          digitalWrite(failSaveLED, HIGH);  
        
        }else{
        
          Serial.print("invaild Value :");
          Serial.println(readBuffer[19]);
        
        }
        bufferIndex = 0;
      }
      if(i == bufferLength - 1){
        bufferIndex = 0;
      }
    }
    
    bufferLength = Serial.available();

  }

  loopEnd = millis();

  //if delay time less then 30 microsecond
  int needDelay = loopEnd - loopStart;

//  if(ifLostSignal == true) {
//
//    //當沒有收到資料時：
//    currentLostSignalTime = currentLostSignalTime + needDelay;
//
//    if (currentLostSignalTime >= LOST_SIGNAL_MAX_TIME) {
//
//      // 進入FailSaveMode;
//      processFailSave();
//      currentLostSignalTime = LOST_SIGNAL_MAX_TIME;
//      digitalWrite(failSaveLED, LOW);
//
//    } else {
//
//      digitalWrite(failSaveLED, HIGH);
//
//    }
//    
//  } else {
//
//    currentLostSignalTime = 0;
//    digitalWrite(failSaveLED, HIGH);
//    
//  }
  
  needDelay = DEFAULT_PERIOD_TIME - needDelay;
  failSafeCheckTime += DEFAULT_PERIOD_TIME;

  // Serial.print("failSafeCheckTime:");
  // Serial.println(failSafeCheckTime);

  if(failSafeCheckTime > LOST_SIGNAL_MAX_TIME) {

      processFailSave();
      failSafeCheckTime = LOST_SIGNAL_MAX_TIME;
      digitalWrite(failSaveLED, LOW);
      ifLostSignal = true;

      
    
  } 
  
  if(needDelay > 0) {
    
    delay(needDelay);
    
  }

  loopProcessCheck +=1;

  
  
}

//initial Servo attach pin
void initServoPin(Servo *servo, int pinNo){
  servo->attach(pinNo);
}


void V7RCCommand(String bufferString){

  // Serial.println(bufferString);
  
  int P0Value = 1500;
  int P1Value = 1500;
  int P2Value = 1500;
  int P3Value = 1500;
  int P4Value = 1500;
  int P5Value = 1500;
  
  if(bufferString.substring(0,3) == "SRV"){
    
    P0Value = (bufferString.substring(3,7)).toInt();  
    P1Value = (bufferString.substring(7,11)).toInt();
    P2Value = (bufferString.substring(11,15)).toInt();
    P3Value = (bufferString.substring(15,19)).toInt();

    servo_channel[0].write(P0Value);
    servo_channel[1].write(P1Value);
    servo_channel[2].write(P2Value);
    servo_channel[3].write(P3Value);
    
  }else if(bufferString.substring(0,3) == "SS8"){
    
//    int stringIndex = 3;
//    char buf[2];
//    
//    for(int i = 0;i<6;i++){
//      buf[0] = bufferString.charAt(stringIndex);
//      buf[1] = bufferString.charAt(stringIndex + 1);
//      
//      if(i == 0){
//        P0Value = convertHexStringToInt(buf);
//      }else if(i == 1){
//        P1Value = convertHexStringToInt(buf);
//      }else if(i == 2){
//        P2Value = convertHexStringToInt(buf);
//      }else if(i == 3){
//        P3Value = convertHexStringToInt(buf);
//      }else if(i == 4){
//        P4Value = convertHexStringToInt(buf);
//      }else if(i == 5){
//        P5Value = convertHexStringToInt(buf);
//      }
//
//      stringIndex = stringIndex + 2;
//    }
//
//    servo_channel[0].write((P0Value)*10);
//    servo_channel[1].write((P1Value)*10);
//    servo_channel[2].write((P2Value)*10);
//    servo_channel[3].write((P3Value)*10);
//    servo_channel[4].write((P4Value)*10);
//    servo_channel[5].write((P5Value)*10);
    
    
    P0Value = (bufferString.substring(3,5)).toInt();
    P1Value = (bufferString.substring(5,7)).toInt();
    P2Value = (bufferString.substring(7,9)).toInt();
    P3Value = (bufferString.substring(9,11)).toInt();
    P4Value = (bufferString.substring(11,13)).toInt();
    P5Value = (bufferString.substring(13,15)).toInt();
    
    servo_channel[0].write(mapValue(P0Value));
    servo_channel[1].write(mapValue(P1Value));
    servo_channel[2].write(mapValue(P2Value));
    servo_channel[3].write(mapValue(P3Value));
    servo_channel[4].write(mapValue(P4Value));
    servo_channel[5].write(mapValue(P5Value));

//    Serial.print("Servo0:");
//    Serial.println(mapValue(P0Value));
//    Serial.print("Servo1:");
//    Serial.println(mapValue(P1Value));
//    Serial.print("Servo2:");
//    Serial.println(mapValue(P2Value));
//    Serial.print("Servo3:");
//    Serial.println(mapValue(P3Value));
//    Serial.print("Servo4:");
//    Serial.println(mapValue(P4Value));
//    Serial.print("Servo5:");
//    Serial.println(mapValue(P5Value));
    
  }else if(bufferString.substring(0,3) == "SRT"){
    
    int Tank0 = 1500;
    int Tank1 = 1500;
    
    P0Value = (bufferString.substring(3,7)).toInt();
    P1Value = (bufferString.substring(7,11)).toInt();
    P2Value = (bufferString.substring(11,15)).toInt();
    P3Value = (bufferString.substring(15,19)).toInt();

    if (P1Value >= 1500) {
        Tank1 = Tank1 + (P1Value - 1500);
        Tank0 = Tank0 - (P1Value - 1500);
    } else {
        Tank1 = Tank1 - (1500 - P1Value);
        Tank0 = Tank0 + (1500 - P1Value);
    }
    if (P0Value >= 1500) {
       Tank1 = Tank1 + (P0Value - 1500);
       Tank0 = Tank0 + (P0Value - 1500);
    } else {
        Tank1 = Tank1 - (1500 - P0Value);
        Tank0 = Tank0 - (1500 - P0Value);
    }
    if (Tank0 < SERVO_DEFAULT_MIN_VALUE) {
        Tank0 = SERVO_DEFAULT_MIN_VALUE;
    }
    if (Tank0 > SERVO_DEFAULT_MAX_VALUE) {
        Tank0 = SERVO_DEFAULT_MAX_VALUE;
    }
    if (Tank1 > SERVO_DEFAULT_MAX_VALUE) {
        Tank1 = SERVO_DEFAULT_MAX_VALUE;
    }
    if (Tank1 < SERVO_DEFAULT_MIN_VALUE) {
        Tank1 = SERVO_DEFAULT_MIN_VALUE;
    }

    servo_channel[0].write(Tank0);
    servo_channel[1].write(Tank1);
    servo_channel[2].write(P2Value);
    servo_channel[3].write(P3Value);

  }else{
    // Serial.write("ERROR");
  }
  
}

//convert PWM Value
int mapValue(int Value){
  int mapvalue = 0;
  mapvalue = map(Value,0,99,544,2400);
  return mapvalue;
}


// 沒有收到數據時，要回到預設值;
void processFailSave() {

  for(int i = 0;i< numOfServ;i++){

    // 起始化Servo,並且寫入預設值;    
    servo_channel[i].write(SERVO_DEFAULT_VALUE);
  
  }
  
}

  

//int convertHexStringToInt(char in[]){
//  int tens;
//  int digits;
//    
//  if (!isxdigit(in[0]) || !isxdigit(in[1]))   // Valid hex digit character?
//    return -1;
//
//  in[0] = toupper(in[0]);   // Use upper case
//  in[1] = toupper(in[1]);
//  
//  tens = in[0] >= 'A' ? (in[0] - 'A' + 10) : in[0] - '0';
//  digits = in[1] >= 'A' ? (in[1] - 'A' + 10) : in[1] - '0';
//  return tens * 16 + digits;
//}
