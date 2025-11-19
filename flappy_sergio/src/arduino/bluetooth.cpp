#include <AccelStepper.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

// Fisica
#define VELOCIDADE_MAXIMA_SUBIDA 1000
#define VELOCIDADE_MAXIMA_QUEDA -1000
#define FORCA_PULO 1000
#define GRAVIDADE -500

//ambos os motores
#define VELOCIDADE_MAXIMA 1000

// Motor barra
#define PIN_STEP 0
#define PIN_DIRECTION 0

// Motor esteira
#define PIN_STEP_ESTEIRA 3
#define PIN_DIR_ESTEIRA 2
#define VELOCIDADE_ESTEIRA -250

// Bluetooth
#define BT_RX_PIN 9
#define BT_TX_PIN 10
#define BT_BAUD_RATE 9600

// Posicoes Limite da Barra Vertical
#define CEIL_LIMIT 0
#define FLOOR_LIMIT 1200
#define INITIAL_POSITION FLOOR_LIMIT

SoftwareSerial BTserial(BT_TX_PIN, BT_RX_PIN);
AccelStepper motor(AccelStepper::DRIVER, PIN_STEP, PIN_DIRECTION);
AccelStepper motorEsteira(AccelStepper::DRIVER, PIN_STEP_ESTEIRA, PIN_DIR_ESTEIRA);

void setup(){
  Serial.begin(BT_BAUD_RATE);
  BTserial.begin(BT_BAUD_RATE);

  motor.setMaxSpeed(VELOCIDADE_MAXIMA);
  motor.setCurrentPosition(INITIAL_POSITION);

  motorEsteira.setMaxSpeed(VELOCIDADE_MAXIMA);
  motorEsteira.setSpeed(VELOCIDADE_ESTEIRA);
}

long velocidade_atual = 0;
int jump_signal = 0;

void loop(){ 
    if (BTserial.available())
        jump_signal = BTserial.read() - '0';
        
    velocidade_atual = GRAVIDADE;
    if (jump_signal)
        velocidade_atual = FORCA_PULO;

    long posicao_atual = motor.currentPosition();    

    if (posicao_atual <= CEIL_LIMIT){
        motorEsteira.setSpeed(0);//mata no teto
        motor.setCurrentPosition(CEIL_LIMIT);
        if (jump_signal)
            velocidade_atual = 0;
    }

    if (posicao_atual >= FLOOR_LIMIT){
        //motorEsteira.setSpeed(0); nao mata no chao
        motor.setCurrentPosition(FLOOR_LIMIT);
        if (!jump_signal)
            velocidade_atual = 0;
    }

    motor.setSpeed(-velocidade_atual);
    motor.runSpeed();
    motorEsteira.runSpeed();
}