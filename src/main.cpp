#include <Arduino.h>        // framework
#include <WiFi.h>           // yhteys verkkoon
#include <HTTPClient.h>     // tiedot nettisivulta
#include <ArduinoJson.h>    // purkaa json
#include <Wire.h>           // i2c oled näyttöön
#include <Adafruit_GFX.h>   // grafiikka
#include <Adafruit_SSD1306.h> // ss1306 oled näytön ohjain
#include "time.h"           // kellonajan hallinta

// omat verkon salasanat 
const char* ssid = "--------";
const char* password = "-----------";

//näytön koko ja nollaus , -1 ei erillistä reset pinniä
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// näytön asetukset 
void updateDisplay(String otsikko, String hinta_teksti) {
    display.clearDisplay();         // vanha kuva pois 
    display.setTextColor(SSD1306_WHITE); // Asetetaan tekstin väri (valkoinen mustalla pohjalla)
    
    //yläotsikko
    display.setTextSize(1);         // koko
    display.setCursor(0,0);         // sijainti
    display.println(otsikko);       //teksti
    
    // hinta
    display.setTextSize(3);         // koko
    display.setCursor(0, 25);       // sijainti
    display.print(hinta_teksti);    //teksti
    
    // hinnan perään yksikkö
    display.setTextSize(1);
    display.println(" snt");
    
    display.display();              
}

//API gemini teki
float fetchCurrentPrice() {
    // wifi
    if (WiFi.status() != WL_CONNECTED) return -1.0;

    // Haetaan tämänhetkinen UTC-aika ESP32:n sisäisestä kellosta
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)) return -1.0;

    // Muutetaan aika API:n ymmärtämään muotoon (esim. 2026-02-07T16:00:00Z)
    char aikaleima[25];
    strftime(aikaleima, sizeof(aikaleima), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    
    // Rakennetaan kyselyosoite lisäämällä aikaleima osoitteen perään
    String url = "https://api.porssisahko.net/v2/price.json?date=" + String(aikaleima);
    
    // Otetaan yhteys palvelimeen
    HTTPClient http;
    http.begin(url);
    int palautuskoodi = http.GET(); // "GET" pyytää tiedot palvelimelta
    
    float hinta = -1.0; // Oletusarvo virhetilanteelle

    // Jos palvelin vastaa 200 (OK), luetaan tiedot
    if (palautuskoodi == 200) {
        String vastaus = http.getString(); // Tallennetaan koko vastaus tekstinä
        
        // Luodaan JSON-asiakirja ja puretaan vastaus siihen
        StaticJsonDocument<256> doc;
        deserializeJson(doc, vastaus);
        
        // hinta json  {"price": 5.43}
        hinta = doc["price"];
    }
    
    http.end(); //suljetaan yhteys
    return hinta;
}


void setup() {
    Serial.begin(115200);           
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // näytön käynnistys
    
    updateDisplay("sahkon hinta:", "..."); 

    // Yhdistetään wifi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500); // puolisekuntia per yritys
    }


    // kellon synkronointi (pool.ntp.org) api varten
    configTime(0, 0, "pool.ntp.org");
    delay(2000); 
}



void loop() {
    // nykyhinta
    float nykyinenHinta = fetchCurrentPrice();
    
    // tarkistetaan, onnistuiko haku (hinta on positiivinen tai nolla)
    if (nykyinenHinta >= 0) {
        // päivitetään näyttö , 2 desimaalia
        updateDisplay("sahkon hinta:", String(nykyinenHinta, 2));
    } 

    // aika ms  15*60*1000 = 15 min
    delay(15 * 60 * 1000); 
}