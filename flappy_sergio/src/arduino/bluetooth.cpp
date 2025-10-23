#include <AccelStepper.h>
#include <Arduino.h>
#include <SoftwareSerial.h>


// Fisica
#define VELOCIDADE_MAXIMA_SUBIDA 4000
#define VELOCIDADE_MAXIMA_QUEDA -4000
#define FORCA_PULO 1000
#define GRAVIDADE -500

// Motor
#define PIN_STEP 3
#define PIN_DIRECTION 2
#define VELOCIDADE_MAXIMA 4000

// Bluetooth
#define BT_RX_PIN 9
#define BT_TX_PIN 10
#define BT_BAUD_RATE 9600

// Posicoes Limite da Barra Vertical
#define CEIL_LIMIT 0
#define FLOOR_LIMIT 1200
#define INITIAL_POSITION FLOOR_LIMIT

SoftwareSerial BTserial(BT_TX_PIN, BT_RX_PIN);//rx tx arduino
AccelStepper motor(AccelStepper::DRIVER, PIN_STEP, PIN_DIRECTION);

void setup(){
  Serial.begin(BT_BAUD_RATE);
  BTserial.begin(BT_BAUD_RATE);

  motor.setMaxSpeed(VELOCIDADE_MAXIMA);
  motor.setCurrentPosition(INITIAL_POSITION); 
}

long velocidade_atual = 0;
int jump_signal = 0; 

void loop(){ 
    if (BTserial.available())
        jump_signal = BTserial.read()-'0';// le como int '1' quando pula e '0' quando cai
        
    velocidade_atual = GRAVIDADE;
    if (jump_signal)
      velocidade_atual = FORCA_PULO;

    long posicao_atual = motor.currentPosition();    
    // mantem dentro do limite inferior
    if (posicao_atual <= CEIL_LIMIT){
      motor.setCurrentPosition(CEIL_LIMIT);
      if (jump_signal)
        velocidade_atual = 0;
    }
    // mantem dentro do limite superior
    if (posicao_atual >= FLOOR_LIMIT){
      motor.setCurrentPosition(FLOOR_LIMIT);
      if (!jump_signal)
        velocidade_atual = 0;
    }
    motor.setSpeed(-velocidade_atual);
    motor.runSpeed();
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
