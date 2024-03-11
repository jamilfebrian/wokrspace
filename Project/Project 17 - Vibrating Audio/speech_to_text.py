import speech_recognition as sr

# Inisialisasi objek Recognizer
recognizer = sr.Recognizer()

# Fungsi untuk mengonversi suara menjadi teks
def speech_to_text():
    with sr.Microphone() as source:
        print("Silakan bicara sesuatu...")
        recognizer.adjust_for_ambient_noise(source)  # Menghilangkan noise dari mikrofon
        audio = recognizer.listen(source, timeout=5)  # Mendengarkan audio selama 5 detik (atau sesuai kebutuhan)

    try:
        text = recognizer.recognize_google(audio, language="id-ID")  # Menggunakan Google Web Speech API
        print(f"Teks yang terdeteksi: {text}")
    except sr.UnknownValueError:
        print("Maaf, tidak dapat mengenali suara.")
    except sr.RequestError as e:
        print("Terjadi kesalahan pada permintaan: {0}".format(e))

if __name__ == "__main__":
    speech_to_text()
