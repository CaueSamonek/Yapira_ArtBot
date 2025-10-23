#codigo de pulo principal pra linux e windows, para utilizar:
#emparelhar bluetooth e rodar este arquivo apos baixar dependencias

# "from <module> import wait_jump_signal", <module> deve implementar a funcao que
# devolve um numero pra cada tipo de pulo (0=terminou pulo, 1=fez pulo fraco, ...)
from landmarks import wait_jump_signal
import serial
import time
import platform

BTreceiver = None
def connectBluetooth(device_path="/dev/rfcomm0", baud_rate=9600, wait_connection=5):
    if platform.system().lower().startswith("win"):#para windows, nao testado
        for p in serial.tools.list_ports.comports():
            desc = (p.description or "") + " " + (p.product or "") + " " + (p.name or "")
            if "HC-05" in desc:
                device_path = p.device
                break

    receiver = serial.Serial(device_path, baud_rate)
    time.sleep(wait_connection) #aguarda completar conexao
    return receiver

print('Pareando HC')
BTreceiver = connectBluetooth()
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
        BTreceiver = connectBluetooth()
        print("HC-05 Reconectado")
