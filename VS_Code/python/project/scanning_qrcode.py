import cv2
import numpy as np
from pyzbar.pyzbar import decode

# img = cv2.imread('D:\\Jamil\\VSS_code\\test.png')
cap = cv2.VideoCapture(0)
cap.set(3,640)
cap.set(4,480)
# code = decode(img)
# print(code)
while True :
    succer, img1 = cap.read()
    for barcode in decode(img1):
        myData = barcode.data.decode('utf-8')
        print(myData)
        points = np.array([barcode.polygon],np.int32)
        points = points.reshape((-1,1,2))
        cv2.polylines(img1, [points],True,(255,0,0),5)
        points2 = barcode.rect
        cv2.putText(img1,myData,(points2[0],points2[1]),cv2.FONT_HERSHEY_SIMPLEX,
                    0.9,(255,0,255),2)

    cv2.imshow("Result", img1)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

