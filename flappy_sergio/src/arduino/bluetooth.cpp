#include <AccelStepper.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

// Valores do GPT
//#define VELOCIDADE_MAXIMA 8000 // steps/s (≈150 RPM with 3200 steps/rev)
//#define VELOCIDADE_MAXIMA_QUEDA -3000 // steps/s (falling speed limit)
//#define FORCA_PULO 3000 // steps/s (initial jump velocity)
//#define GRAVIDADE -8000 // steps/s² (gravity acceleration)
//#define ACELERACAO_MOTOR 20000 // if using acceleration mode

// Fisica
#define VELOCIDADE_MAXIMA 1000
#define VELOCIDADE_MAXIMA_QUEDA -1200.0
#define FORCA_PULO 2200.0
#define GRAVIDADE -6000.0
#define ACELERACAO_MOTOR 500

// Motor
#define PIN_STEP 9
#define PIN_DIRECTION 8

// Bluetooth
#define BT_RX_PIN 2
#define BT_TX_PIN 3
#define BT_BAUD_RATE 9600;

// Posicoes Limite
#define FLOOR_LIMIT 0
#define CEIL_LIMIT 850
#define INITIAL_POSITION 425

SoftwareSerial BTserial(BT_TX_PIN, BT_RX_PIN);//rx tx arduino
AccelStepper motor(AccelStepper::DRIVER, PIN_STEP, PIN_DIRECTION);

void setup(){
  Serial.begin(BT_BAUD_RATE);
  BTserial.begin(BT_BAUD_RATE);

  motor.setMaxSpeed(VELOCIDADE_MAXIMA);
  motor.setAcceleration(ACELERACAO_MOTOR);
  motor.setCurrentPosition(INITIAL_POSITION); 
}

long velocidade_atual = 0;
int jump_signal = 0; 

void loop(){
    if (BTserial.available())
        jump_signal = BTserial.read()-'0';// le como int '1' quando pula e '0' quando cai

    // aplica fisica de queda
    velocidade_atual += GRAVIDADE;
    if (velocidade_atual < VELOCIDADE_MAXIMA_QUEDA)
        velocidade_atual = VELOCIDADE_MAXIMA_QUEDA;

    // verifica pulo
    if (jump) {
        velocidade_atual = FORCA_PULO;
        Serial.print("Pulo! Velocidade definida para: ");
        Serial.println(velocidade_atual);
    }

    // define proxima posicao do motor
    long proxima_posicao = motor.currentPosition() + velocidade_atual;

    // mantem dentro do limite inferior
    if (proxima_posicao < CHAO_LIMITE){
        proxima_posicao = CHAO_LIMITE;
        velocidade_atual = 0;
    }
    // mantem dentro do limite superior
    if (proxima_posicao > TETO_LIMITE){
        proxima_posicao = TETO_LIMITE;
        velocidade_atual = 0;
    }

    // move motor
    motor.moveTo(proxima_posicao);
    motor.run();

    Serial.print("posição atual: ");
    Serial.println(motor.currentPosition());
}



/*

-> ideia para tratar desconexao do modulo bluetooth: verificacao do pino state
-> se desconectar, travar motores = pausar jogo ateh reconectar

const int BT_STATE_PIN = 7; // pino conectado ao STATE do HC-05

void setup() {
  Serial.begin(9600);
  pinMode(BT_STATE_PIN, INPUT);
}

void loop() {
  if (digitalRead(BT_STATE_PIN) == HIGH)
      Serial.println("Bluetooth conectado");
  else
      Serial.println("Bluetooth desconectado");
}

*/
