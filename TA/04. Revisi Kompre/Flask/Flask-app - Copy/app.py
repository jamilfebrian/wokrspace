from flask import Flask, render_template, request
from flask_socketio import SocketIO, emit
from dronekit import connect, VehicleMode
from pymavlink import mavutil
from threading import Thread
import Jetson.GPIO as GPIO
import time
from smbus2 import SMBus
from bmp280 import BMP280

connection_string = '/dev/ttyAMA0' 
baud_rate = 921600 

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)

relay_pin = 18  
GPIO.setmode(GPIO.BOARD)  
GPIO.setup(relay_pin, GPIO.OUT)

koneksi = "Connect"
armStatus = "Disarmed"
status = "OFF"

logicConnect = False
logicArm = False
logicStatus = False

bus = SMBus(1) 

def relay_on():
    GPIO.output(relay_pin, GPIO.LOW) 

def relay_off():
    GPIO.output(relay_pin, GPIO.HIGH)  

def generate_temperature():
    bmp280 = BMP280(i2c_dev=bus)

    while True:
        temperature = round(bmp280.get_temperature(),2)
        pressure = round(bmp280.get_pressure(),2)
        altitude = round(44330 * (1 - (pressure / 1013.25) ** 0.1903),2)
        yaw = "180°"
        groundSpeed = 1.8
        socketio.emit('temperature_update', 
                      {'temperature': temperature, 
                       "pressure" : pressure, 
                       "yaw": yaw,
                       "altitude": altitude,
                       "groundSpeed" : groundSpeed,
                       })
        time.sleep(1)

@app.route('/')
def index():
    return render_template('index.html', status=status, koneksi=koneksi, armStatus=armStatus)

@app.route('/connect', methods=['POST'])
def command():
    global koneksi
    global logicConnect

    if request.form['connect'] == 'connect':
        logicConnect = not logicConnect
        if logicConnect == True :
            koneksi = "Connect"
        else :
            koneksi = "Disconnect"
    return render_template('index.html', status=status, koneksi=koneksi, armStatus=armStatus)

@app.route('/armed', methods=['POST'])
def armed():
    global armStatus
    global logicArm

    if request.form['armed'] == 'armed':
        logicArm = not logicArm
        if logicArm == True :
            armStatus = "Armed"
        else :
            armStatus = "Disarmed"
    return render_template('index.html', status=status, koneksi=koneksi, armStatus=armStatus)

@app.route('/spray', methods=['POST'])
def control_relay():
    global logicStatus
    global status
    if request.form['sprayer'] == 'on':
        logicStatus = not logicStatus
        if logicStatus==True :
            status = "ON"
            relay_on()
        else :
            status = "OFF"
            relay_off()

    return render_template('index.html', status=status, koneksi=koneksi, armStatus=armStatus)

@app.route('/kirim', methods=['POST'])
def kirim():
    global logicStatus
    global status

    if request.form['kirim'] == 'on':
        logicStatus = not logicStatus
        time.sleep(int(request.form['jarak']))
        if logicStatus==True :
            status = "ON"
            relay_on()
        else :
            status = "OFF"
            relay_off()

    return render_template('index.html', status=status, koneksi=koneksi, armStatus=armStatus)


@socketio.on('connect')
def handle_connect():
    print('Client connected')

if __name__ == '__main__':
    thread = Thread(target=generate_temperature)
    thread.start()
    socketio.run(app, host='0.0.0.0', port=5000, debug=True)
