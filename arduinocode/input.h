#include "libs.h"

class screen;

#define BACK_pin 1
#define OK_pin 2
#define UP_pin 3
#define DOWN_pin 0

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
  bool check_back();
  bool check_ok();
  bool check_down();
  bool check_up();
  String read_pin(int);
  String read_number();
  void addupdatefunction(function<void(void)>);
  void handle(int);
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

bool input::check_back(){
  if(io2.read(BACK_pin)){
    while(io2.read(BACK_pin))update();
    return true;
  }else{
    return false;
  }
  
}

bool input::check_ok(){
  if(io2.read(OK_pin)){
    while(io2.read(OK_pin))update();
    return true;
  }else{
    return false;
  }
  
}

bool input::check_up(){
  if(io2.read(UP_pin)){
    while(io2.read(UP_pin))update();
    return true;
  }else{
    return false;
  }
  
}

bool input::check_down(){
  if(io2.read(DOWN_pin)){
    while(io2.read(DOWN_pin))update();
    return true;
  }else{
    return false;
  }
  
}

String input::read_pin(int n = 6) {
  String pin = "";
  while (true) {
    delay(1);
    update();
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        io1.write(j, (i == j) ? 0 : 1);
      }
      
      if (io1.readButton(4) == 0) {
        while (io1.readButton(4) == 0) update();
        if (i != 3) {
          n--;
          pin += (i * 3 + 1);
          disp.print("*");
        }
      } else if (io1.readButton(5) == 0) {
        while (io1.readButton(5) == 0) update();
        n--;
        if (i == 3) {
          pin += 0;
          disp.print("*");
        } else {
          pin += (i * 3 + 2);
          disp.print("*");
        }
      } else if (io1.readButton(6) == 0) {
        while (io1.readButton(6) == 0) update();
        if (i != 3) {
          n--;
          pin += (i * 3 + 3);
          disp.print("*");
        }
      }
    }
    if (n < 1) break;
    if (check_back())return "nill";
  }
  return pin;
}

String input::read_number() {
  String pin = "";
  while (true) {
    delay(1);
    update();
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        io1.write(j, (i == j) ? 0 : 1);
      }
      
      if (io1.readButton(4) == 0) {
        while (io1.readButton(4) == 0) update();
        if (i != 3) {
          pin += (i * 3 + 1);
          disp.print(String((i * 3 + 1)));
        }
      } else if (io1.readButton(5) == 0) {
        while (io1.readButton(5) == 0) update();
        if (i == 3) {
          pin += 0;
          disp.print(String(0));
        } else {
          pin += (i * 3 + 2);
          disp.print(String((i * 3 + 2)));
        }
      } else if (io1.readButton(6) == 0) {
        while (io1.readButton(6) == 0) update();
        if (i != 3) {
          pin += (i * 3 + 3);
          disp.print(String((i * 3 + 3)));
        }
      }
    }
    
    if (check_back())return "nill";
    if(check_ok())break;
  }
  return pin;
}

void input::addupdatefunction(function<void(void)> fn) {
  update = fn;
}

void input::handle(int n=1){
  while((n--)>0){
    update();
    delay(1);
  }
  
}