#include <ESP8266WiFi.h>
#define WLAN_SSID       "Raj"
#define WLAN_PASS       "12345abcd"
#include "Wire.h"
const int MPU_ADDR = 0x68;
int16_t accelerometer_x, accelerometer_y, accelerometer_z;
int16_t gyro_x, gyro_y, gyro_z;
int16_t temperature;
int crashpin = D3;
int val ;
int r;
int temp;
char tmp_str[7];
char* convert_int16_to_str(int16_t i) {
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}


void setup() {
  Serial.begin(115200);
  pinMode(crashpin, INPUT);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}


void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 7 * 2, true);
  accelerometer_x = Wire.read() << 8 | Wire.read();
  accelerometer_y = Wire.read() << 8 | Wire.read();
  accelerometer_z = Wire.read() << 8 | Wire.read();
  temperature = Wire.read() << 8 | Wire.read();
  gyro_x = Wire.read() << 8 | Wire.read();
  gyro_y = Wire.read() << 8 | Wire.read();
  gyro_z = Wire.read() << 8 | Wire.read();
  temp = (temperature / 340.00 + 36.53);
  val = digitalRead(crashpin);

  if (accelerometer_x < 1000 && accelerometer_y < -8000) {
    r = 0;
  }

  else if (accelerometer_x > 14000  && accelerometer_y < 8000) {
    r = 1;
  }

  else if (accelerometer_x > 8000 && accelerometer_y < 1000) {
    r = 0;
  }
  else if (accelerometer_x < -8000 && accelerometer_y < 1000) {
    r = 0;
  }

  else {
    r = 0;
  }
  Serial.print    ("r     =");
  Serial.println  (r);
  Serial.print    ("temp  =");
  Serial.println  (temp);
  Serial.print    ("crash =");
  Serial.println  (val);
  Serial.println();

  if ((r == 0 || temp > 60 || val == 0))
  {
    Serial.print("Connecting to ");
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    Serial.println();
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(12000);
    WiFi.disconnect();
    Serial.println("WiFi disconnected");



  }
  else
  {
    Serial.println("  -----Everything is Fine-----");
  }

  delay(2000);
}
