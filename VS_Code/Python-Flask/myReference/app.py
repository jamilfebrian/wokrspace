from flask import Flask, render_template, request
import Jetson.GPIO as GPIO
import time

# Inisialisasi Flask
app = Flask(__name__)

# Inisialisasi GPIO
relay_pin = 18  # Ganti dengan pin GPIO yang Anda gunakan untuk relay
GPIO.setmode(GPIO.BOARD)  # Gunakan penomoran pin papan
GPIO.setup(relay_pin, GPIO.OUT)  # Set pin sebagai output

# Fungsi untuk mengontrol relay
def relay_on():
    GPIO.output(relay_pin, GPIO.LOW)  # Relay menyala (aktif rendah)

def relay_off():
    GPIO.output(relay_pin, GPIO.HIGH)  # Relay mati

# Rute untuk halaman utama
@app.route('/')
def index():
    return render_template('index.html')

# Rute untuk mengontrol relay
@app.route('/relay', methods=['POST'])
def control_relay():
    if request.form['action'] == 'on':
        relay_on()
    elif request.form['action'] == 'off':
        relay_off()
    return render_template('index.html')

# Menjalankan server Flask
if __name__ == '__main__':
    try:
        relay_off()  # Pastikan relay dalam keadaan mati saat memulai
        app.run(host='0.0.0.0', port=5000)  # Jalankan server pada semua antarmuka jaringan
    finally:
        GPIO.cleanup()  # Membersihkan pengaturan GPIO
