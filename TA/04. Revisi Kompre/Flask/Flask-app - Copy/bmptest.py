import time
from smbus2 import SMBus
from bmp280 import BMP280

# Membuka bus I2C
bus = SMBus(1)  # Bus I2C default di Jetson Nano adalah 1

try:
    # Inisialisasi sensor BMP280
    bmp280 = BMP280(i2c_dev=bus)

    print("Membaca data dari sensor BMP280...")
    while True:
        # Membaca suhu
        suhu = bmp280.get_temperature()
        # Membaca tekanan udara
        tekanan = bmp280.get_pressure()

        ketinggian = 44330 * (1 - (tekanan / 1013.25) ** 0.1903)

        # Menampilkan data
        print(f"Suhu: {suhu:.2f} °C")
        print(f"Tekanan: {tekanan:.2f} hPa")
        print("----------------------------")
        
        # Delay 1 detik
        time.sleep(1)

except Exception as e:
    print(f"Terjadi kesalahan: {e}")

finally:
    bus.close()
