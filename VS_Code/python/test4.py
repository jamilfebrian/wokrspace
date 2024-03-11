import speech_recognition as sr

# Membuat objek recognizer dari SpeechRecognition
recognizer = sr.Recognizer()

# Merekam audio dari mikrofon
with sr.Microphone() as source:
    print("Silakan bicara...")
    audio = recognizer.listen(source)
    print("Perekaman selesai.")

# Mengenali teks dari audio
try:
    recognized_text = recognizer.recognize_google(audio)
    print("Teks yang dikenali:", recognized_text)

    # Mengubah teks menjadi array kata-kata
    kata_kata = recognized_text.split()
    print("Array kata-kata:", kata_kata)

except sr.UnknownValueError:
    print("Maaf, tidak dapat mengenali suara.")
except sr.RequestError as e:
    print(f"Terjadi kesalahan dalam permintaan: {e}")
