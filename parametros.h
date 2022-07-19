#ifndef parametros_h
#define parametros_h

#define pwm_channel 0

double setpoint = 0;
double pwmout = 0;

double KI = 0;
double KP = 0;
double KD = 0;
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

double histerese = 0;
double temperaturamaxima = 0;
double temperaturaminima = 0;

double tempoanteriorMQTT = 0;
String metodocontrole = "On/Off";
String lampada = "Off";

void iniMQTT();
void onConnectionEstablished();
void publicadorMQTT(double temperatura);

double calculoOnOff(double temperatura);
double calculoPID(double temperature);

#endif
