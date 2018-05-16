#define SENSOR_CADENCE  A0
#define SENSOR_TEMP     A5

/**
 * Setup para Bluetooth - Comandos AT   
 * 
  Serial.print("AT");//Comando para verificar 'ok'
  delay(1000);
  Serial.print("AT+VERSION");//Comando para verificar versão
  delay(1000);
  Serial.print("AT+PIN1342"); //Define senha para 1342
  delay(1000);
  Serial.print("AT+NAMEHelpDev-Cadence"); //Define nome para HelpDev
  delay(1000);
 *
 */
void setup() {
  delay(1000);
  Serial.begin(9600);//Inicia bluetooth
  delay(1000);

}

int count = 0;
float temp = 0;
long tmpTime = -1;
int rpm = 0;
boolean inRead = false;

void loop() {
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
  
  temp += (float(analogRead(SENSOR_TEMP))*5/(1023))/0.01;
  
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
    Serial.println("{temp:"+String(temp/10)+",rpm:"+String(rpm)+"}");
    count=0;
    temp=0;
  }
  
  delay(100);
}
