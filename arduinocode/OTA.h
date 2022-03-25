#include"libs.h"



class OTA{
    public:
    
    void init();
    void handle();
}ota;

void OTA::init(){
    ArduinoOTA.onStart([]() {
    disp.clear();
    disp.println("Started Downloading");
    disp.progress=true;
  });

  ArduinoOTA.onEnd([]() {
    disp.progress=false;
    disp.clear();
    disp.println("Download completed");
    delay(1000);
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    disp.oled.clearDisplay();
    disp.oled.setCursor(0, 0);
    disp.oled.print("total bytes:");
    disp.oled.print(total);
    disp.oled.print("\n\nBytes downloaded:\n");
    disp.oled.print(progress);
    disp.oled.print("\n\nDownloading:");
    disp.oled.print((progress*100)/total);
    disp.oled.print("\%");
    disp.oled.display();
  });

  ArduinoOTA.onError([](ota_error_t error) {
    disp.progress=false;
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
    delay(1);
}
