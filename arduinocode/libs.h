#define AVGDELAY 2000

#define display_contrast_adress 0
#define locker_data_addr 100

#include <ESP8266WiFiMulti.h>

#include <NTPClient.h>

//#include <sstream>

//#include<vector>

#include <SPI.h>

#include <Wire.h>

#include <Adafruit_SH110X.h>

#include <Adafruit_SSD1306.h>

#include <Hash.h>

#include <EEPROM.h>

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>

#include <ESP8266WebServer.h>

//#include <WiFiManager.h>

//#include <ESP8266mDNS.h>

//#include <WiFiUdp.h>

#include <ArduinoOTA.h>

#include <PCF8574.h>

#include <UrlEncode.h>

#include <Servo.h>

#ifndef pcf8574_h
#define pcf8574_h
#include <PCF8574.h>
#endif

#ifndef RTC_h
#include <RTC.h>
#define RTC_h
#endif

#ifndef locker_h
#define locker_h
#include"locker.h"
#endif

#ifndef OTA_h
#define OTA_h
#include"OTA.h"
#endif 

#ifndef connectivity_h
#define connectivity_h
#include"connectivity.h"
#endif

#ifndef input_h
#define input_h
#include"input.h"
#endif

#ifndef time_h
#define time_h
#include"time.h"
#endif

#ifndef display_h
#define display_h
#include"display.h"
#endif

#ifndef utility_h
#define utility_h
#include"utility.h"
#endif

#ifndef bitmaps_h
#define bitmaps_h
#include"bitmaps.h"
#endif

#ifndef credentials_h
#define credentials_h
#include"credentials.h"
#endif

#ifndef timer_h
#define timer_h
#include"timer.h"
#endif


using namespace std;
