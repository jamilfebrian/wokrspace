import cv2

cap = cv2.VideoCapture(0)

# Mengatur resolusi dan frame rate video
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
cap.set(cv2.CAP_PROP_FPS, 30)

# Mengambil informasi tentang video yang diambil
width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
fps = int(cap.get(cv2.CAP_PROP_FPS))

# Membuat objek video writer
fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('output.mp4', fourcc, fps, (width, height))

while True:
    # Membaca frame dari video capture
    ret, frame = cap.read()

    # Menampilkan frame di layar
    cv2.imshow('frame', frame)

    # Menyimpan frame ke video writer
    out.write(frame)

    # Keluar dari loop jika tombol 'q' ditekan
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Melepas semua objek dan menutup jendela
cap.release()
out.release()
cv2.destroyAllWindows()