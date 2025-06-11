#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

/* WIFI Credential */
const char *ssid = "***"; // Silahkan isi ini dengan ID WIFI 
const char *password = "**********************************"; // Silahkan isi ini dengan password WIFI

/* Server URL (localhost) */
const char *URL = "http://192.168.1.54:5000";

/* Interval pengambilan foto */
#define INTERVAL_FOTO 1800  /* Detik, total 30 menit */

void setup_camera(){

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG; 
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if(psramFound()){
    config.jpeg_quality = 10;
    config.fb_count = 2;
    config.grab_mode = CAMERA_GRAB_LATEST;
  } 
  else {
    config.frame_size = FRAMESIZE_SVGA;
    config.fb_location = CAMERA_FB_IN_DRAM;
  }

  // camera initialization
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Kamera gagal dibuka dengan kode error 0x%x", err);
    return;
  }
  delay(1000);
  Serial.println();

  sensor_t *s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }
  
  pinMode(LED_GPIO_NUM, OUTPUT);  /* Camera Flash LED */
}

void flash_on(int delayAfterFlash){
  digitalWrite(LED_GPIO_NUM, HIGH);
  delay(delayAfterFlash);
}

void flash_off(){
  digitalWrite(LED_GPIO_NUM, LOW);
}

void setup(){
  Serial.begin(115200);
  Serial.println();

  /* Connect to WIFI */
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.printf("Memuat koneksi WiFi '%s'", ssid);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\n");
  Serial.printf("Terkoneksi ke WiFi '%s'\n", ssid);
  delay(1000);

  setup_camera();
}

void loop() {
   if(WiFi.status() == WL_CONNECTED){
      /* Connect to server */
      HTTPClient httpClient;
      int httpConnectionSuccess = httpClient.begin(URL);
      if(!httpConnectionSuccess){
        Serial.printf("[ERROR] URL tidak valid.\n");
        return;
      }

      /* Mengambil gambar */
      Serial.printf("\nMasuk Proses mengirim gambar\n");

      flash_on(300);
      camera_fb_t *frame = esp_camera_fb_get();
      flash_off();

      if(frame == NULL){
        Serial.printf("Gagal mengambil gambar dari kamera\n");
      }

      /* Mengirim gambar ke server */
      httpClient.addHeader("Content-Type", "image/jpeg");
      httpClient.addHeader("ngrok-skip-browser-warning", ".");
      int httpResponseCode = httpClient.POST(frame->buf, frame->len);
      Serial.printf("[HTTP RESPONSE CODE] %d\n", httpResponseCode);

      if(httpResponseCode == 200) {
        Serial.printf("Berhasil mengirim gambar ke server.\n");
      }  
      else{
        Serial.printf("Gagal mengirim gambar ke server.\n[ERROR] %s\n", httpResponseCode, httpClient.errorToString(httpResponseCode).c_str());
      }

      httpClient.end();
      esp_camera_fb_return(frame);
      Serial.printf("\n");
  }

  /* Delay interval */
  delay(INTERVAL_FOTO * 1000); /* konversi ke milidetik */
}
