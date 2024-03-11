from pymavlink import mavutil
from dronekit import connect, VehicleMode, LocationGlobalRelative
import time

def send_body_ned_velocity(velocity_x, velocity_y, velocity_z, duration=0):
    msg = vehicle.message_factory.set_position_target_local_ned_encode(
        0,       # time_boot_ms (not used)
        0, 0,    # target system, target component
        mavutil.mavlink.MAV_FRAME_BODY_NED, # frame Needs to be MAV_FRAME_BODY_NED for forward/back left/right control.
        0b0000111111000111, # type_mask
        0, 0, 0, # x, y, z positions (not used)
        velocity_x, velocity_y, velocity_z, # m/s
        0, 0, 0, # x, y, z acceleration
        0, 0)
    for x in range(0,duration):
        vehicle.send_mavlink(msg)
        time.sleep(1)

connection_string = 'udp:127.0.0.1:14550' # Edit to suit your needs.
takeoff_alt = 3
vehicle = connect(connection_string, wait_ready=True)
while not vehicle.is_armable:
    time.sleep(1)
vehicle.mode = VehicleMode("GUIDED")
vehicle.armed = True
while not vehicle.armed:
    print('Waiting for arming...')
    time.sleep(1)
vehicle.simple_takeoff(takeoff_alt) # Take off to target altitude
while True:
    print('Altitude: %d' % vehicle.location.global_relative_frame.alt)
    if vehicle.location.global_relative_frame.alt >= takeoff_alt * 0.95:
        print('REACHED TARGET ALTITUDE')
        break
    time.sleep(1)

send_body_ned_velocity(5, 0, 0, 1)
time.sleep(10)

vehicle.mode = VehicleMode("LAND")