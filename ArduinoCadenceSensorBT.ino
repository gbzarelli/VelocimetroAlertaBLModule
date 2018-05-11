const int SENSOR = A0;
long tmpTime = -1;
int rpm = 0;
boolean inRead = false;

void setup() {
  delay(1000);
  Serial.begin(9600);//Inicia bluetooth
  delay(1000);
 /* 
  Serial.print("AT");//Comando para verificar 'ok'
  delay(1000);
  Serial.print("AT+VERSION");//Comando para verificar versão
  delay(1000);
  Serial.print("AT+PIN1342"); //Define senha para 1342
  delay(1000);
  Serial.print("AT+NAMEHelpDev-Cadence"); //Define nome para HelpDev
  delay(1000);
  */
}

void loop() {
  int sensor = digitalRead(SENSOR);
  if(HIGH == sensor){
    if(!inRead){
      inRead=true;
      if(tmpTime == -1){
        tmpTime = millis();
      }else{
        long timeLoop = millis() - tmpTime;
        tmpTime = millis();
        rpm = 60000 / timeLoop;
        if(rpm<0){ rpm=0; }
        Serial.println(rpm);
      }
    }
  }else if(inRead){
    inRead=false;
  }
  delay(100);
}
