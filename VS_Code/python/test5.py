import time
from dronekit import connect, VehicleMode, LocationGlobalRelative
from gpiozero import DistanceSensor

# Fungsi untuk menginisialisasi koneksi dengan drone
def connect_drone():
    connection_string = "/dev/ttyUSB0"  # Ganti dengan string koneksi yang sesuai
    return connect(connection_string, wait_ready=True)

# Fungsi untuk membaca data dari sensor ultrasonik
def read_ultrasonic_sensor():
    # Ganti GPIO_TRIGGER dan GPIO_ECHO dengan pin GPIO yang sesuai pada Jetson Nano
    sensor = DistanceSensor(echo=12, trigger=14)
    return sensor.distance * 100  # Konversi dari meter ke sentimeter

# Fungsi untuk menghindari hambatan
def avoid_obstacle(vehicle):
    obstacle_distance_threshold = 50  # Jarak hambatan dalam sentimeter
    while True:
        obstacle_distance = read_ultrasonic_sensor()
        if obstacle_distance <= obstacle_distance_threshold:
            print("Obstacle detected! Stopping the drone.")
            vehicle.mode = VehicleMode("BRAKE")  # Mode darurat untuk menghentikan drone
            time.sleep(2)  # Hentikan drone selama 2 detik
            vehicle.mode = VehicleMode("GUIDED")  # Kembalikan ke mode GUIDED
            time.sleep(2)  # Tunggu selama 2 detik sebelum melanjutkan
        time.sleep(0.5)  # Baca sensor setiap 0.5 detik

def main():
    vehicle = connect_drone()
    vehicle.mode = VehicleMode("GUIDED")  # Ganti mode ke GUIDED

    try:
        avoid_obstacle(vehicle)
    except KeyboardInterrupt:
        print("Program berakhir.")
    finally:
        vehicle.close()

if __name__ == "__main__":
    main()
