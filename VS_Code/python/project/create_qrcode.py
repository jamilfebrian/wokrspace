# import numpy as np
# from pyzbar.pyzbar import decode
# import cv2

# img = cv2.imread("qr1.png")
# code = decode(img)
# print(code)
# for barcode in decode(img):
#     print(barcode.data)
#     text = barcode.data.decode('utf-8')
#     print(text)
#     print(barcode.rect)

import qrcode 
# Link for website
input_data = "Mantes see"
#Creating an instance of qrcode
qr = qrcode.QRCode(
        version=1,
        box_size=10,
        border=5)

qr.add_data(input_data)
qr.make(fit=True)
img = qr.make_image(fill='black', back_color='white')
img.save('test.png') 

