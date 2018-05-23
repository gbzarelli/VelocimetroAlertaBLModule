#include <SoftwareSerial.h>

#define PIN_TX_BLE  10
#define PIN_RX_BLE  11
#define SENSOR_CADENCE  9
#define SENSOR_TEMP     A4

SoftwareSerial mySerial(PIN_TX_BLE, PIN_RX_BLE); // RX, TX
/**
 * Setup para Bluetooth LE - HM-10 - Comandos AT   
 * 
  mySerial.print("AT+VERSION");//Comando para verificar 'ok'
  delay(1000);
  mySerial.print("AT+BAUD");//Comando para verificar velocidade canal
  delay(1000);
  mySerial.print("AT+TYPE");//Comando para verificar velocidade canal
  delay(1000);
  mySerial.print("AT+NAMEVelAlert-BLE");
  delay(1000);
 *
 */
void setup() {
  pinMode(SENSOR_CADENCE,INPUT);
  pinMode(SENSOR_TEMP,INPUT);
  Serial.begin(9600);//Inicia Serial
  mySerial.begin(9600);//Inicia bluetooth
  delay(1000);
}

int count = 0;
float temp = 0;
long tmpTime = -1;
int rpm = 0;
boolean inRead = false;

void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  
  if(HIGH == digitalRead(SENSOR_CADENCE)){
    if(!inRead){
      inRead=true;
      if(tmpTime == -1){
        tmpTime = millis();
      }else{
        long timeLoop = millis() - tmpTime;
        tmpTime = millis();
        rpm = 60000 / timeLoop;
        if(rpm<0){ rpm=0; }
      }
    }
  }else if(inRead){
    inRead=false;
  }
  
  temp += (float(analogRead(SENSOR_TEMP))*3.3/(1023))/0.01;
  
  if(++count == 10){
    if(rpm>0){
      long timeLoop = millis() - tmpTime;
      if(timeLoop>0){
        int rpmx = 60000 / timeLoop;
        if(rpmx >= 0 && rpmx < rpm){
          rpm = rpmx;
        }
      }
    }
    String out = "{temp:"+String(temp/10)+",rpm:"+String(rpm)+"}";
    mySerial.println(out);
    Serial.println(out);
    count=0;
    temp=0;
  }
  
  delay(100);
}
