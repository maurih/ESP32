#include "parametros.h"

double calculoOnOff(double temperatura){
  auto erro = setpoint - temperatura;
  temperaturamaxima = setpoint + histerese + 273.15;
  temperaturaminima = setpoint - histerese + 273.15;
  if(temperatura > temperaturamaxima) {
    pwmout = 0;
  }
  if(temperatura < temperaturaminima){
    pwmout = 100;
  }
  return pwmout;
}

double calculoPID(double temperatura){
  setpoint += 273.15;
  auto erro = setpoint - temperatura;
  tempoatual = millis();
  tempocorrido = tempoatual - tempoanterior;
  int i;
  //cálculo das médias móveis
  for(i=4; i>0; i--){
    mediamovel1[i] = mediamovel1[i-1];
    mediamovel2[i] = mediamovel2[i-1];
  }
  mediamovel1[0] = erro;
  for(i=4; i>-1; i--){
    calc1 += mediamovel1[i];
  }
  auto mediamovelerro1 = calc1/5;
  mediamovel2[0] = mediamovelerro1;
  for(i=4; i>-1; i--){
    calc2 += mediamovel2[i];
  }
  auto mediamovelerro2 = calc2/5;  
  //calculo saida PWM com PID
  auto deltapwm = ((erro / abs(erro)) * KI) + ((mediamovelerro1 / tempocorrido) * KP) + ((mediamovelerro2 / tempocorrido) * KD);
  auto pwmout = ((pwmanterior + deltapwm) * 100);
  pwmanterior = pwmout / 100;
  return pwmout;
}
