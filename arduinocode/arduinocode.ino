#define arduinoOTA
#define basecode
//#define experiment
//#define maincode
//#define RTCUPDATE

#ifdef maincode

#include "libs.h"


ESP8266WiFiMulti wifiMulti;
long reconnect_time=5000,current_time=0;



void setup(){
WiFi.mode(WIFI_AP_STA);
WiFi.softAP("Locker","");
wifiMulti.addAP(STASSID,STAPSK);
wifiMulti.addAP(STASSID1,STAPSK1);
wifiMulti.addAP(STASSID2,STAPSK2);

disp.init(0x3c);
disp.clear();
disp.logo();
disp.clear();
disp.println("Waiting for wifi");
while (wifiMulti.run(1000)!=WL_CONNECTED){}
disp.clear();
disp.println("Connected to\n :"+WiFi.SSID());
disp.println("IP1:"+WiFi.localIP().toString());
disp.println("IP2:"+WiFi.softAPIP().toString());
ota.init();
delay(1000);
}

void loop(){
  update();
}

void update(){
  wifiMulti.run(100);
  ota.handle();
}
#endif
