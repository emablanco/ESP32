#include "configEma.h"
#include "CTBot.h"


template<class T> inline Print &operator <<(Print &obj, T arg) {
  obj.print(arg);
  return obj;
}



CTBot miBot;
CTBotInlineKeyboard miTeclado;
CTBotInlineKeyboard miURL;

int buzzer = 4;
int frecuencia = 200;
int canal = 0;
int sensor = 2;
unsigned long tiempo;
bool peligro = false;
bool alarmaActivada = false;

bool movimiento();
void alarma(const int &);

uint32_t emaID = 5604166531;

void setup(){

  pinMode(sensor,INPUT);
  ledcSetup(canal,frecuencia,8);
  ledcAttachPin(buzzer,canal);
  Serial.begin(115200);
  Serial.println("Iniciando Bot Telegram...");

  miBot.wifiConnect(ssid, password);

  miBot.setTelegramToken(token);

  if (miBot.testConnection()) {
    Serial.println("Bot Conectado");
  }
  else {
    Serial.println("Error con Bot");
  }

  miTeclado.addButton("Encender", "encender", CTBotKeyboardButtonQuery);
  miTeclado.addButton("Apagar", "apagar", CTBotKeyboardButtonQuery);
  miTeclado.addRow();
  miTeclado.addButton("mira documentación", "https://emablanco.github.io/", CTBotKeyboardButtonURL);
  miURL.addButton("Mira la Cam 1", "https://emablanco.github.io/", CTBotKeyboardButtonURL);

}

void loop(){
  
  TBMessage msg;

  if (miBot.getNewMessage(msg)) {

    if (msg.messageType == CTBotMessageText) {
    
      if (msg.text.equalsIgnoreCase("opciones")) {
        
        miBot.sendMessage(msg.sender.id, "Cambiar estado", miTeclado);
      }
      
      else {
        miBot.sendMessage(msg.sender.id, "prueba 'opciones'");
      }

    } else if (msg.messageType == CTBotMessageQuery) {
    
      Serial << "Mensaje: " <<  msg.sender.firstName;
      
      if (msg.callbackQueryData.equals("encender")) {
      
        Serial.println(" Encender");
        
        alarmaActivada = true;
        
        miBot.endQuery(msg.callbackQueryID, "Alarma Encendido", true);
      
      } else if (msg.callbackQueryData.equals("apagar")) {
      
         Serial.println(" Apagar Alarma");
         
          alarmaActivada = false;
        
        miBot.endQuery(msg.callbackQueryID, "Camara apgada");
      }
    }
  }

  delay(250);

  if(alarmaActivada){
    alarmaFuncionando();
    //no anda emaID solo el numero
    miBot.sendMessage(5604166531, "Cambiar estado", miURL);

  }
}


void alarmaFuncionando(){
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
