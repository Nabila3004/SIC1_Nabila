#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 18        // Pin dimana sensor DHT11 terhubung
#define DHTTYPE DHT11   // Tipe sensor yang digunakan
DHT dht(DHTPIN, DHTTYPE);

// SSID dan Password WiFi
const char* ssid = "Sukses Bersama";
const char* password = "berjuang3";

// URL server tujuan
const char* serverName = "http://192.168.1.44:5000/";

// Fungsi untuk mengklasifikasikan suhu
String classifyTemperature(float temperature) {
  if (temperature < 20) {
    return "Cold";
  } else if (temperature < 30) {
    return "Warm";
  } else {
    return "Hot";
  }
}

// Fungsi untuk mengklasifikasikan kelembapan
String classifyHumidity(float humidity) {
  if (humidity < 30) {
    return "Dry";
  } else if (humidity < 60) {
    return "Moderate";
  } else {
    return "Humid";
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Menghubungkan ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Membaca suhu dan kelembapan dari DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Memastikan pembacaan berhasil
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Klasifikasikan suhu dan kelembapan
  String tempCategory = classifyTemperature(t);
  String humCategory = classifyHumidity(h);

  // Membuat JSON payload
  String postData = "{ \"temperature\": " + String(t) + ", \"humidity\": " + String(h) + 
                    ", \"tempCategory\": \"" + tempCategory + "\", \"humCategory\": \"" + humCategory + "\" }";

  // Mengirim data ke server melalui HTTP POST
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }

  // Interval untuk membaca data lagi (misalnya setiap 10 detik)
 delay(10000);
}