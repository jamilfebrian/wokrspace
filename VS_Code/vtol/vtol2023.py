from __future__ import print_function
import time
from dronekit import connect, VehicleMode, LocationGlobalRelative
from pymavlink import mavutil
# from color_detection import camera
import threading

import argparse
parser = argparse.ArgumentParser(description='Commands vehicle using vehicle.simple_goto.')
parser.add_argument('--connect',default="udp:127.0.0.1:14550",
                    help="Vehicle connection target string. If not specified, SITL automatically started and used.")
args = parser.parse_args()
connection_string = args.connect

print('Connecting to vehicle on: %s' % connection_string)
vehicle = connect(connection_string, wait_ready=True,baud=921600)

# threading.Thread(target=camera, daemon=True).start()


def arm_and_takeoff(aTargetAltitude):
    print("Basic pre-arm checks")
    while not vehicle.is_armable:
        print(" Waiting for vehicle to initialise...")
        time.sleep(1)

    print("Arming motors")
    vehicle.mode = VehicleMode("GUIDED")
    vehicle.armed = True

    while not vehicle.armed:
        print(" Menunggu Arming Motor...")
        time.sleep(1)

    print("Tabang!")
    vehicle.simple_takeoff(aTargetAltitude)  
    while True:
        print(" Altitude: ", vehicle.location.global_relative_frame.alt)
        if vehicle.location.global_relative_frame.alt >= aTargetAltitude * 0.95:
            print("Reached target altitude")
            break
        time.sleep(1)

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

def condition_yaw(heading, relative=False):
    if relative:
        is_relative=1 #yaw relative to direction of travel
    else:
        is_relative=0 #yaw is an absolute angle
    # create the CONDITION_YAW command using command_long_encode()
    msg = vehicle.message_factory.command_long_encode(
        0, 0,    # target system, target component
        mavutil.mavlink.MAV_CMD_CONDITION_YAW, #command
        0, #confirmation
        heading,    # param 1, yaw in degrees
        0,          # param 2, yaw speed deg/s
        1,          # param 3, direction -1 ccw, 1 cw
        is_relative, # param 4, relative offset 1, absolute angle 0
        0, 0, 0)    # param 5 ~ 7 not used
    # send command to vehicle
    vehicle.send_mavlink(msg)

arm_and_takeoff(5)

print("Set default/target airspeed to 3")
vehicle.airspeed = 3
vehicle.groundspeed = 3


send_body_ned_velocity(5, 0, 0, 1)
time.sleep(10)
condition_yaw(90, relative=True)
time.sleep(5)
send_body_ned_velocity(5, 0, 0, 1)
time.sleep(10)

# a.join()
print("Returning to Launch")
vehicle.mode = VehicleMode("LAND")

# Close vehicle object before exiting script
print("Close vehicle object")
vehicle.close()
