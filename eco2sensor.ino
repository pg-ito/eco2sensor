#include <M5StickC.h>

//SGP30 needs 15 seconds to initialize calibration after power on.
//The screen will display TVOC and CO2

// #include <M5Stack.h>
#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp;
int i = 15;
long last_millis = 0;


void setup() {
//  M5.begin(true, false, true, true);
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(1);
  // header("SGP30 TEST",TFT_BLACK);
  Serial.begin(115200);
  Serial.println("SGP30 test");
  if (! sgp.begin()){
    M5.Lcd.printf("Sensor not found :(\n");
    Serial.println("Sensor not found :(");
    while (1);
  }

  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);
  M5.Lcd.drawString("Initialization...", 10, 130, 4);

}

void loop() {
  while(i > 0) {    
    if(millis()- last_millis > 1000) {
      last_millis = millis();
      i--;
      M5.Lcd.fillRect(20, 150, 40, 20, TFT_BLACK);
      char buf[3];
      sprintf(buf, "%02d", i),
      M5.Lcd.drawString(buf, 20, 150, 4);
    }
  }
  // M5.Lcd.fillRect(0, 120, 300, 30, TFT_BLACK);

  if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 15);
  
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("eCO2:\n",sgp.eCO2);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("%d ppm\n",sgp.eCO2);
  
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("TVOC:%d ppb\n",sgp.TVOC);
  
  Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
  Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");
  delay(1000);
  if (! sgp.IAQmeasureRaw()) {
    Serial.println("MeasurementRaw failed");
    return;
  }
  Serial.print("RawTVOC "); Serial.print(sgp.rawEthanol); Serial.print(" ppb\t");
  Serial.print("RaweCO2 "); Serial.print(sgp.rawH2); Serial.println(" ppm");
  delay(3000);
}
