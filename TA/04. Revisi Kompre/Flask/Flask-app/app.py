from flask import Flask, render_template
from flask_socketio import SocketIO, emit
import random
import time
from threading import Thread

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)

def generate_temperature():
    while True:
        # Simulasi pembacaan data suhu
        temperature = round(random.uniform(20.0, 30.0), 2)
        pressure = round(random.uniform(40.0, 50.0), 2)
        sprayer = "spray"
        socketio.emit('temperature_update', 
                      {'temperature': temperature, 
                       "pressure" : pressure, 
                       "sprayer": sprayer
                       })
        time.sleep(1)  # Kirim data setiap 2 detik

@app.route('/')
def index():
    return render_template('index.html')

@socketio.on('connect')
def handle_connect():
    print('Client connected')

if __name__ == '__main__':
    thread = Thread(target=generate_temperature)
    thread.start()
    socketio.run(app, host='0.0.0.0', port=5000, debug=True)
