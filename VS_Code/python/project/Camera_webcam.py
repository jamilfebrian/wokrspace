import cv2

x=0
nama=0

cap=cv2.VideoCapture(0)

while True:
    #variabel frame untuk read dari kamera
    ret,frame=cap.read()

    #untuk menampilkan frame / image
    #'frame' untuk nama window, bisa diganti dengan nama apapun
    cv2.imshow('frame', frame)
    k=cv2.waitKey(1)& 0xFF
    
    if k==32:   #32 = id untuk menekan spasi
        print('spasi telah ditekan')
        x=1
        if x==1:

            #capture image dan menyimpannya dalam direktori tertentu
            cv2.imwrite('img/'+str(nama)+'.jpg',frame)
            print(str(nama)+'.jpg telah tersimpan')
            nama+=1
        x=0

    if k==27:   #27 = id untuk menekan ESC
        print('esc telah ditekan')

        
        cap.release()
        cv2.destroyAllWindows()
        break
