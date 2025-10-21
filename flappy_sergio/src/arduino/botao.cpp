#include <AccelStepper.h>

// --- Definição dos Pinos ---
#define pino_passo 9
#define pino_direcao 8
#define pino_botao 4

// !!! --- CONSTANTES DA FÍSICA E LIMITES --- !!!
const float GRAVIDADE = 2;
const float FORCA_PULO = -3000.0;
const float VELOCIDADE_MAXIMA_QUEDA = 1200.0;

// Valores testados !!!!!
const long CHAO_LIMITE = 0;
const long TETO_LIMITE = 850;

const long POSICAO_INICIAL = (CHAO_LIMITE + TETO_LIMITE) / 2;

AccelStepper motor(AccelStepper::DRIVER, pino_passo, pino_direcao);
float velocidade_atual = 0.0;
bool estado_anterior_botao = false;

void setup()
{
  Serial.begin(9600);
  pinMode(pino_botao, INPUT_PULLUP);
  motor.setMaxSpeed(1000);
  motor.setAcceleration(500);
  motor.setCurrentPosition(POSICAO_INICIAL);
  Serial.println("Sistema iniciado com limites de altura e posicao inicial central.");
  Serial.println("Pressione o botao para pular.");
}

void loop()
{
  bool estado_atual_botao = (digitalRead(pino_botao) == LOW);
  bool botao_foi_apertado = (estado_anterior_botao == false && estado_atual_botao == true);

  // 1. APLICA A FÍSICA A CADA LOOP
  velocidade_atual += GRAVIDADE;

  // 2. LIMITA A VELOCIDADE DE QUEDA
  if (velocidade_atual < VELOCIDADE_MAXIMA_QUEDA) {
    velocidade_atual = VELOCIDADE_MAXIMA_QUEDA;
  }

  // 3. VERIFICA O PULO
  if (botao_foi_apertado) {
    velocidade_atual = FORCA_PULO;
    Serial.print("Pulo! Velocidade definida para: ");
    Serial.println(velocidade_atual);
  }

  // 4. CALCULA A PRÓXIMA POSIÇÃO ALVO
  long proxima_posicao = motor.currentPosition() + (long)velocidade_atual;

  // 5. APLICA OS LIMITES DO CHÃO E DO TETO (O SEGREDO ESTÁ AQUI)
  // Garante que a posição alvo NUNCA saia do intervalo.
  if (proxima_posicao < CHAO_LIMITE) {
    proxima_posicao = CHAO_LIMITE;
    // Reseta a velocidade para 0 para evitar que a gravidade se acumule e "empurre" contra o
    // limite.
    velocidade_atual = 0;
  }

  if (proxima_posicao > TETO_LIMITE) {
    proxima_posicao = TETO_LIMITE;
    // Reseta a velocidade ao atingir o teto.
    velocidade_atual = 0;
  }

  // 6. ATUALIZA E EXECUTA O MOVIMENTO (SEMPRE ACONTECEM)
  motor.moveTo(proxima_posicao);
  motor.run();

  Serial.print("posição atual: ");
  Serial.println(motor.currentPosition());

  estado_anterior_botao = estado_atual_botao;
}
