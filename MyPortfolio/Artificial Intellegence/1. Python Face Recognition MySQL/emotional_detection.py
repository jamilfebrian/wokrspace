import cv2
from fer import FER
import matplotlib.pyplot as plt

# Memuat classifier Haarcascade untuk deteksi wajah
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Membuka webcam
cap = cv2.VideoCapture(0)

# Inisialisasi detektor emosi
detector = FER()

while True:
    # Membaca frame dari webcam
    ret, frame = cap.read()

    if not ret:
        print("Tidak dapat menerima frame (stream berakhir?). Exiting ...")
        break

    # Mengubah frame menjadi skala abu-abu (grayscale)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Mendeteksi wajah dalam frame
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    # Menggambar bounding box di sekitar wajah dan mendeteksi emosi
    for (x, y, w, h) in faces:
        # Gambar bounding box
        cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)

        # Ekstraksi wajah untuk deteksi emosi
        face_roi = frame[y:y+h, x:x+w]

        # Gunakan FER untuk mendeteksi emosi
        emotion = detector.detect_emotions(face_roi)
        
        if emotion:
            # Mendapatkan emosi dengan confidence terbesar
            top_emotion = max(emotion[0]["emotions"], key=emotion[0]["emotions"].get)
            
            # Tampilkan emosi di atas bounding box
            cv2.putText(frame, top_emotion, (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (36,255,12), 2)
            
            # Deteksi apakah orang senang atau marah
            if top_emotion == 'happy':
                print("Emosi: Senang")
            elif top_emotion == 'angry':
                print("Emosi: Marah")

    # Tampilkan frame dengan bounding box dan emosi
    cv2.imshow('Face Recognition and Emotion Detection', frame)

    # Tekan 'q' untuk keluar dari loop
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Membersihkan resource
cap.release()
cv2.destroyAllWindows()
