const int pinoLDR = A0;
const int pinoLed = 3;
const int limiarEscuro = 500;

int valorLuz = 0;

void setup(){
Serial.begin(9600);
pinMode(pinoLed, OUTPUT);
Serial.println("Pratica4-SensordeLuminosidade
LDR Iniciada");
}

void loop(){
valorLuz = analogRead(pinoLDR);
Serial.print("Leitura LDR (A0): ");
Serial.println(valorLuz);

if (valorLuz < limiarEscuro){
digitalWrite(pinoLed, HIGH);
Serial.println("Ambiente ESCURO -LED esta aceso");
}

else{
digitalWrite(pinoLed, LOW);
Serial.println("Ambiente CLARO - LED Apagado");
}
delay(250);
}
