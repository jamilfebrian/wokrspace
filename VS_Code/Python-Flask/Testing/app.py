from flask import Flask, render_template, request

app = Flask(__name__)

# Halaman utama dengan button
@app.route('/', methods=['GET', 'POST'])
def index():
    button_text = "Tekan Saya"
    if request.method == 'POST':
        button_text = "Sudah Ditekan"
    return render_template('index.html', button_text=button_text)

if __name__ == '__main__':
    app.run(debug=True)
