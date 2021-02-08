#include <M5StickC.h>

//SGP30 needs 15 seconds to initialize calibration after power on.
//The screen will display TVOC and CO2

// #include <M5Stack.h>
#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp;
int i = 15;
long last_millis = 0;

const long reset_max = 1200;// reset every 1h e.g. wait_msec*reset_max = 3600,000[msec]
const int wait_msec = 3000;
long reset_count = 0;

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
  reset_count++;
  if(reset_count > reset_max){
    sgp.IAQinit();
    sleep(wait_msec);
    reset_count = 0;
    return;
  }

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 15);
  
  M5.Lcd.setTextSize(2);
  int eco2 = (sgp.eCO2>999)? 999: sgp.eCO2;
  M5.Lcd.printf("eCO2:\n", eco2);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("%d ppm\n", sgp.eCO2);
  
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
  delay(wait_msec);
}
