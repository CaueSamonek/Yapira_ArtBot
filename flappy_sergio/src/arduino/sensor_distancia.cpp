#include <AccelStepper.h>

// --- Definição dos Pinos ---
#define pino_passo 9
#define pino_direcao 8

// NOVOS PINOS para o sensor de distância
#define pino_trig 12
#define pino_echo 11

// --- CONSTANTES DO SENSOR E DA FÍSICA ---
const int DISTANCIA_PULO = 20; // Pula se nada for detectado a menos de 20 cm

const float GRAVIDADE = -6000.0;
const float FORCA_FLAP = 2200.0;
const float VELOCIDADE_MAXIMA = 6000.0;
const float ACELERACAO_MOTOR = 8000.0;

// LIMITES DO "JOGO"
const long CHAO_LIMITE = 0;
const long TETO_LIMITE = 950;
const long POSICAO_INICIAL = (CHAO_LIMITE + TETO_LIMITE) / 2;

// --- CONTROLE DE TEMPO E ESTADO ---
unsigned long ultima_atualizacao_fisica = 0;
const int INTERVALO_FISICA_MS = 20;

bool jogoIniciado = false;
AccelStepper motor(AccelStepper::DRIVER, pino_passo, pino_direcao);

float velocidade_atual_sps = 0.0;
// Variável para detectar a "borda" do sinal do sensor
bool objetoDetectadoAnteriormente = true;

// --- FUNÇÃO PARA MEDIR A DISTÂNCIA ---
// Retorna a distância em centímetros
long medirDistancia()
{
  // Dispara um pulso ultrassônico
  digitalWrite(pino_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(pino_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pino_trig, LOW);

  // Mede o tempo que o pulso demorou para voltar
  long duration = pulseIn(pino_echo, HIGH);

  // Converte o tempo em distância (cm)
  // Velocidade do som é 340 m/s ou 0.034 cm/µs
  // Dividimos por 2 por causa da viagem de ida e volta do som
  return duration * 0.034 / 2;
}

void setup()
{
  Serial.begin(9600);
  // Configura os pinos do sensor HC-SR04
  pinMode(pino_trig, OUTPUT);
  pinMode(pino_echo, INPUT);

  motor.setMaxSpeed(VELOCIDADE_MAXIMA);
  motor.setAcceleration(ACELERACAO_MOTOR);
  motor.setPinsInverted(true, false, false);

  motor.setCurrentPosition(CHAO_LIMITE);
  Serial.println("Assumindo posicao inicial no chao.");

  Serial.println("Movendo para a posicao central...");
  motor.moveTo(POSICAO_INICIAL);

  while (motor.distanceToGo() != 0) {
    motor.run();
  }
  Serial.println("Pronto! Remova o objeto da frente do sensor para comecar.");
}

void loop()
{
  // LÊ O SENSOR E DETERMINA SE O PULO DEVE SER ATIVADO
  long distancia = medirDistancia();
  bool objetoDetectadoAtualmente = (distancia < DISTANCIA_PULO && distancia > 0);

  // O pulo é ativado na TRANSIÇÃO: quando um objeto que ESTAVA na frente é REMOVIDO.
  bool flapAtivado = (objetoDetectadoAnteriormente && !objetoDetectadoAtualmente);
  objetoDetectadoAnteriormente = objetoDetectadoAtualmente;

  // Se o "flap" foi ativado...
  if (flapAtivado) {
    // Se o jogo ainda não começou, esta é a chamada para iniciar!
    if (!jogoIniciado) {
      jogoIniciado = true;
      ultima_atualizacao_fisica = millis();
      Serial.println("Jogo iniciado!");
    }
    // Aplica o pulo (flap)
    velocidade_atual_sps = FORCA_FLAP;
  }

  // A FÍSICA SÓ É APLICADA DEPOIS QUE O JOGO COMEÇAR
  if (jogoIniciado && (millis() - ultima_atualizacao_fisica >= INTERVALO_FISICA_MS)) {
    float dt = (millis() - ultima_atualizacao_fisica) / 1000.0;
    ultima_atualizacao_fisica = millis();

    velocidade_atual_sps += GRAVIDADE * dt;

    long pos_atual = motor.currentPosition();
    long proxima_posicao = pos_atual + (long)(velocidade_atual_sps * dt);

    if (proxima_posicao <= CHAO_LIMITE) {
      proxima_posicao = CHAO_LIMITE;
      if (velocidade_atual_sps < 0) {
        velocidade_atual_sps = 0;
      }
    }
    if (proxima_posicao >= TETO_LIMITE) {
      proxima_posicao = TETO_LIMITE;
      if (velocidade_atual_sps > 0) {
        velocidade_atual_sps = 0;
      }
    }

    motor.moveTo(proxima_posicao);
  }

  motor.run();
}
