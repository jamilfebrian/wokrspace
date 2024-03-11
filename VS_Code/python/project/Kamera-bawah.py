import cv2
import numpy as np
from time import sleep
# import pigpio

# pi = pigpio.pi()

def nothing(x):
    pass

cap = cv2.VideoCapture(0)

ret = cap.set(3,640) #width
ret = cap.set(4,480) #height
#ret = cap.set(10,50) #brightness min: 0   , max: 255 , increment:1
#ret = cap.set(11,50) #contrast min: 0   , max: 255 , increment:1
#ret = cap.set(12,50) #saturation min: 0   , max: 255 , increment:1
#ret = cap.set(13,50) #hue
#ret = cap.set(14,50) #gain
#ret = cap.set(15,0) #exposure
#ret = cap.set(17,5000) #white_balance
cap.set(cv2.CAP_PROP_AUTOFOCUS, 0)

cv2.namedWindow("Dropping")
cv2.resizeWindow("Dropping",280,250)

cv2.createTrackbar("L-H", "Dropping", 0, 179, nothing)
cv2.createTrackbar("L-S", "Dropping", 27, 255, nothing)
cv2.createTrackbar("L-V", "Dropping", 216, 255, nothing)
cv2.createTrackbar("U-H", "Dropping", 17, 179, nothing)
cv2.createTrackbar("U-S", "Dropping", 255, 255, nothing)
cv2.createTrackbar("U-V", "Dropping", 255, 255, nothing)

cv2.namedWindow("Landing")
cv2.resizeWindow("Landing",280,250)

cv2.createTrackbar("L-H", "Landing", 10, 179, nothing)
cv2.createTrackbar("L-S", "Landing", 131, 255, nothing)
cv2.createTrackbar("L-V", "Landing", 255, 255, nothing)
cv2.createTrackbar("U-H", "Landing", 58, 179, nothing)
cv2.createTrackbar("U-S", "Landing", 255, 255, nothing)
cv2.createTrackbar("U-V", "Landing", 255, 255, nothing)

kernel = np.ones((5,5),np.uint8)

