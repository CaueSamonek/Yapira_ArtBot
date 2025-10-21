import cv2
import numpy as np

cap = cv2.VideoCapture(0)

# subtrator de fundo adaptativo, aparentemente o KNN eh mais simples/rapido
#fgbg = cv2.createBackgroundSubtractorMOG2(history=100, varThreshold=10, detectShadows=False)
fgbg = cv2.createBackgroundSubtractorKNN(history=20, dist2Threshold=500, detectShadows=False)

while True:
    ret, frame = cap.read()
    if not ret:
        print("Erro Ao Ler Câmera")
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY) # muda para escala de cinza
    blur_gray = cv2.GaussianBlur(gray, (9, 9), 0) # adiciona blur para suavizacao
    masked_gray = fgbg.apply(blur_gray) # aplica subtrator de fundo
    masked_gray = cv2.morphologyEx(masked_gray, cv2.MORPH_OPEN, None, iterations=2)

    #desenha retangulos de contorno no frame principal
    contours = cv2.findContours(masked_gray,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)[0]
    mask = np.zeros_like(masked_gray)
    for c in contours:
        x,y,w,h = cv2.boundingRect(c)
        cv2.rectangle(frame, (x,y), (x+w, y+h), (0,255,0), 2)
        cv2.drawContours(mask, [c], -1, 255, -1)

    cv2.imshow("Color Frame", frame)
    cv2.imshow("Masked Frame", masked_gray)

    if cv2.waitKey(1) & 0xFF == 27:
        break

cap.release()
cv2.destroyAllWindows()

