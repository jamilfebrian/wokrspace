import cv2 
import numpy as np

cap = cv2.VideoCapture(0)
cap.set(3,640)
cap.set(4,480)

def empty():
    pass

cv2.namedWindow("HSV")
cv2.resizeWindow("HSV",640,240)
cv2.createTrackbar("HUE min","HSV",0,179,empty)
cv2.createTrackbar("HUE max","HSV",179,179,empty)
cv2.createTrackbar("SAT min","HSV",0,255,empty)
cv2.createTrackbar("SAT max","HSV",255,255,empty)
cv2.createTrackbar("VAL min","HSV",0,255,empty)
cv2.createTrackbar("VAL max","HSV",255,255,empty)


while True:
    _, img = cap.read()
    imgHSV = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)

    h_min = cv2.getTrackbarPos("HUE min","HSV")
    h_max = cv2.getTrackbarPos("HUE max","HSV")
    s_min = cv2.getTrackbarPos("SAT min","HSV")
    s_max = cv2.getTrackbarPos("SAT max","HSV")
    v_min = cv2.getTrackbarPos("VAL min","HSV")
    v_max = cv2.getTrackbarPos("VAL max","HSV")\
    
    lower = np.array([h_min,s_min,v_min])
    upper = np.array([h_max,s_max,v_max])
    mask = cv2.inRange(imgHSV,lower,upper)
    result = cv2.bitwise_and(img,img,mask=mask)

    hstack = np.hstack([mask,result])

    cv2.imshow("Kamera", img)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()