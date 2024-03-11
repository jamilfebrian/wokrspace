from dronekit import connect, VehicleMode, LocationGlobalRelative
import time
import math

# Koneksi ke drone
connection_string = '/dev/ttyTHS1'  # Ganti dengan koneksi yang sesuai
baud_rate = 921600
vehicle = connect(connection_string,baud=baud_rate, wait_ready=True)

# Fungsi untuk menggerakkan drone maju
def move_forward(distance):
    # Menghitung lokasi target berdasarkan posisi dan orientasi saat ini
    current_location = vehicle.location.global_relative_frame
    target_location = get_location_metres(current_location, distance, 0)

    # Menggerakkan drone menuju lokasi target
    vehicle.simple_goto(target_location)

    # Menunggu hingga drone mencapai lokasi target
    while not is_at_target(target_location):
        time.sleep(1)

# Fungsi untuk memeriksa apakah drone telah mencapai target
def is_at_target(target_location):
    current_location = vehicle.location.global_relative_frame
    distance = get_distance_metres(current_location, target_location)
    return distance < 1  # Jarak threshold untuk menentukan apakah sudah mencapai target

# Fungsi untuk menghitung lokasi dalam meter berdasarkan jarak dan arah relatif terhadap titik awal
def get_location_metres(original_location, dNorth, dEast):
    earth_radius = 6378137.0  # Radius bumi dalam meter
    new_latitude = original_location.lat + (dNorth / earth_radius) * (180 / math.pi)
    new_longitude = original_location.lon + (dEast / (earth_radius * math.cos(math.pi * original_location.lat / 180))) * (
            180 / math.pi)
    target_location = LocationGlobalRelative(new_latitude, new_longitude, original_location.alt)
    return target_location

# Fungsi untuk menghitung jarak antara dua lokasi dalam meter
def get_distance_metres(location1, location2):
    dlat = location2.lat - location1.lat
    dlong = location2.lon - location1.lon
    return math.sqrt((dlat * dlat) + (dlong * dlong)) * 1.113195e5

# Menunggu hingga drone siap terbang
while not vehicle.is_armable:
    time.sleep(1)

# Mengubah mode ke GUIDED (untuk mengontrol secara manual)
vehicle.mode = VehicleMode("GUIDED")
while not vehicle.mode.name == "GUIDED":
    time.sleep(1)

# Arm drone
vehicle.armed = True
while not vehicle.armed:
    time.sleep(1)

# Menggerakkan drone maju 5 meter
move_forward(5)

# Menunggu beberapa detik agar drone mencapai tujuan
time.sleep(5)

# Menghentikan gerakan drone
vehicle.mode = VehicleMode("LAND")

# Menunggu hingga drone mendarat dan mematikan mesin
while vehicle.armed:
    time.sleep(1)

# Disconnect dari drone
vehicle.disconnect()