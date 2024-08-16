import requests
import json

BOT_TOKEN = "5346287582:AAFyl0CBY_hr5PJDFeCJMg9X0LwKOV-RP2s"
chat_id = '1599903092'
text = "Text yang saya kirim"

def send_message(chat_id, text, reply_markup=None):
    url = f"https://api.telegram.org/bot{BOT_TOKEN}/sendMessage"
    data = {"chat_id": chat_id, "text": text}
    if reply_markup:
        data["reply_markup"] = reply_markup
    response = requests.post(url, data=data)
    return response.json()

feedback_keyboard = json.dumps({
    "inline_keyboard": [
        [
            {"text": "Sangat Baik", "callback_data": "feedback_sb"},
            {"text": "Baik", "callback_data": "feedback_b"}
        ],
        [
            {"text": "Cukup", "callback_data": "feedback_c"},
            {"text": "Kurang", "callback_data": "feedback_k"}
        ]
    ]
})

def handle_callback(update):
    query = update["callback_query"]
    data = query["data"]
    chat_id = query["message"]["chat"]["id"]
    message_id = query["message"]["message_id"]
    text = f"Terima kasih atas umpan balik Anda: {data}"
    send_message(chat_id, text)
    # tambahkan kode lainnya sesuai kebutuhan

def handle_request(request):
    update = json.loads(request.data.decode("utf-8"))
    if "message" in update:
        message = update["message"]
        chat_id = message["chat"]["id"]
        text = message.get("text", "")
        if text == "/feedback":
            send_message(chat_id, "Berikan umpan balik Anda:",
                         feedback_keyboard)
        elif "callback_query" in update:
            handle_callback(update)

