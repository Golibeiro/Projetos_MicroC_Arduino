const int pinoPot = A0;
const int pinoLedPwmPrincipal = 9;
const int pinoBotao = 2;
const int pinoLedFeedback = 3;

int valorPot = 0;
int valorPwm = 0;
int estadoBotaoAtual = HIGH;
int estadoBotaoAnterior = HIGH;

unsigned long ultimoDebounceTempo = 0;
unsigned long debounceDelay = 50;

boolean modoAnulacaoAtivo = false;

void setup(){
  Serial.begin(9600);
  pinMode(pinoLedPwmPrincipal, OUTPUT);
  pinMode(pinoLedFeedback, OUTPUT);
  pinMode(pinoBotao, INPUT_PULLUP);
  digitalWrite(pinoLedFeedback, LOW);
  Serial.println("Pratica Combinada - DESAFIO 2: Feedback Visual");
  Serial.println("Potenciometro controla brilho. Clique no botao para apagar/liberar LED.");
  Serial.println("LED Vermelho indica modo de anulacao ATIVO.");
}

void loop(){
  int leituraBotao = digitalRead(pinoBotao);
  if (leituraBotao != estadoBotaoAnterior) {
    ultimoDebounceTempo = millis();
  }

  if ((millis() - ultimoDebounceTempo) > debounceDelay) {
    if (leituraBotao != estadoBotaoAtual) {
      estadoBotaoAtual = leituraBotao;

      if (estadoBotaoAtual == LOW) {
        modoAnulacaoAtivo = !modoAnulacaoAtivo;

        if (modoAnulacaoAtivo){
          Serial.println("MODO ANULACAO ATIVO (LED Principal APAGADO, LED Feedback ACESO)");
          digitalWrite(pinoLedFeedback, HIGH);
        } else{
          Serial.println("MODO ANULACAO DESATIVADO (Pot controla, LED Feedback APAGADO)");
          digitalWrite(pinoLedFeedback, LOW);
  }
  }
  }
  }

  estadoBotaoAnterior = leituraBotao;

  valorPot = analogRead(pinoPot);
  valorPwm = map(valorPot, 0, 1023, 0, 255);

  if (modoAnulacaoAtivo){
    analogWrite(pinoLedPwmPrincipal, 0);
  } else{
    analogWrite(pinoLedPwmPrincipal, valorPwm);

    if (!modoAnulacaoAtivo){
      Serial.print("Pot controla - Pot: ");
      Serial.print(valorPot);
      Serial.print(" -> PWM: ");
      Serial.println(valorPwm);
  }
}
}
