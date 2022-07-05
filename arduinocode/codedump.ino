#ifdef basecode

#include "libs.h"

using namespace std;

#ifdef arduinoOTA
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#ifndef STASSID
#define STASSID "AP"
#define STAPSK "123456789"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
#endif

Adafruit_SH1106G oled = Adafruit_SH1106G(128, 64, &Wire, -1);
;


void sdelay(long n) {
  while (n > 0) {
    delay(1);
    n--;
    ArduinoOTA.handle();
    /*
    while(digitalRead(D3)){
      delay(1);
      if(digitalRead(D3))continue;
      disp.clear();
    disp.print("down");
    delay(100);
    }
    while(digitalRead(D4)){
      delay(1);
      if(digitalRead(D4))continue;
      disp.clear();
    disp.print("back");
    delay(100);
    }
    while(digitalRead(D5)){
      delay(1);
      if(digitalRead(D5))continue;
      disp.clear();
    disp.print("okey");
    delay(100);
    }
    while(digitalRead(D6)){
      delay(1);
      if(digitalRead(D6))continue;
      disp.clear();
    disp.print("up");
    delay(100);
    }*/
  }
}

static DS1307 RTC;
PCF8574 port(0x20);

void setup() {
  Serial.begin(115200);
  RTC.begin();
  port.begin();
  oled.begin(0x3c, true);
  disp.init(0x3c, true);
  oled.setContrast(0);
  oled.display();

  oled.clearDisplay();
  Serial.println("hi");
#ifdef arduinoOTA
  println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }



  ArduinoOTA.onStart([]() {
    disp.clear();
    disp.println("Started Uploading");
    disp.progress = true;
  });
  ArduinoOTA.onEnd([]() {
    disp.progress = false;
    disp.clear();
    disp.println("Upload completed");
    delay(1000);
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {

  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  disp.clear();
  disp.println("Ready\n\nIP: " + WiFi.localIP().toString());
  sdelay(1000);
  disp.oled.clearDisplay();

#endif


  if (RTC.isRunning()) {
    Serial.println("Yes");
    Serial.print(RTC.getDay());
    Serial.print("-");
    Serial.print(RTC.getMonth());
    Serial.print("-");
    Serial.print(RTC.getYear());
    Serial.print(" ");
    Serial.print(RTC.getHours());
    Serial.print(":");
    Serial.print(RTC.getMinutes());
    Serial.print(":");
    Serial.print(RTC.getSeconds());
    Serial.print("");
    if (RTC.getHourMode() == CLOCK_H12) {
      switch (RTC.getMeridiem()) {
        case HOUR_AM:
          Serial.print(" AM");
          break;
        case HOUR_PM:
          Serial.print(" PM");
          break;
      }
    }
    Serial.println("");
    delay(1000);
  } else {
    delay(1500);

    println("No");
    delay(1000);
    oled.println("Setting Time");
    //RTC.setHourMode(CLOCK_H12);
    RTC.setHourMode(CLOCK_H24);
    RTC.setDateTime(__DATE__, __TIME__);

    oled.println("New Time Set");
    oled.println(__DATE__);
    // Serial.print(" ");
    oled.println(__TIME__);
    oled.display();
    RTC.startClock();
    delay(5000);
    oled.clearDisplay();
  }


  text(1000);
  logo(1000);
  //faceshow(1000);
  //disp.clear_display=true;
  while (true) {
    sdelay(1000);
    //println(String(RTC.getDay())+"-"+String(RTC.getMonth())+"-"+String(RTC.getYear())+"\n"+String(RTC.getHours())+":"+String(RTC.getMinutes())+":"+String(RTC.getSeconds()));

    disp.clear();
    disp.print(String(RTC.getDay()));
    disp.print("-");
    disp.print(String(RTC.getMonth()));
    disp.print("-");
    disp.print(String(RTC.getYear()));
    disp.print("\n");
    disp.print(String(RTC.getHours()));
    disp.print(":");
    disp.print(String(RTC.getMinutes()));
    disp.print(":");
    disp.print(String(RTC.getSeconds()));
  }
}

void loop() {
#ifdef arduinoOTA
  ArduinoOTA.handle();
#endif


  sdelay(100);
  while (Serial.available()) {
    oled.clearDisplay();
    oled.setCursor(0, 0);
    String temp = Serial.readString();
    for (int j = 0; j < temp.length(); j++)
      if (temp[j] == '/') {
        oled.setTextSize(temp[j + 1] - 48);

        temp[j] = '\0';
        temp[j + 1] = '\0';
      }
    for (int j = 0; j < temp.length(); j++)
      if (temp[j] == '\\') {
        temp[j] = 219 + temp[j + 1] - 48;
        temp[j + 1] = '\0';
      }

    for (int j = 0; j < temp.length(); j++)
      if (temp[j] == '\\') {
        temp[j] = 219 + temp[j + 1] - 48;
        temp[j + 1] = '\0';
      }
    oled.print(temp);
    Serial.print(temp);
    oled.display();
    sdelay(1000);

    oled.display();
  }
  // put your main code here, to run repeatedly:
}

void text(int d) {
  oled.setCursor(0, 0);
  oled.setTextSize(1);
  oled.setTextColor(SH110X_WHITE);
  oled.setCursor(0, 0);
  oled.print("Vineeth Rai K\n\n");
  oled.setTextSize(2);
  oled.print("7337688347\n");
  oled.setTextSize(1);
  oled.println("\ntesting some things\nhere and there");
  oled.display();
  sdelay(d);
}

void logo(int d) {
  oled.clearDisplay();
  oled.drawBitmap(128 / 2 - 41 / 2, 64 / 2 - 41 / 2, logo2, 41, 41, 1);
  oled.display();
  sdelay(d);
}

void faceshow(int d) {
  oled.clearDisplay();
  //oled.drawBitmap(128 / 2 - 90 / 2, 64 / 2 - 64 / 2,  face, 90, 64, 1);
  oled.drawBitmap(0, 0, face2, 128, 64, 1);
  oled.display();
  sdelay(d);
}

void println(String x) {
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.setTextSize(1);
  oled.setTextColor(SH110X_WHITE);
  oled.print(x);
  oled.display();
  sdelay(1);
}

int readpin() {
  int l[] = { 4, 5, 6 };
  int r[] = { 0, 1, 2, 3 };
  int v = -1;


  return v;
}


#endif