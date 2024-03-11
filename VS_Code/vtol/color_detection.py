import cv2 
import numpy as np
from PIL import Image
import Jetson.GPIO as GPIO

cap = cv2.VideoCapture(0)
pin_relay = 18 
counter = True
dotX = 100
dotY = 200

cap.set(3,640)
cap.set(4,480)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(pin_relay, GPIO.OUT)
GPIO.output(pin_relay, GPIO.HIGH)

def camera():
    while True:
        _, img = cap.read()
        imgHsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)

        lower = np.array([0,181,76])  #atue nilai lower HSV
        upper = np.array([108,255,255]) #atur nilai Upper HSV
        
        mask = cv2.inRange(imgHsv,lower,upper)
        mask_ = Image.fromarray(mask)
        bbox = mask_.getbbox()
        cv2.circle(img,(dotX,dotY),3,(0,0,255),3)

        if bbox is not None :
            global counter
            x1,y1,x2,y2 = bbox
            cv2.rectangle(img,(x1,y1),(x2,y2),(0,255,0),4)

            if (x1<dotX and x2>dotX) and (y1<dotY and y2>dotY) and counter:
                GPIO.output(pin_relay, GPIO.LOW)
                print("payload telah dijatuhkan")
                counter = False

        cv2.imshow("result", img)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            GPIO.cleanup()
            cap.release()
            cv2.destroyAllWindows()
            break

camera()

