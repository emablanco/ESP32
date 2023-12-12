#include <Arduino.h>
int buzzer = 4;
int frecuencia = 200;
int canal = 0;

int sensor = 2;
unsigned long tiempo;
bool peligro = false;

bool movimiento();
void alarma(const int &);

void setup(){
  pinMode(sensor,INPUT);
  ledcSetup(canal,frecuencia,8);
  ledcAttachPin(buzzer,canal);
  Serial.begin(115200); 
}

void loop(){
  tiempo = millis();
    if(movimiento()){
      Serial.println("Estado alto");
      while(millis() - tiempo < 10000){
        for(frecuencia ; frecuencia < 2000; frecuencia +=10){
          alarma(10);
        }
        for(frecuencia ; frecuencia > 500; frecuencia -=10){
          alarma(10);
        }
      }
      peligro = !peligro;
    }
  }

bool movimiento(){
  if(digitalRead(sensor) == HIGH){
    peligro = !peligro;
  }
  return peligro;
}

void alarma(const int &t){
  ledcWriteTone(canal,frecuencia);
  ledcWrite(canal,128);
  delay(t);
  ledcWrite(canal, 0);
}
