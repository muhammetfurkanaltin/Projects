#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Blynk ve WiFi bilgileri
char auth[] = "auth";  // Blynk Authentication Token
char ssid[] = "ssid";  // WiFi SSID
char pass[] = "pass";  // WiFi Şifresi

#define BLYNK_PRINT Serial  // Blynk seri çıktıları
#define PIN_UPTIME V6       // Uptime değeri için kullanılan sanal pin
BlynkTimer timer;           // Zamanlayıcı tanımlaması
Servo servo;                // Servo motor objesi

// Uptime değerini Blynk'e gönderir
BLYNK_READ(PIN_UPTIME) {
  Blynk.virtualWrite(PIN_UPTIME, millis() / 1000);  // Milisaniyeyi saniyeye çevirip gönderir
}

// Analog sensör okumalarını işler ve sıcaklık değerini Blynk'e gönderir
void myTimerEvent() {
  int analogValue = analogRead(A0);              // A0 pininden analog değer okunuyor
  float millivolts = (analogValue / 1600.0) * 3300;  // Okunan değeri milivolta çeviriyor
  float celsius = millivolts / 10;               // Milivolttan sıcaklık derecesine çeviri
  Blynk.virtualWrite(V5, celsius);               // Sıcaklık değerini Blynk'e gönderir
}

// Servo motor kontrolü
BLYNK_WRITE(V0) {
  int s0 = param.asInt();  // Blynk uygulamasından gelen değeri alır
  Serial.println(s0);      // Seri monitöre yazdırır
  servo.write(s0);         // Servo motoru istenen açıya getirir
  Blynk.virtualWrite(V1, s0);  // Güncel servo açısını Blynk'e gönderir
  delay(1000);             // 1 saniye bekler
  servo.write(85);         // Servo motoru varsayılan konuma (85 derece) geri getirir
  Blynk.virtualWrite(V1, 85);  // Servo açısını günceller
}

void setup() {
  Serial.begin(9600);        // Seri haberleşme başlatılır
  servo.attach(D4);          // Servo D4 pinine bağlanır
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);  // Blynk başlatılır
  timer.setInterval(1000L, myTimerEvent);  // 1 saniyelik döngüyle sıcaklık okuma ayarlanır
}

void loop() {
  Blynk.run();  // Blynk fonksiyonlarını çalıştırır
  timer.run();  // Zamanlayıcıyı çalıştırır
}