while True:
    _, img = cap.read()
    #img = cv2.GaussianBlur(img,(1,1),0)
    hsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)     
    l_hR = cv2.getTrackbarPos("L-H","Dropping")
    l_sR = cv2.getTrackbarPos("L-S","Dropping")
    l_vR = cv2.getTrackbarPos("L-V","Dropping")
    u_hR = cv2.getTrackbarPos("U-H","Dropping")
    u_sR = cv2.getTrackbarPos("U-S","Dropping")
    u_vR = cv2.getTrackbarPos("U-V","Dropping")

    l_hY = cv2.getTrackbarPos("L-H","Landing")
    l_sY = cv2.getTrackbarPos("L-S","Landing")
    l_vY = cv2.getTrackbarPos("L-V","Landing")
    u_hY = cv2.getTrackbarPos("U-H","Landing")
    u_sY = cv2.getTrackbarPos("U-S","Landing")
    u_vY = cv2.getTrackbarPos("U-V","Landing")

    #oren
    red_lower = np.array([l_hR,l_sR,l_vR])
    red_upper = np.array([u_hR,u_sR,u_vR])
    
    #Cyan
    yellow_lower = np.array([l_hY,l_sY,l_vY])
    yellow_upper = np.array([u_hY,u_sY,u_vY])

    #masking
    red = cv2.inRange(hsv, red_lower, red_upper)
    yellow = cv2.inRange(hsv, yellow_lower, yellow_upper)
    
    #morphological opening
    red = cv2.erode(red,kernel,iterations=2)
    red = cv2.dilate(red,kernel,iterations=2)

    yellow = cv2.erode(yellow,kernel,iterations=2)
    yellow = cv2.dilate(yellow,kernel,iterations=2)

    #morphological closing
    red = cv2.dilate(red,kernel,iterations=2)
    red = cv2.erode(red,kernel,iterations=1)

    yellow = cv2.dilate(yellow,kernel,iterations=2)
    yellow = cv2.erode(yellow,kernel,iterations=1)

    #Horizontal
    cv2.line(img,(0,160),(640,160),(255,255,255),2)
    cv2.line(img,(0,320),(640, 320),(255,255,255),2)

    #vertical
    cv2.line(img,(213,0),(213,480),(255,255,255),2)
    cv2.line(img,(426,0),(426,480),(255,255,255),2)
    
    #Center
    cv2.circle(img,(320,240),4,(255,255,255),-1)

    #Dropping
    (contours,hierarchy)=cv2.findContours(red,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    for pic, contour in enumerate(contours):
        area = cv2.contourArea(contour)
        if(area>200):           
            x,y,w,h = cv2.boundingRect(contour)
            img = cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255),2)
            M = cv2.moments(contour)
            cx = int(M['m10']/M['m00'])
            cy = int(M['m01']/M['m00'])
            
            cv2.circle(img,(cx,cy),4,(255,255,255),-1)
            cv2.line(img,(320,240),(cx, cy),(255,255,255), 1)
            cv2.putText(img,"Dropping area",(x,y),cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0,0,255))
            
            if(int(x) >= 0 and int(x) <= 213 and int(y) >= 0 and int(y) <= 159):
                print('Maju')
            elif(int(x) >= 213 and int(x) <= 426 and int(y) >= 0 and int(y) <= 159):
                print('Maju')
            elif(int(x) >= 426 and int(x) <= 640 and int(y) >= 0 and int(y) <= 159):
                print('Maju')
            elif(int(x) >= 0 and int(x) <= 213 and int(y) >= 159 and int(y) <= 318):
                print('Kiri')
            elif(int(x) >= 213 and int(x) <= 426 and int(y) >= 159 and int(y) <= 318):
                print('Dropping')
                # pi.set_servo_pulsewidth(17, 1500)
                # pi.set_servo_pulsewidth(27, 1500)
                # pi.set_servo_pulsewidth(22, 1500)
            elif(int(x) >= 426 and int(x) <= 640 and int(y) >= 159 and int(y) <= 318):
                print('Kanan')
            elif(int(x) >= 0 and int(x) <= 213 and int(y) >= 318 and int(y) <= 480):
                print('Mundur')
            elif(int(x) >= 213 and int(x) <= 426 and int(y) >= 318 and int(y) <= 480):
                print('Mundur')
            elif(int(x) >= 426 and int(x) <= 640 and int(y) >= 318 and int(y) <= 480):
                print('Mundur')
            else:
                print('Cari Lokasi')
            
    #Landing
    (contours,hierarchy)=cv2.findContours(yellow,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    for pic, contour in enumerate(contours):
        ((x,y),radius) = cv2.minEnclosingCircle(contour)
        if radius < 200 and radius >=0:
            #cv2.putText(img,shape,(cx,cy),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255))
            img = cv2.circle(img,(int(x),int(y)), int(radius),(20,106,255), 2)
            cv2.line(img,(320,240),(int(x), int(y)),(0,0,0), 1)
            cv2.circle(img,(int(x),int(y)),4,(0,0,0),-1)
            #cv2.putText(img,"Landing area",(int(x),int(y)-50),cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255,255,255))
            
            M = cv2.moments(contour)
            cx = int(M['m10']/M['m00'])
            cy = int(M['m01']/M['m00'])
            shape = "unidentified"
            peri = cv2.arcLength(contour, True)
            approx = cv2.approxPolyDP(contour, 0.04 * peri, True)
                
            if len(approx) == 3:
                shape = "triangle"

            elif len(approx) == 4:
                (x, y, w, h) = cv2.boundingRect(approx)
                ar = w / float(h)
                shape = "square" if ar >= 0.95 and ar <= 1.05 else "rectangle"
                cv2.putText(img,shape,(cx,cy),cv2.FONT_HERSHEY_SIMPLEX, 1, (255,255,255))
            
            elif len(approx) == 5:
                shape = "pentagon"

            else:
                shape = "Circle"
                cv2.putText(img,shape,(cx,cy),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255))
                img = cv2.circle(img,(int(x),int(y)), int(radius),(20,106,255), 2)
                cv2.line(img,(320,240),(int(x), int(y)),(0,0,0), 1)
                cv2.circle(img,(int(x),int(y)),4,(0,0,0),-1)
                #cv2.putText(img,"Landing disini",(int(x),int(y)-50),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255))
                cv2.putText(img,'Landing', (0,460), cv2.FONT_HERSHEY_TRIPLEX, 0.8, (0,0,0))

                
    cv2.imshow("Frame",img)
    cv2.imshow("Dropping",red)
    cv2.imshow("Landing",yellow)
    if cv2.waitKey(10) & 0xFF == ord('q'):
        # pi.set_servo_pulsewidth(17, 1000)
        # pi.set_servo_pulsewidth(27, 1000)
        # pi.set_servo_pulsewidth(22, 1000)
        cap.release()
        cv2.destroyAllWindows()
        break
