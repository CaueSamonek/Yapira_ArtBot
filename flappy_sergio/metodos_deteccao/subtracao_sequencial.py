import cv2
import numpy as np

cap = cv2.VideoCapture(0)

prev_gray = None

while True:
    ret, frame = cap.read()
    if not ret:
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gray = cv2.GaussianBlur(gray, (9,9), 0)

    if prev_gray is None:
        prev_gray = gray
        continue

    # diferença de frames
    diff = cv2.absdiff(prev_gray, gray)
    diff = cv2.threshold(diff, 50, 200, cv2.THRESH_BINARY)[1]
    diff = cv2.morphologyEx(diff, cv2.MORPH_OPEN, np.ones((3,3), np.uint8))

    # pega contornos
    contours, _ = cv2.findContours(diff, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    mask_filtered = np.zeros_like(diff)

    for c in contours:
        x, y, w, h = cv2.boundingRect(c)
        cv2.drawContours(mask_filtered, [c], -1, 255, -1)
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0,255,0), 2)

    cv2.imshow("Color Frame", frame)
    cv2.imshow("Masked Frame", mask_filtered)

    prev_gray = gray.copy()
    if cv2.waitKey(1) & 0xFF == 27:
        break

cap.release()
cv2.destroyAllWindows()

