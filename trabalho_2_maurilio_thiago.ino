#include "pinos.h"
#include "parametros.h"

void setup() {
  Serial.begin(19200);
  ledcSetup (pwm_channel, 5000, 16);
  ledcAttachPin (mot_1, pwm_channel);
  ledcAttachPin (res_1, pwm_channel);
  ledcWrite (pwm_channel, 3227);
  pinMode (mot_2, OUTPUT);
  pinMode (res_2,OUTPUT);
  digitalWrite (mot_2, HIGH);
  digitalWrite (res_2, HIGH);
}

void loop() {
  auto value = analogRead(temp);
  double temperatura = 0.080586 * value;
  publicadorMQTT(temperatura);
  if(metodocontrole == "On/Off"){
    pwmout = calculoOnOff(temperatura);
  }
  if(metodocontrole == "PID"){
    pwmout = calculoPID(temperatura);
  }
  if (pwmout > 50 && pwmout < 100){
    digitalWrite (res_2, HIGH);
    digitalWrite (mot_2, LOW);
    int saidapwm = pwmout * 65.52;
    ledcWrite (pwm_channel, saidapwm);
  }
  if (pwmout < 50 && pwmout > 0){
    digitalWrite (mot_2, HIGH);
    digitalWrite (res_2, LOW);
    int saidapwm = (100 - pwmout) * 65.52;
    ledcWrite (pwm_channel, saidapwm);
  }
  if (pwmout = 50){
    digitalWrite (mot_2, HIGH);
    digitalWrite (res_2, HIGH);
  }
  if (pwmout >= 100){
    int saidapwm = 6554;
    digitalWrite (res_2, HIGH);
    digitalWrite (mot_2, LOW);
    ledcWrite (pwm_channel, saidapwm);
  }
  if (pwmout <= 0){
    int saidapwm = 6554;
    digitalWrite (mot_2, HIGH);
    digitalWrite (res_2, LOW);
    ledcWrite (pwm_channel, saidapwm);
  }
}
