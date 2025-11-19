import tkinter as tk
from PIL import Image, ImageTk
import serial

BACKGROUND_IMAGE = "bg.jpeg"
WINDOW_W = 1000
WINDOW_H = 1000

# Cria a janela
root = tk.Tk()
root.title("Contador")
root.geometry(f"{WINDOW_W}x{WINDOW_H}")

# Carrega imagem de fundo
img = Image.open(BACKGROUND_IMAGE)
img = img.resize((WINDOW_W, WINDOW_H))
bg_image = ImageTk.PhotoImage(img)

# Label para a imagem de fundo
bg_label = tk.Label(root, image=bg_image)
bg_label.place(x=0, y=0, relwidth=1, relheight=1)

# Label do contador por cima
label = tk.Label(
    root,
    text="0",
    font=("Helvetica", 96, "bold"),
    fg="#00ff00",
    bg="#ffffff",
    bd=0
)
label.place(relx=0.5, rely=0.5, anchor="center")  # centraliza

# Função para atualizar o contador
def show(num):
    label.config(text=str(num))
    root.update_idletasks()

# ------------------ Serial ------------------
ser = None
try:
    ser = serial.Serial("/dev/ttyUSB0", 9600) #ou 'COM3' ou outra porta conectada
except:
    ser = serial.Serial("/dev/ttyUSB1", 9600) #ou 'COM3' ou outra porta conectada

def update_counter():
    if ser.in_waiting > 0:
        try:
            line = ser.readline().decode().strip()
            num = int(line)
            show(num)
        except:
            pass
    root.after(50, update_counter)

# inicia loop de atualização
update_counter()
root.mainloop()
