#include "libs.h"

class screen;
class input;

//#define IO_INVALID


#define BACK_pin 0
#define OK_pin 2
#define UP_pin 1
#define DOWN_pin 3

const int keypad_pins[7]{6,5,4,3,2,1,0};

class input {
  private:
  function<void(void)> update = nullptr;
  PCF8574 io1;
  PCF8574 io2;

  public:
  input();
  input(char, char);
  enum state { BACK,
               OK,
               UP,
               DOWN,
               NILL };

  state read_switch();
  bool check_back(bool);
  bool check_ok();
  bool check_down();
  bool check_up();
  bool check_down(int);
  bool check_up(int);
  String read_pin(int);
  String read_number(int);
  void addupdatefunction(function<void(void)>);

  #ifdef WEB_IO
  public:
    bool web_number_present=false;
    bool web_button_present=false;
    char web_number='\0';
    state web_button=NILL;
  #endif
  
}io;

input::input(char adr1, char adr2) {
  io1.setAddress(adr1);
  io2.setAddress(adr2);
  io1.begin();
  io2.begin();
}

input::input() {
  io1.setAddress(0x20);
  io2.setAddress(0x27);
  io1.begin();
  io2.begin();
}

input::state input::read_switch() {
  int timeout = 60000;
  while (true) {
    update();

    if (io2.read(DOWN_pin)) {
      while(io2.read(DOWN_pin))update();
      return DOWN;
    } else if (io2.read(BACK_pin)) {
      while(io2.read(BACK_pin))update();
      return BACK;
    } else if (io2.read(OK_pin)) {
      while(io2.read(OK_pin))update();
      return OK;
    } else if (io2.read(UP_pin)) {
      while(io2.read(UP_pin))update();
      return UP;
    } else if (web_button_present) {
      web_button_present=false;
      return web_button;
    } else {
      if (timeout > 0) {
        timeout--;
        delay(1);
      } else {
        return NILL;
      }
    }
  }
}

bool input::check_back(bool wait=true){
  if(io2.read(BACK_pin)){
    if(wait)
      while(io2.read(BACK_pin))update();
    return true;
  }
  #ifdef WEB_IO
  if(web_button_present){
    if(web_button==BACK){
      web_button_present=false;
      return true;
    }
  }
  #endif
  
  return false;
  
}

bool input::check_ok(){
  if(io2.read(OK_pin)){
    while(io2.read(OK_pin))update();
    return true;
  }
  #ifdef WEB_IO
  if(web_button_present){
    if(web_button==OK){
      web_button_present=false;
      return true;
    }
  }
  #endif
  
  return false;
  
}

bool input::check_up(){
  if(io2.read(UP_pin)){
    while(io2.read(UP_pin))update();
    return true;
  }
  
  #ifdef WEB_IO
  if(web_button_present){
    if(web_button==UP){
      web_button_present=false;
      return true;
    }
  }
  #endif
  
  return false;
  
}

bool input::check_down(){
  if(io2.read(DOWN_pin)){
    while(io2.read(DOWN_pin))update();
    return true;
  }
  #ifdef WEB_IO
  if(web_button_present){
    if(web_button==DOWN){
      web_button_present=false;
      return true;
    }
  }
  #endif
  
  return false;
}

bool input::check_up(int timegap){
  static unsigned int drop_delay=1000;
  static bool flag=false;
  unsigned long temp=millis();
  if(io2.read(UP_pin)){
    while(io2.read(UP_pin)&&(millis()-temp)<drop_delay)update();
    if(!flag){
      flag=true;
      drop_delay=timegap;
    }else{
      if(drop_delay>5)
      drop_delay-=5;
      else
      drop_delay=1;
    }
    return true;
  }else{
    drop_delay=1000;
    flag=false;
    return false;
  }
  
}

bool input::check_down(int timegap){
  static unsigned int drop_delay=1000;
  static bool flag=false;
  unsigned long temp=millis();
  if(io2.read(DOWN_pin)){
    while(io2.read(DOWN_pin)&&(millis()-temp)<drop_delay)update();
    if(!flag){
      flag=true;
      drop_delay=timegap;
    }else{
      if(drop_delay>5)
      drop_delay-=5;
      else
      drop_delay=1;
    }

    return true;
  }else{
    drop_delay=1000;
    flag=false;
    return false;
  }
  
}

String input::read_pin(int n = 6) {
  String pin = "";
  while (true) {
    delay(1);
    update();

    if (n < 1) break;
    if (check_back())return "nill";

    #ifdef WEB_IO
    if(web_number_present){
      pin+=web_number;
      n--;
      disp.print("*");
      web_number_present=false;
      continue;
    }
    #endif

    #ifndef IO_INVALID
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        io1.write(keypad_pins[j], (i == j) ? 0 : 1);
      }
      
      if (io1.readButton(keypad_pins[4]) == 0) {
        while (io1.readButton(keypad_pins[4]) == 0) update();
        if (i != 3) {
          n--;
          pin += (i * 3 + 1);
          disp.print("*");
          continue;
        }
      } else if (io1.readButton(keypad_pins[5]) == 0) {
        while (io1.readButton(keypad_pins[5]) == 0) update();
        n--;
        if (i == 3) {
          pin += 0;
          disp.print("*");
          continue;
        } else {
          pin += (i * 3 + 2);
          disp.print("*");
          continue;
        }
      } else if (io1.readButton(keypad_pins[6]) == 0) {
        while (io1.readButton(keypad_pins[6]) == 0) update();
        if (i != 3) {
          n--;
          pin += (i * 3 + 3);
          disp.print("*");
          continue;
        }
      }
    }
    #endif
  }
  return pin;
}

String input::read_number(int n=-1) {
  String pin = "";
  bool limited=true;
  if(n==-1)limited=false;
  while (true) {
    delay(1);
    update();
    #ifndef IO_INVALID
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        io1.write(keypad_pins[j], (i == j) ? 0 : 1);
      }
      
      if (io1.readButton(keypad_pins[4]) == 0) {
        while (io1.readButton(keypad_pins[4]) == 0) update();
        if (i != 3) {
          n--;
          pin += (i * 3 + 1);
          disp.print(String((i * 3 + 1)));
        }
      } else if (io1.readButton(keypad_pins[5]) == 0) {
        while (io1.readButton(keypad_pins[5]) == 0) update();
        n--;
        if (i == 3) {
          pin += 0;
          disp.print(String(0));
        } else {
          pin += (i * 3 + 2);
          disp.print(String((i * 3 + 2)));
        }
      } else if (io1.readButton(keypad_pins[6]) == 0) {
        while (io1.readButton(keypad_pins[6]) == 0) update();
        if (i != 3) {
          n--;
          pin += (i * 3 + 3);
          disp.print(String((i * 3 + 3)));
        }
      }
    }
    #endif
    if(limited){
      if(n<1){
        delay(AVGDELAY/2);
        return pin;
        
      }
    }
    if (check_back())return "nill";
    if(check_ok())break;

    //#ifdef WEB_IO
    if(web_number_present){
      web_number_present=false;
      pin+=web_number;
      n--;
      disp.print(String(web_number));
    }

    if(limited){
      if(n<1){
        delay(AVGDELAY/2);
        return pin;
        
      }
    }
    if (check_back())return "nill";
    if(check_ok())break;
    //#endif
  }
  return pin;
}

void input::addupdatefunction(function<void(void)> fn) {
  update = fn;
}

