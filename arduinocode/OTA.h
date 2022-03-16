#define OTA_h

#include"libs.h"



class OTA{
    public:
    
    void init();
    void handle();
}ota;

void OTA::init(){
    ArduinoOTA.onStart([]() {
    disp.clear();
    disp.println("Started Uploading");
    disp.progress=true;
  });

  ArduinoOTA.onEnd([]() {
    disp.progress=false;
    disp.clear();
    disp.println("Upload completed");
    delay(1000);
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    
  });

  ArduinoOTA.onError([](ota_error_t error) {
      disp.clear();
    disp.println("Error["+String(error)+"]: ");
    if (error == OTA_AUTH_ERROR) {
      disp.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      disp.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      disp.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      disp.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      disp.println("End Failed");
    }
  });
  ArduinoOTA.begin();
}

void OTA::handle(){
    ArduinoOTA.handle();
}
