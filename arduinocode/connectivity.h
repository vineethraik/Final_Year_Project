#include "libs.h"

ESP8266WiFiMulti wifiMulti;

class connectivity{
  private:
  function<void(void)> update=nullptr;

  public:
  void init();
  bool connect(bool);
  void printconnection();
  bool run(int);
  void addupdatefunction(function<void(void)>);

  
}wifi;


void connectivity::init(){
  WiFi.persistent(false);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("Locker","");
  #ifdef STASSID1
  wifiMulti.addAP(STASSID1,STAPSK1);
  #endif
  #ifdef STASSID2
  wifiMulti.addAP(STASSID2,STAPSK2);
  #endif
  #ifdef STASSID3
  wifiMulti.addAP(STASSID3,STAPSK3);
  #endif
  #ifdef STASSID4
  wifiMulti.addAP(STASSID4,STAPSK4);
  #endif
  #ifdef STASSID5
  wifiMulti.addAP(STASSID5,STAPSK5);
  #endif
  #ifdef STASSID6
  wifiMulti.addAP(STASSID6,STAPSK6);
  #endif
}

bool connectivity::connect(bool is_os_connect=false){
  while (wifiMulti.run(3000)!=WL_CONNECTED){
    delay(1);
    update();
    if(is_os_connect){
      if(io.check_back(false)){
        disp.clear();
        disp.print("process interupted");
        delay(AVGDELAY);
        return false;
      }
    }
    }
    return true;
}

void connectivity::printconnection(){
  disp.clear();
  disp.println("SSID:"+WiFi.SSID());
  disp.println("\nSTAIP:"+WiFi.localIP().toString());
  disp.println("\nAPIP:"+WiFi.softAPIP().toString());
}

bool connectivity::run(int d=1){
  return wifiMulti.run(d);
}

void connectivity::addupdatefunction(function<void(void)> fn){
  update=fn;
}