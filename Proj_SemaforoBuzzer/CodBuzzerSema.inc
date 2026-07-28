// Pino LED veicular
const int PINO_VERDE_V = 10;
const int PINO_AMARELO_V = 11;
const int PINO_VERMELHO_V = 12;
const int PINO_BUZZER = 6;

// Pino do LED de pedestre
const int PINO_VERDE_P = 7;
const int PINO_VERMELHO_P = 8;

// Pino BOT de pedestre
const int PINO_BOTAO_P = 2;

// Tempo de cada fase em ms
const unsigned long TEMPO_VERDE_V = 8000;
const unsigned long TEMPO_PISCA_VERDE_V = 2000;
const unsigned long TEMPO_AMARELO_V = 3000;
const unsigned long TEMPO_VERMELHO_V_MIN = 5000;
const unsigned long TEMPO_VERDE_P = 7000;
const unsigned long TEMPO_VERMELHO_P_PISCANTE = 3000;
const unsigned long TEMPO_PISCA_INTERVALO = 300;

// Estados do Semaforo para uso posterior
enum EstadoSemaforo {
    VEICULAR_VERDE,
    VEICULAR_VERDE_PISCANDO,
    VEICULAR_AMARELO,
    VEICULAR_VERMELHO,
    PEDESTRE_PREPARA,
    PEDESTRE_VERDE,
    PEDESTRE_VERMELHO_PISCANDO,
    PEDESTRE_FINALIZA
};

EstadoSemaforo estadoAtual = VEICULAR_VERDE;

unsigned long tempoEntradaEstado = 0;
unsigned long ultimoTempoPisca = 0;
bool ledPiscaEstado = LOW;
bool botaoPedestrePressionado = false;
bool pedidoPedestrePendente = false;

int estadoBotaoEstavel = HIGH;
int ultimoEstadoLeitura = HIGH;
unsigned long ultimoDebounceTempo = 0;
const unsigned long debounceDelay = 50;

void setup() {
    pinMode(PINO_VERDE_V, OUTPUT);
    pinMode(PINO_AMARELO_V, OUTPUT);
    pinMode(PINO_VERMELHO_V, OUTPUT);
    pinMode(PINO_BUZZER, OUTPUT);
    pinMode(PINO_VERDE_P, OUTPUT);
    pinMode(PINO_VERMELHO_P, OUTPUT);
    pinMode(PINO_BOTAO_P, INPUT_PULLUP);

    Serial.begin(9600);
    Serial.println("Semaforo com Chamada de Pedestre Iniciado.");

    mudarParaEstado(VEICULAR_VERDE);
}

void loop() {
    verificarBotaoPedestre();
    controlarSemaforo();
}

void verificarBotaoPedestre() {
    int leituraBotao = digitalRead(PINO_BOTAO_P);

    if (leituraBotao != ultimoEstadoLeitura) {
        ultimoDebounceTempo = millis();
    }

    if ((millis() - ultimoDebounceTempo) > debounceDelay) {
        if (leituraBotao != estadoBotaoEstavel) {
            estadoBotaoEstavel = leituraBotao;

            if (estadoBotaoEstavel == LOW) {
                if (!pedidoPedestrePendente) {
                    botaoPedestrePressionado = true;
                    pedidoPedestrePendente = true;
                    Serial.println("Botao de pedestre pressionado!");
                }
            }
        }
    }

    ultimoEstadoLeitura = leituraBotao;
}

