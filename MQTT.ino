#include "parametros.h"
#include "pinos.h"
#include "EspMQTTClient.h"

EspMQTTClient client(
  "WIFI",
  "SENHA",
  "broker.emqx.io",
  "emqx",
  "public",
  "controladormqtt",
  1883
);

void iniMQTT() {
    client.enableDebuggingMessages();
}

void onConnectionEstablished() {
  client.subscribe("quarto/temperatura/configurar/metodo", [](const String & payload) {     
    metodocontrole = payload;
  });
  client.subscribe("quarto/temperatura/configurar/histerese", [](const String & payload) {   
    if (metodocontrole == "On/Off")
        histerese = payload.toDouble();
  });
  client.subscribe("quarto/temperatura/configurar/KP", [](const String & payload) {     
    if (metodocontrole == "PID")
        KP = payload.toDouble();
  });
  client.subscribe("quarto/temperatura/configurar/KI", [](const String & payload) {    
    if (metodocontrole == "PID")
        KI = payload.toDouble();
  });
  client.subscribe("quarto/temperatura/configurar/KD", [](const String & payload) {     
    if (metodocontrole == "PID")
        KD = payload.toDouble();
  });
  client.subscribe("quarto/temperatura/setpoint", [](const String & payload) {      
        setpoint = payload.toDouble();
  });
  client.subscribe("quarto/luz/acender", [](const String & payload) {      
    lampada = payload;
    digitalWrite(rele,  payload == "On"? HIGH : LOW);
  });
  client.subscribe("quarto/luz/apagar", [](const String & payload) {       
    lampada = payload;
    digitalWrite(rele,  payload == "Off"? LOW : HIGH);
  });    
}

void publicadorMQTT(double temperatura) {
    client.loop();
    if (client.isConnected()) {
      if(millis()- tempoanteriorMQTT > 300){
          client.publish("quarto/temperatura", String(temperatura));
          client.publish("quarto/luz/estado", lampada);
          tempoanteriorMQTT = millis();
      }  
    }
    
}
