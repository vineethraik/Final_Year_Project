#ifdef experiment

#include "libs.h"

void setup() {
  //locker.writemem();
  //Serial.begin(115200);
  //Serial.setDebugOutput(true);
  

  locker.init();
  disp.clear();
  disp.logo();
  disp.clear();
  locker.addupdatefunction([]() {
    yield();
    //delay(1);
    locker.handle_server();
    //ota.handle();
    timer.run();
  });
  disp.println(F("Connecting to wifi"));
  wifi.connect();
  disp.clear(true);

  /*
  timer.set_new_event(1000,10000,[]()->bool{
    disp.save_buffer();
    disp.clear();
    disp.print(String(ESP.getFreeHeap()));
    unsigned long t=millis();
    while(millis()-t<1000){
      yield();
      locker.handle_server();
    }
    disp.load_buffer();
    return true;
  });

/**/
}
void loop() {
  locker.run_os();
  delay(1);
  //ota.handle();
}


#endif