void controlarSemaforo() {
    unsigned long tempoNoEstadoAtual = millis() - tempoEntradaEstado;

    switch (estadoAtual) {

        case VEICULAR_VERDE:
            if (tempoNoEstadoAtual >= (TEMPO_VERDE_V - TEMPO_PISCA_VERDE_V)) {
                if (pedidoPedestrePendente) {
                    mudarParaEstado(VEICULAR_VERDE_PISCANDO);
                } else if (tempoNoEstadoAtual >= TEMPO_VERDE_V) {
                    mudarParaEstado(VEICULAR_VERDE_PISCANDO);
                }
            }
            break;

        case VEICULAR_VERDE_PISCANDO:
            piscarLed(PINO_VERDE_V, TEMPO_PISCA_VERDE_V);

            if (tempoNoEstadoAtual >= TEMPO_PISCA_VERDE_V) {
                mudarParaEstado(VEICULAR_AMARELO);
            }
            break;

        case VEICULAR_AMARELO:
            if (tempoNoEstadoAtual >= TEMPO_AMARELO_V) {
                mudarParaEstado(VEICULAR_VERMELHO);
            }
            break;

        case VEICULAR_VERMELHO:
            if (pedidoPedestrePendente &&
                tempoNoEstadoAtual >= TEMPO_VERMELHO_V_MIN / 2) {
                mudarParaEstado(PEDESTRE_PREPARA);
            } else if (!pedidoPedestrePendente &&
                       tempoNoEstadoAtual >= TEMPO_VERMELHO_V_MIN) {
                mudarParaEstado(VEICULAR_VERDE);
            }
            break;

        case PEDESTRE_PREPARA:
            if (tempoNoEstadoAtual >= 500) {
                mudarParaEstado(PEDESTRE_VERDE);
            }
            break;

        case PEDESTRE_VERDE:
            if (tempoNoEstadoAtual >= TEMPO_VERDE_P) {
                mudarParaEstado(PEDESTRE_VERMELHO_PISCANDO);
            }
            break;

        case PEDESTRE_VERMELHO_PISCANDO:
            piscarLed(PINO_VERMELHO_P, TEMPO_VERMELHO_P_PISCANTE);

            if (tempoNoEstadoAtual >= TEMPO_VERMELHO_P_PISCANTE) {
                mudarParaEstado(PEDESTRE_FINALIZA);
            }
            break;

        case PEDESTRE_FINALIZA:
            if (tempoNoEstadoAtual >= 1000) {
                pedidoPedestrePendente = false;
                botaoPedestrePressionado = false;
                mudarParaEstado(VEICULAR_VERDE);
            }
            break;
    }
}

void mudarParaEstado(EstadoSemaforo novoEstado) {
    estadoAtual = novoEstado;
    tempoEntradaEstado = millis();
    ledPiscaEstado = LOW;

    apagarTodosLedsVeiculares();
    apagarTodosLedsPedestre();

    Serial.print("Mudando para estado: ");

    switch (estadoAtual) {

        case VEICULAR_VERDE:
            Serial.println("VEICULAR_VERDE");
            digitalWrite(PINO_VERDE_V, HIGH);
            digitalWrite(PINO_VERMELHO_P, HIGH);
            break;

        case VEICULAR_VERDE_PISCANDO:
            Serial.println("VEICULAR_VERDE_PISCANDO");
            digitalWrite(PINO_VERMELHO_P, HIGH);
            break;

        case VEICULAR_AMARELO:
            Serial.println("VEICULAR_AMARELO");
            digitalWrite(PINO_AMARELO_V, HIGH);
            digitalWrite(PINO_VERMELHO_P, HIGH);
            break;

        case VEICULAR_VERMELHO:
            Serial.println("VEICULAR_VERMELHO");
            digitalWrite(PINO_VERMELHO_V, HIGH);
            digitalWrite(PINO_VERMELHO_P, HIGH);
            break;

        case PEDESTRE_PREPARA:
            Serial.println("PEDESTRE_PREPARA");
            digitalWrite(PINO_VERMELHO_V, HIGH);
            digitalWrite(PINO_VERMELHO_P, HIGH);
            break;

        case PEDESTRE_VERDE:
            Serial.println("PEDESTRE_VERDE");
            digitalWrite(PINO_VERMELHO_V, HIGH);
            digitalWrite(PINO_VERDE_P, HIGH);
            break;

        case PEDESTRE_VERMELHO_PISCANDO:
            Serial.println("PEDESTRE_VERMELHO_PISCANDO");
            digitalWrite(PINO_VERMELHO_V, HIGH);
            digitalWrite(PINO_BUZZER, ledPiscaEstado);
            tone(6, 5274, 3000);
            break;

        case PEDESTRE_FINALIZA:
            Serial.println("PEDESTRE_FINALIZA");
            digitalWrite(PINO_VERMELHO_V, HIGH);
            digitalWrite(PINO_VERMELHO_P, HIGH);
            break;
    }
}

void piscarLed(int pinoLed, unsigned long duracaoTotalPisca) {
    if ((millis() - ultimoTempoPisca) > TEMPO_PISCA_INTERVALO) {
        ultimoTempoPisca = millis();
        ledPiscaEstado = !ledPiscaEstado;
        digitalWrite(pinoLed, ledPiscaEstado);
    }
}

void apagarTodosLedsVeiculares() {
    digitalWrite(PINO_VERDE_V, LOW);
    digitalWrite(PINO_AMARELO_V, LOW);
    digitalWrite(PINO_VERMELHO_V, LOW);
}

void apagarTodosLedsPedestre() {
    digitalWrite(PINO_VERDE_P, LOW);
    digitalWrite(PINO_VERMELHO_P, LOW);
}
