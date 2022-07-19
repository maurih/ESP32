#define pwm_channel 0
#define mot_1 14
#define mot_2 27
#define res_1 26
#define res_2 25

#define temp 39

#define setpoint = 298.15
#define KI = 10;
#define KP = 10;
#define KD = 10;

auto erroanterior = 0;
auto pwmanterior = 0.5;
auto deltaerroanterior = 0;

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
  auto deltaerro = erro - erroanterior;
  //calculo saida PWM com PID
  auto deltapwm = ((erro / abs(erro)) * KI) + ((deltaerro / erro) * KP) + (((deltaerro * deltaerroanterior) / erro) * KD);
  auto pwmout = ((pwmanterior + deltapwm) * 100);
  pwmanterior = pwmout / 100;
  erroanterior = erro;
  deltaerroanterior = deltaerro;
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
