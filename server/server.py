from flask import Flask, request, render_template, redirect, url_for
import os
from datetime import datetime
from disease_detection import prediction
import pandas as pd

app = Flask(__name__)

disease_info = pd.read_csv('disease_info.csv' , encoding='cp1252')

@app.route('/favicon.ico')
def favicon():
    return '', 204

@app.route('/')
def index():
    return redirect('/home')

@app.route('/home')
def home_page():
    return render_template('index.html')


@app.route('/home/images')
def plant_menu_page():
    image_list = os.listdir('./static/images/')
    image_list.sort(reverse=True)
    return render_template('image_menu.html', images=image_list)


@app.route('/home/images/<filename>')
def plant_data_page(filename):
    output_prediction = disease_info['disease_name'][prediction('./static/images/' + filename)]
    return render_template('image_data.html', image_filename=filename, disease_prediction=output_prediction)


@app.route('/static/images', methods=['POST'])
def receive_images():
    if request.data is not None:
        filename = datetime.now().strftime('%Y-%m-%d_%H:%M:%S.jpg')
        print(f"Menerima gambar {filename}...")
        with open(f"./static/images/{filename}", "wb") as file:
            file.write(request.data)

        return '[OK] Gambar berhasil disimpan', 200

    return '[ERROR] Gambar tidak dapat disimpan', 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port = 5000)
