const int pinoPot = A0;
const int pinoLedPwm = 9;
const int pinoBotao = 2;
int valorPot = 0;
int valorPwm = 0;
int estadoBotao = 0;
void setup(){
Serial.begin(9600);
pinMode(pinoLedPwm, OUTPUT);
pinMode(pinoBotao, INPUT_PULLUP);
Serial.println("Pratica 5 Combinada
Iniciada");
Serial.println("Potenciometro controla
brilho, botao apaga o LED.");
}
void loop(){
estadoBotao = digitalRead(pinoBotao);
valorPot = analogRead(pinoPot);
valorPwm = map(valorPot, 0, 1023, 0, 255);
if (estadoBotao == LOW){
analogWrite(pinoLedPwm, 0);
Serial.println("Botao Pressionado - LED
APAGADO");
} else {
(estadoBotao == HIGH),
analogWrite(pinoLedPwm, valorPwm);
Serial.print("Botao Solto - Pot: ");
Serial.print(valorPot);
Serial.print(" -> PWM: ");
Serial.println(valorPwm);
