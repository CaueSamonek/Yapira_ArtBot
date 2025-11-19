#codigo de pulo principal pra linux e windows, para utilizar:
#emparelhar bluetooth e rodar este arquivo apos baixar dependencias

# "from <module> import wait_jump_signal", <module> deve implementar a funcao que
# devolve um numero pra cada tipo de pulo (0=terminou pulo, 1=fez pulo fraco, ...)
import time
################## testes
i=0
def wait_jump_signal():
    global i
    time.sleep(0.5)
    i+=1
    return i%2;
###################from landmarks import wait_jump_signal

import serial
import platform
import sys

bluetooth_port = sys.argv[1]

BTreceiver = None
def connectBluetooth(device_path, baud_rate=9600, wait_connection=0):
    if platform.system().lower().startswith("win"):#para windows, nao testado
        for p in serial.tools.list_ports.comports():
            desc = (p.description or "") + " " + (p.product or "") + " " + (p.name or "")
            if "HC-05" in desc:
                device_path = p.device
                break
    print("main.py: Tentando Conectar Bluetooth Na Porta", device_path)

    receiver = serial.Serial(device_path, baud_rate)
    time.sleep(wait_connection) #aguarda completar conexao
    return receiver

print('Pareando HC')
BTreceiver = connectBluetooth(bluetooth_port)
print('Pareado')

jumping = False
jump_count = 0
while True:
    try:
        jump_signal = str(wait_jump_signal())
        print(jump_signal)
        BTreceiver.write(jump_signal.encode())

    except serial.SerialException:
        print("HC-05 Desconectado | Tentando Reconexão...")
        BTreceiver = connectBluetooth(bluetooth_port)
        print("HC-05 Reconectado")
