#define pwm_channel 0
#define mot_1 14
#define mot_2 27
#define res_1 26
#define res_2 25

#define temp 39

double setpoint = 298.15;
double KI = 1;
double KP = 2;
double KD = 1;

double tempoatual = 0;
double tempocorrido = 0;
double tempoanterior = 0;
double pwmanterior = 0.5;
double mediamovel1[5]={0, 0, 0, 0, 0};
double mediamovel2[5]={0, 0, 0, 0, 0};
double mediamovelerro1 = 0;
double mediamovelerro2 = 0;
double calc1 = 0;
double calc2 = 0;

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
  //tratando a temperatura
  auto value = analogRead(temp);
  auto temperatura = 0.080586 * value;
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
  //atuacao do controlador
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
  if(pwmout <=0){
    int saidapwm = 6554;
    digitalWrite (mot_2, HIGH);
    digitalWrite (res_2, LOW);
    ledcWrite (pwm_channel, saidapwm);
  }  
  delay (500);
}
