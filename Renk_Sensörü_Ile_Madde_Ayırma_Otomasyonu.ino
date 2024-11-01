#include "Servo.h"
#include "Wire.h"
#include "Adafruit_TCS34725.h"

const int kirmiziPos = 135;  
const int turuncuPos = 115;   
const int sariPos = 20;       
const int yesilPos = 80;     
const int maviPos = 60;       
const int yokPos = 40;       

Servo servo1;
Servo servo2;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
    Serial.begin(9600);
    Serial.println("Sensör Başlatıldı");

    if (tcs.begin()) {
        Serial.println("Sensör Bulundu");
    } else {
        Serial.println("Sensör yok, bağlantıları kontrol et");
        while (1);
    }
    servo1.attach(3);
    servo2.attach(2);
    servo1.write(yokPos); 
    servo2.write(yokPos); 
    delay(1000);
}

void loop() {
    uint16_t clearcol, kirmizi, yesil, mavi;
    float ortalama, k, y, m;

    delay(100);
    tcs.getRawData(&kirmizi, &yesil, &mavi, &clearcol);

    ortalama = (kirmizi + yesil + mavi) / 3;
    k = kirmizi / ortalama;
    m = mavi / ortalama;
    y = yesil / ortalama;

    Serial.println("\tClear:"); Serial.println(clearcol);
    Serial.println("\tKırmızı:"); Serial.println(k);
    Serial.println("\tMavi:"); Serial.println(m);
    Serial.println("\tYeşil:"); Serial.println(y);

   
    if (k > 1.2 && k > m && k > y) { 
        Serial.println("Kırmızı nesne tespit edildi");
        servo1.write(kirmiziPos);
        delay(2000); 
        servo1.write(yokPos); 
    } else if (y > 1.2 && y > k && y > m) { 
        Serial.println("Yeşil nesne tespit edildi");
        servo1.write(yesilPos);
        delay(2000); 
        servo1.write(yokPos); 
    } else if (m > 1.2 && m > k && m > y) { 
        Serial.println("Mavi nesne tespit edildi");
        servo1.write(maviPos);
        delay(2000); 
        servo1.write(yokPos);
    } else {
        Serial.println("Tanımlanamayan renk");
    }

    delay(500);
}
