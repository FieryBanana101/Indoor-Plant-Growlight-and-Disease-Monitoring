
## Indoor Plant Growlight and-Disease Monitoring


### Kelompok 2 - K02 - WI2001 

> Muhammad Fatih Irkham Mauludi 	(13524004)  
> Josh Reinhart Zidik 			    (13524048)  
> Dzaki Ahmad Al Hussainy 		    (13524084)  
> Kloce Paul William Saragih 		(13524040)  
> Muhammad Nafis Habibi			    (13524018)  


Sistem monitoring penyakit pada tanaman yang ditambah dengan grow light spektrum merah-biru dengan periode tertentu untuk imitasi cahaya matahari.

Digunakan mikrokontroller ESP32 untuk pengaturan grow light, dan ESP32-CAM untuk monitoring penyakit tanaman. Deteksi penyakit dilakukan dengan model yang diambil dari: 

https://github.com/manthan89-py/Plant-Disease-Detection.


Demonstrasi penggunaan dapat dilihat pada laporan tugas besar, yang terletak pada direktori "doc/".


### Setup Penggunaan

Sebelum dilakukan setup, pastikan seluruh hardware yang dibutuhkan sudah ada dan telah disusun seperti rangkaian pada laporan. Dibutuhkan python, terutama versi 3 (atau lebih).


1.) Clone repositori ini
```bash
git clone https://github.com/FieryBanana101/Indoor-Plant-Growlight-and-Disease-Monitoring
```

2.) Install seluruh python module yang dibutuhkan
```bash
pip install -r requirements.txt
```

3.) Download model yang sudah dilatih pada link berikut ini, kemudian pindahkan file model tersebut ke dalam folder "server/".
```bash
https://drive.google.com/drive/folders/1ewJWAiduGuld_9oGSrTuLumg9y62qS6A
```
**Notes:** Model tersebut berasal dari repositori https://github.com/manthan89-py/Plant-Disease-Detection


4.) Sesuaikan SSID dan Password WIFI pada setiap script .ino untuk ESP32 dan ESP32-CAM
```c
/* WIFI Credential */
const char *ssid = "***"; // Silahkan isi ini dengan ID WIFI 
const char *password = "**********************************"; // Silahkan isi ini dengan password WIFI
```
Kemudian upload kedua script tersebut ke ESP32 dan ESP32-CAM melalui Arduino-IDE  
**Notes:** Pastikan bahwa sudah diinstall driver ESP32 untuk Arduino-IDE, seperti panduan pada dokumentasi https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html

5.) Jalankan server dengan cara masuk ke folder "server/", kemudian jalankan "server.py" dengan python:
```bash
python3 server.py
```

6.) Hubungkan semua power source komponen dan nyalakan wifi yang sesuai seperti di kode ESP32.


Setelah selesai, alat akan berjalan secara otomatis dan akan mengambil gambar setiap 30 menit, serta mengubah warna grow light secara periodik.


### Struktur Direktori

```bash
├── docs
│   ├── Kelompok 2 - Sistem monitoring tanaman.pdf
│   └── Laporan Tugas Besar PRD Kelompok 2.pdf
├── hardware
│   ├── ESP32-CAM_plant_monitor
│   │   ├── app_httpd.cpp
│   │   ├── camera_index.h
│   │   ├── camera_pins.h
│   │   ├── ci.json
│   │   ├── ESP32-CAM_plant_monitor.ino
│   │   └── partitions.csv
│   └── ESP32_control_grow_light
│       ├── ci.json
│       └── ESP32_control_grow_light.ino
├── README.md
├── requirements.txt
└── server
    ├── CNN.py
    ├── disease_detection.py
    ├── disease_info.csv
    ├── plant_disease_model_1_latest.pt
    ├── __pycache__
    │   ├── CNN.cpython-312.pyc
    │   └── disease_detection.cpython-312.pyc
    ├── server.py
    ├── static
    │   ├── favicon.ico
    │   └── images
    │       ├── 2025-05-26_13:22:45.jpg
    │       ├── 2025-05-26_13:22:55.jpg
    │       ├── 2025-05-26_13:23:06.jpg
    │       ├── 2025-05-26_13:23:16.jpg
    │       ├── 2025-05-26_13:23:26.jpg
    │       ├── 2025-05-26_13:23:37.jpg
    │       ├── 2025-05-26_13:23:47.jpg
    │       ├── 2025-05-26_13:23:59.jpg
    │       ├── 2025-05-26_13:24:11.jpg
    │       ├── 2025-05-26_13:24:21.jpg
    │       ├── 2025-05-26_13:24:31.jpg
    │       ├── 2025-05-26_13:24:42.jpg
    │       ├── 2025-05-26_13:25:09.jpg
    │       ├── 2025-05-26_13:25:20.jpg
    │       ├── 2025-05-26_13:25:31.jpg
    │       ├── 2025-05-26_13:25:42.jpg
    │       ├── 2025-05-26_13:25:52.jpg
    │       ├── 2025-05-26_13:26:02.jpg
    │       ├── 2025-05-26_15:28:19.jpg
    │       └── 2025-05-26_15:29:14.jpg
    └── templates
        ├── image_data.html
        ├── image_menu.html
        └── index.html
```