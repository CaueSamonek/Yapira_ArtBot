import cv2 #camera e manipulacao de imagem
import mediapipe as mp # landmarks do corpo

cap = cv2.VideoCapture(0)

#variaveis para deteccao do corpo
mp_pose = mp.solutions.pose
mp_draw = mp.solutions.drawing_utils
mp_indexes = mp.solutions.pose.PoseLandmark

pose = mp_pose.Pose(min_detection_confidence=0.5, min_tracking_confidence=0.5)

line_y = 20 # porcentagem de altura do pulo em relacao a altura dos frames
jumping = False
jump_count=0
def wait_jump_signal():
    global line_y, jumping, jump_count
    jump_signal = None
    while True:
        read_ok, frame = cap.read()
        if not read_ok:
            print('Erro Ao Ler Câmera')
            break

        h, w,_ = frame.shape
        results = pose.process(frame)

        line_px = int(h*(1-line_y/100)) #calcula valor em pixel da linha
        cv2.line(frame, (0,line_px), (w,line_px), (0,0,255), 2) # desenha linha

        if results.pose_landmarks: # se achou corpo
            mp_draw.draw_landmarks(frame, results.pose_landmarks, mp_pose.POSE_CONNECTIONS)

            l_foot_y = int(results.pose_landmarks.landmark[mp_indexes.LEFT_FOOT_INDEX].y*h)
            r_foot_y = int(results.pose_landmarks.landmark[mp_indexes.RIGHT_FOOT_INDEX].y*h)

            # ambos os pes acima da linha
            both_above = l_foot_y < line_px and r_foot_y < line_px
            if both_above and not jumping:
                jumping = True
                jump_signal = 1;
            elif not both_above and jumping:
                jumping = False
                jump_count += 1
                jump_signal = 0; 
    
        #adiciona contador de pulo e mostra imagem
        cv2.putText(frame, f"Pulos: {jump_count}", (30, 60), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 255, 0), 2)
        cv2.imshow("Deteccao de Pulo", frame)

        if cv2.waitKey(1) & 0xFF == 27:
            cap.release()
            cv2.destroyAllWindows()
            return 0

        if jump_signal is not None:
            break

    return jump_signal
