#ifdef RTCUPDATE
#include"libs.h"


WiFiUDP ntpUDP;
ESP8266WiFiMulti wifiMulti;

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);
static DS1307 RTC;

void udelay(int d){
  while((d--)>0){
    delay(1);
    ota.handle();
  }
}

void setup(){
  disp.init(0x3c);
  disp.clear();
disp.println("setupokey");
  delay(1000);
  RTC.begin();
  WiFi.mode(WIFI_AP_STA);
WiFi.softAP("Locker","");
wifiMulti.addAP(STASSID,STAPSK);
wifiMulti.addAP(STASSID1,STAPSK1);
wifiMulti.addAP(STASSID2,STAPSK2);
 
disp.clear();
disp.println("Connecting to wifi");
  while ( wifiMulti.run(1000) != WL_CONNECTED ) {
    delay ( 500 );
    disp.print ( "." );
  }
  ota.init();
disp.clear();
disp.println("Connected");
delay(1000);
  timeClient.begin();
  RTC.setDateTime(__DATE__,__TIME__);
  delay(2000);
  timeClient.update();
  disp.clear();
  disp.println(timeClient.getFormattedTime());
  RTC.setHours(timeClient.getHours());
  RTC.setMinutes(timeClient.getMinutes());
  RTC.setSeconds(timeClient.getSeconds());
}

void loop() {
  timeClient.update();
  disp.clear();
  disp.println(timeClient.getFormattedTime());
  ota.handle();
  delay(1000);
}



#endif
