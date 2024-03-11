import time
from gpiozero import DistanceSensor

def read_ultrasonic_sensor():
    # Ganti GPIO_TRIGGER dan GPIO_ECHO dengan pin GPIO yang sesuai pada Jetson Nano
    sensor = DistanceSensor(echo=12, trigger=14)
    return sensor.distance * 100  # Konversi dari meter ke sentimeter

while True:

    obstacle_distance = read_ultrasonic_sensor()
    print(f"Jarak yang terdeteksi {obstacle_distance} m")
    time.sleep(1)