import requests
import json

logic = 6
bot_token = '5346287582:AAFyl0CBY_hr5PJDFeCJMg9X0LwKOV-RP2s'
chat_id = '1599903092'

def kirimPhoto():
    path_file = 'D:\\Jamil\\Picture\\Wallpaper\\2-gambar-itachi-dan-bulan-dibelakangnya.jpg'
    url   = f'https://api.telegram.org/bot{bot_token}/sendSticker'
    files = {'sticker': open(path_file, 'rb')}
    data  = {'chat_id': chat_id , 'caption': 'testing kirim poto'}
    response = requests.post(url, files=files, data=data)
    if response.status_code == 200:
        print('Foto telah berhasil dikirim!')
    else:
        print('Gagal mengirim foto. Status code: ', response.status_code)

def kirimLokasi():
    latitude = -0.894861
    longitude = 100.350207
    url = f'https://api.telegram.org/bot{bot_token}/sendLocation'
    data = {
        'chat_id': chat_id,
        'latitude': latitude,
        'longitude': longitude,
        }
    response = requests.post(url, data=data)
    if response.status_code == 200:
        print("Lokasi telah terkirimkan")
    else:
        print('Gagal mengirim Lokasi. Status code: ', response.status_code)

def kirimVideo():
    videoPath = "D:\\Jamil\\VSS_code\\output.mp4"
    url = f'https://api.telegram.org/bot{bot_token}/sendVideo'
    files = {'video':  open(videoPath, 'rb')}
    data = {'chat_id': chat_id, 'caption': "Video Anda"}
    response = requests.post(url, data=data, files=files)
    if response.status_code != 200:
        print('Gagal mengirim video ke Telegram:', response.text)
    else:
        print('Video berhasil dikirim ke Telegram!')

def kirimAudio():
    videoPath = "D:\\Armed.wav"
    url = f'https://api.telegram.org/bot{bot_token}/sendAudio'
    files = {'audio':  open(videoPath, 'rb')}
    data = {'chat_id': chat_id, 'caption': "Audio Anda"}
    response = requests.post(url, data=data, files=files)
    if response.status_code != 200:
        print('Gagal mengirim Audio ke Telegram:', response.text)
    else:
        print('Audui berhasil dikirim ke Telegram!')

def kirimPesan():
    URL = f'https://api.telegram.org/bot{bot_token}/sendMessage'
    message = "Ditemukan korban bencana pada lokasi diatas"
    data = {
        "chat_id": chat_id,
        "text": message,
    }
    response = requests.post(URL, data=data)
    print("Pesan berhasil diteruskan!")

def teruskanPesan():
    URL_GET_UPDATE = f"https://api.telegram.org/bot{bot_token}/getUpdates"
    URL = f"https://api.telegram.org/bot{bot_token}/sendMessage"

    response = requests.get(URL_GET_UPDATE)
    dataUpdate = json.loads(response.content.decode('utf8'))
    message_id = dataUpdate['result'][-1]['message']['message_id']
    textMessage = dataUpdate['result'][-1]['message']['text']
    if textMessage=="bro":
        data = {
        "chat_id": chat_id,
        "text" : f"Bagak apak ang?",
        "reply_to_message_id": message_id
        }
    else : 
        data = {
        "chat_id": chat_id,
        "text" : f"Anda mengetikkan {textMessage}",
        "reply_to_message_id": message_id
        }
    requests.post(URL, data=data)

if logic==1:
    kirimLokasi()
    kirimPesan()
elif logic==2:
    kirimPhoto()
elif logic==3:
    kirimVideo()
elif logic==4:
    kirimAudio()
elif logic==5:
    kirimPesan()
elif logic==6:
    teruskanPesan()
