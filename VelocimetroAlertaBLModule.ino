#include <SoftwareSerial.h>
#include "DHT.h"

#define PIN_TX_BLE      11
#define PIN_RX_BLE      10
#define SENSOR_CADENCE  12
#define DHT_PIN         A3

#define COD_SEND          "1"
#define COD_ENABLE_AUTO   "2"
#define COD_DISABLE_AUTO  "3"
#define COD_READ_AUTO     "4"

void writeData();
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
SoftwareSerial mySerial(PIN_TX_BLE, PIN_RX_BLE); // RX, TX
DHT dht;

int temperature = -1;
int humidity = -1;
int err=0;
long tmpTime = -1;
int rpm = 0;
boolean inRead = false;
long mi;
long sec;
String data="{null}";
boolean automaticSendData=false;

void setup() {
  pinMode(SENSOR_CADENCE,INPUT);
  mySerial.begin(9600);//Inicia bluetooth
  delay(1000);
  dht.setup(DHT_PIN);
  mi = millis();
}

boolean markToRead=false;

void loop() {
  if(markToRead){
    if(mySerial.available()>0){
      String valueRead = mySerial.readString();
      if(valueRead.equals(COD_SEND)){
        mySerial.println(data);
      }else if(valueRead.equals(COD_ENABLE_AUTO)){
        automaticSendData=true;
      }else if(valueRead.equals(COD_DISABLE_AUTO)){
        automaticSendData=false;
      }else if(valueRead.equals(COD_READ_AUTO)){
        mySerial.println(automaticSendData);
      }
    }
  }
  markToRead=false;
  //Se leu o sensor (pullUP)
  if(LOW == digitalRead(SENSOR_CADENCE)){
    if(!inRead){
      inRead=true;
      if(tmpTime == -1){
        tmpTime = millis();
      }else{
        long timeLoop = millis() - tmpTime;
        tmpTime = millis();
        rpm = 60000 / timeLoop;
        if(rpm<0){ rpm=0; }
        writeData();
      }
    }
  }else if(inRead){//Sensor sem leitura apos uma leitura
    inRead=false;
  }else if( (millis()-mi) >= 2000 ){//Se passou 2seg sem leitura! (abaixo de 30rpm) - calculo automatico
      if(rpm>0){
        //Pega a diferenca de tempo da ultima leitura
        long timeLoop = millis() - tmpTime;
        int rpmx = 60000 / timeLoop;
        //Atribui novo rpm se ele for menor que o lido anteriormente
        //pois é um calculo independente do giro, para exibição em tempo real.
        if(rpmx >= 0 && rpmx < rpm){
          rpm = rpmx;
        }
      }
      writeData();
  }   
}

void writeData(){
    mi = millis();
    int tmpHumidity = dht.getHumidity();
    int tmpTemperature = dht.getTemperature();
    
    if(String(dht.getStatusString()).equals("OK")){
      err=0;
      humidity = tmpHumidity;
      temperature = tmpTemperature;
    }else{
       if(++err>5){
        temperature=-1;
        humidity=-1;
       }
    }
    data = "{\"atemp\":"+String(temperature)+",\"rhu\":"+String(humidity)+",\"cad\":"+String(rpm)+"}";
    if(automaticSendData){
      mySerial.println(data);
    }
    markToRead=true;
}

