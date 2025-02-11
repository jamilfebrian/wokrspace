from dronekit import connect, VehicleMode, LocationGlobalRelative
import time
import math


# Fungsi untuk mengontrol lepas landas
def takeoff(vehicle, target_altitude):
    print("Lepas landas...")
    vehicle.mode = VehicleMode("GUIDED")
    vehicle.armed = True

    while not vehicle.armed:
        print("Menunggu inisialisasi arming.")
        time.sleep(1)

    vehicle.simple_takeoff(target_altitude)

    while True:
        current_altitude = vehicle.location.global_relative_frame.alt
        if current_altitude >= target_altitude * 0.95:
            print("Lepas landas berhasil!")
            break
        time.sleep(1)

# Fungsi untuk menggerakkan drone ke depan
def move_forward(vehicle, distance):
    print("Maju ke depan...")
    initial_location = vehicle.location.global_relative_frame
    target_location = get_location_metres(initial_location, 0, distance)

    vehicle.simple_goto(target_location)

    while vehicle.mode.name == "GUIDED":
        current_location = vehicle.location.global_relative_frame
        distance_to_target = get_distance_metres(current_location, target_location)

        if distance_to_target <= 0.1:
            print("Sudah mencapai tujuan.")
            break

        time.sleep(1)

# Fungsi untuk mendapatkan lokasi baru berdasarkan jarak dan bearing dari lokasi awal
def get_location_metres(original_location, dNorth, dEast):
    earth_radius = 6378137.0
    dLat = dNorth / earth_radius
    dLon = dEast / (earth_radius * math.cos(math.pi * original_location.lat / 180))

    new_lat = original_location.lat + (dLat * 180 / math.pi)
    new_lon = original_location.lon + (dLon * 180 / math.pi)

    return LocationGlobalRelative(new_lat, new_lon, original_location.alt)

# Fungsi untuk mendapatkan jarak antara dua lokasi dalam meter
def get_distance_metres(aLocation1, aLocation2):
    dLat = aLocation2.lat - aLocation1.lat
    dLon = aLocation2.lon - aLocation1.lon

    return math.sqrt((dLat * dLat) + (dLon * dLon)) * 1.113195e5

# Menentukan IP dan port koneksi drone
connection_string = "udp:127.0.0.1:14550"  # Ubah dengan IP dan port drone Anda

# Membuat koneksi ke drone
vehicle = connect(connection_string, wait_ready=True)

# Lepas landas ke ketinggian 5 meter
target_altitude = 5  # Ganti ketinggian sesuai kebutuhan Anda
takeoff(vehicle, target_altitude)

# Maju ke depan sejauh 5 meter
distance_to_move = 5  # Ganti jarak sesuai kebutuhan Anda
move_forward(vehicle, distance_to_move)

# Mendaratkan drone
print("Mendaratkan...")
vehicle.mode = VehicleMode("LAND")

# Menunggu drone benar-benar mendarat
while not vehicle.mode.name == "LAND":
    time.sleep(1)

# Menutup koneksi ke drone
vehicle.close()
print("Selesai")
