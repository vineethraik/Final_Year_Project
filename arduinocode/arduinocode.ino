#define arduinoOTA
//#define basecode
#define experiment
//#define maincode
//#define RTCUPDATE
//#define memory_init

#ifdef maincode

#include "libs.h"


void update();
String temp;


void setup(){
locker.init();

disp.clear();
disp.logo();
disp.clear();

disp.println("Waiting for wifi");
delay(1000);
wifi.connect();
delay(1000);
wifi.printconnection();
//ota.init();
delay(2000);
io.addupdatefunction(update);
}

void loop(){
  update();
  switch(io.read_switch()){
    case io.NILL:
    disp.clear();
    disp.print("no switch pressed");
    break;

    case io.OK:
    disp.clear();
    disp.print("OK pressed");
    break;

    case io.UP:
    disp.clear();
    disp.print("UP pressed");
    break;

    case io.DOWN:
    disp.clear();
    disp.print("DOWN pressed");
    break;

    case io.BACK:
    disp.clear();
    disp.print("BACK pressed");
    break;
  }
  delay(1000);
  disp.clear();
  disp.print("pin is:");
  temp=io.read_pin();
  disp.print("->"+temp);
  delay(1000);
  disp.clear();
  disp.print("number:");
  temp=io.read_number();
  disp.print("->"+temp);
  
}

void update(){
  wifi.run(100);
  ota.handle();
  delay(1);
}
#endif
