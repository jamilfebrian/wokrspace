import cv2

def main():
    # Mendapatkan akses ke kamera (0 untuk kamera internal, 1, 2, dll. untuk kamera eksternal jika ada)
    cap = cv2.VideoCapture(0)

    if not cap.isOpened():
        print("Kamera tidak dapat diakses.")
        return

    while True:
        # Membaca frame dari kamera
        ret, frame = cap.read()

        if not ret:
            print("Gagal membaca frame.")
            break

        # Menampilkan frame ke layar
        cv2.imshow("Kamera", frame)

        # Memberhentikan looping jika tombol 'q' ditekan
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Membebaskan sumber daya kamera dan menutup jendela tampilan
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
