import cv2
import mediapipe as mp
import time
cap = cv2.VideoCapture(0)

mp_face_detection = mp.solutions.face_detection
mpHands = mp.solutions.hands
hands = mpHands.Hands(static_image_mode=False,
                      max_num_hands=2,
                      min_detection_confidence=0.5,
                      min_tracking_confidence=0.5)
mpDraw = mp.solutions.drawing_utils

pTime = 0
cTime = 0

while True:
    success, img = cap.read()
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(imgRGB)
    face_detection = mp_face_detection.FaceDetection(model_selection=0, min_detection_confidence=0.5)
    #print(results.multi_hand_landmarks)

    if results.multi_hand_landmarks:
        for handLms in results.multi_hand_landmarks: 
            for id, lm in enumerate(handLms.landmark):
                #print(id,lm)
                h, w, c = img.shape
                cx, cy = int(lm.x *w), int(lm.y*h)
                #if id ==0:
                cv2.circle(img, (cx,cy), 3, (255,0,255), cv2.FILLED)

            mpDraw.draw_landmarks(img, handLms, mpHands.HAND_CONNECTIONS)


    cTime = time.time()
    fps = 1/(cTime-pTime)
    pTime = cTime

    img.flags.writeable = False
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = face_detection.process(img)

    # Draw the face detection annotations on the img.
    img.flags.writeable = True
    img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)
    if results.detections:
      for detection in results.detections:
        mpDraw.draw_detection(img, detection)
    cv2.putText(img,str(int(fps)), (10,70), cv2.FONT_HERSHEY_PLAIN, 3, (255,0,255), 3)

    cv2.imshow("Image", cv2.flip(img, 1))
    if cv2.waitKey(5) & 0xFF == 27:
      break