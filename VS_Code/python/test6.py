import firebase_admin
from firebase_admin import credentials, db

# Inisialisasi koneksi ke Firebase
cred = credentials.Certificate("credential.json")
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://project-test-19065010-default-rtdb.asia-southeast1.firebasedatabase.app/'
})

# Fungsi untuk mengirim data ke Firebase
def send_data_to_firebase(data):
    try:
        # Dapatkan referensi ke database Firebase
        ref = db.reference('/esp32')  # Ganti dengan path data di Firebase

        # Kirim data ke Firebase
        ref.push().set(data)

        print('Data berhasil dikirim ke Firebase')
    except Exception as e:
        print('Gagal mengirim data ke Firebase. Kesalahan:', str(e))

# Contoh data yang akan dikirimkan ke Firebase
sample_data = {
    'suhu': 91,
    'humy': 90,
    'speed': 92
}

# Panggil fungsi untuk mengirim data ke Firebase
send_data_to_firebase(sample_data)
