import cv2
import numpy as np

class RedPointDetector:
    def __init__(self):
        # Inisialisasi nilai batas bawah dan atas untuk warna merah
        self.low_red = np.array([100, 100, 100])
        self.high_red = np.array([179, 255, 255])
        
    def detect(self, frame):
        # Ubah citra ke dalam ruang warna HSV
        hsv_img = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        # Buat masker menggunakan rentang warna merah
        mask = cv2.inRange(hsv_img, self.low_red, self.high_red)
        # Temukan kontur dari objek yang terdeteksi
        contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        # Urutkan kontur berdasarkan luasnya dalam urutan menurun
        contours = sorted(contours, key=lambda x: cv2.contourArea(x), reverse=True)

        # Inisialisasi bounding box dengan nilai default
        box = (-100, -100, 0, 0)
        for cnt in contours:
            # Dapatkan koordinat bounding box
            (x, y, w, h) = cv2.boundingRect(cnt)
            # Jika luas bounding box lebih besar dari 300, update nilai box
            if w * h > 300:
                box = (x, y, x+w, y+h)
                break
        return box

class KalmanFilter:
    kf = cv2.KalmanFilter(4, 2)
    kf.measurementMatrix = np.array([[1, 0, 0, 0], [0, 1, 0, 0]], np.float32)
    kf.transitionMatrix = np.array([[1, 0, 1, 0], [0, 1, 0, 1], [0, 0, 1, 0], [0, 0, 0, 1]], np.float32)

    def predict(self, coordx, coordy):
        # Fungsi ini memperkirakan posisi objek berdasarkan posisi sebelumnya
        measured = np.array([[np.float32(coordx)], [np.float32(coordy)]])
        # Koreksi prediksi dengan nilai pengukuran aktual
        self.kf.correct(measured)
        # Lakukan prediksi berikutnya berdasarkan model Kalman
        predicted = self.kf.predict()
        # Ambil koordinat x dan y yang telah diprediksi
        x, y = int(predicted[0]), int(predicted[1])
        return x, y

# Inisialisasi detektor objek dan filter Kalman
od = RedPointDetector()
kf = KalmanFilter()

# Buka video
cap = cv2.VideoCapture("gerak4.mp4")

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Deteksi area merah pada frame saat ini
    redpoint_bbox = od.detect(frame)
    x, y, x2, y2 = redpoint_bbox
    cx = int((x + x2) / 2)
    cy = int((y + y2) / 2)

    # Prediksi pergerakan objek
    predicted = kf.predict(cx, cy)
    
    # Tampilkan lingkaran merah pada area deteksi
    cv2.circle(frame, (cx, cy), 10, (0, 0, 255), -1)
    # Tampilkan lingkaran kuning pada prediksi pergerakan
    cv2.circle(frame, (predicted[0], predicted[1]), 10, (0, 255, 255), 4)
    
    # Tampilkan frame yang telah dimodifikasi
    cv2.imshow("Frame", frame)

    # Tunggu tombol 'Esc' ditekan untuk keluar
    key = cv2.waitKey(10)
    if key == 27:
        break

# Bebaskan sumber daya dan tutup jendela tampilan
cap.release()
cv2.destroyAllWindows()