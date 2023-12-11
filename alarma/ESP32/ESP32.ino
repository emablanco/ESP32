
int buzzer =4;
int frecuencia = 500;
int canal = 0;

void setup() {
  ledcSetup(canal, frecuencia, 8);
  ledcAttachPin(buzzer, canal);
}

void loop() {
  for(frecuencia; frecuencia < 2000; frecuencia+= 15){
    tono(frecuencia,20); 
  }
  for(frecuencia ; frecuencia > 500; frecuencia -= 15){
    tono(frecuencia, 20);
  }
}

void tono(int f, int tiempo){
  ledcWriteTone(canal,f);
  ledcWrite(canal,146);
  delay(tiempo);
  ledcWrite(canal,0);
  //delay(tiempo);
}
