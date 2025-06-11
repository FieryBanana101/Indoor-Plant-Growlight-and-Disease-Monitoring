#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"

#define PIN_LED_MERAH 27
#define PIN_LED_BIRU 25

const char *ssid = "***"; // Silahkan isi ini dengan ID WIFI 
const char *password = "**********************************"; // Silahkan isi ini dengan password WIFI

// Server untuk melihat waktu saat ini.
const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const char *ntpServer3 = "time.google.com";

const char *time_zone = "WIB-7";

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  Serial.println();
  Serial.println("Server time:");
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void timeavailable(struct timeval *t) {
  Serial.println("Terdapat perubahan dari server NTP");
  printLocalTime();
}

void setup() {
  Serial.begin(115200);

  /* Setup  WiFi */
  Serial.printf("Menghubungkan ke WiFi %s\n", ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);

    Serial.print(".");
  }
  Serial.printf("WiFi %s terhubung\n", ssid);

  /* Setup server time */
  esp_sntp_servermode_dhcp(1);
  sntp_set_time_sync_notification_cb(timeavailable);
  configTzTime(time_zone, ntpServer1, ntpServer2, ntpServer3);

  /* Setup GPIO */
  pinMode(PIN_LED_MERAH, OUTPUT);
  pinMode(PIN_LED_BIRU, OUTPUT);
}

void loop() {
  delay(5000);
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Waktu server tidak dapat diambil.");
    return;
  }
  Serial.println();
  Serial.println("Waktu server:");
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  int hour = timeinfo.tm_hour;
  
  // Pembagian 3 periode waktu lampu
  // 1. (6.00 - 17.00) -> Full spektrum merah-biru
  // 2. (17.00 - 22.00) -> Spektrum biru
  // 3. (22.00 - 6.00) -> Lampu mati
  if(6 <= hour && hour < 17){
    digitalWrite(PIN_LED_MERAH, HIGH);
    digitalWrite(PIN_LED_BIRU, HIGH);
  }
  else if(17 <= hour && hour < 22){
    digitalWrite(PIN_LED_BIRU, HIGH);
    digitalWrite(PIN_LED_MERAH, LOW);
  }
  else{
    digitalWrite(PIN_LED_MERAH, LOW);
    digitalWrite(PIN_LED_BIRU, LOW);
  }
  
}
