#define display_h

#include "libs.h"



class screen{
  public:
  Adafruit_SH1106G oled=Adafruit_SH1106G(128,64 , &Wire, -1);
  static bool progress;
  static bool clear_display;
  
  void init(char,bool);
  void logo();
  void println(String,int);
  void print(String,int);
  void printprogress(String,int);
  void clear();
}disp;

bool screen::progress=false;
bool screen::clear_display=false;

void screen::init(char addr,bool value=true){
  oled.begin(addr,value);
  oled.setContrast(0);
}

void screen::logo(){
  oled.drawBitmap(128 / 2 - 41 / 2, 64 / 2 - 41 / 2,  logo2, 41, 41, 1);
  oled.display();
  for(int i=0;i<128;i++){
    oled.drawLine(i,0,i,oled.width()-1,SH110X_BLACK);
    delay(10);
    oled.display();
  }
  
}

void screen::println(String str="",int sz=1){
  if(!progress){
  oled.setTextSize(sz);
  oled.println(str);
  oled.display();
  delay(1);
  }
  
}

void screen::print(String str="",int sz=1){
  if(!progress){
  oled.setTextSize(sz);
  oled.print(str);
  oled.display();
  delay(1);
  }
}

void screen::printprogress(String str="",int sz=1){
  
  oled.setTextSize(sz);
  oled.print(str);
  oled.display();
  delay(1);
  
}

void screen::clear(){
  oled.clearDisplay();
  oled.setCursor(0,0);
  oled.setTextSize(1);
  oled.setTextColor(SH110X_WHITE);
  if(clear_display){
    oled.display();
  }
  
}
