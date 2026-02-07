# ESP32 Pörssisähkömonitori

Yksinkertainen IoT-laite, joka hakee pörssisähkön hinnan `api.porssisahko.net` -palvelusta ja näyttää sen OLED-näytöllä.

## Ominaisuudet
- Yhdistää WiFi-verkkoon.
- Hakee ajantasaisen sähkön hinnan.
- Näyttää hinnan selkeästi SSD1306 OLED -näytöllä (senttiä/kWh).
- Päivittää tiedot 15 minuutin välein.

## Laitteisto
- **Mikrokontrolleri:** ESP32 (esim. ESP32 DevKit V1)
- **Näyttö:** 0.96" I2C OLED (SSD1306, 128x64 pikseliä)

## Kytkentä
| ESP32 | OLED Näyttö |
|-------|-------------|
| 3.3V  | VCC         |
| GND   | GND         |
| D21   | SDA         |
| D22   | SCL         |

## Asennus
1. **Vaatimukset:**
   - [PlatformIO](https://platformio.org/) (VS Code -laajennus).
2. **Kloonaa repositorio:**
   ```bash
   git clone <REPO_URL>
   ```
3. **Määritä WiFi-asetukset:**
   Avaa `src/main.cpp` ja muokkaa seuraavat rivit vastaamaan omaa verkkoasi:
   ```cpp
   const char* ssid = "OMA_VERKKO";
   const char* password = "SALASANA";
   ```
4. **Lataa koodi:**
   Kytke ESP32 tietokoneeseen ja paina PlatformIO:n **Upload**-nappia.

## Kirjastot
Projekti käyttää seuraavia kirjastoja (asentuvat automaattisesti `platformio.ini` kautta):
- Adafruit SSD1306
- Adafruit GFX Library
- ArduinoJson
