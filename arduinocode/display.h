#include "libs.h"



class screen{
  public:
  Adafruit_SH1106G oled=Adafruit_SH1106G(128,64 , &Wire, -1);
  static bool progress;
  static bool clear_display;
  static bool refresh;
  
  void init(char,bool);
  void logo();
  void println(String,int);
  void println_selection(String,int);
  void print(String,int);
  
  void printprogress(String,int);
  void clear();
}disp;

bool screen::progress=false;
bool screen::clear_display=false;
bool screen::refresh=true;

void screen::init(char addr=0x3c,bool value=true){
  oled.begin(addr,value);
  oled.setContrast(0);
  oled.setTextSize(1);
  oled.setTextColor(SH110X_WHITE);
}

void screen::logo(){
  oled.drawBitmap(128 / 2 - 41 / 2, 64 / 2 - 41 / 2,  logo2, 41, 41, 1);
  oled.display();
  delay(1000);
  /*for(int i=0;i<128;i++){
    oled.drawLine(i,0,i,oled.width()-1,SH110X_BLACK);
    delay(10);
    oled.display();
  }*/

  for(int i=oled.height()/2;i>=0;i--){
    oled.drawCircle(oled.width()/2+1,oled.height()/2,i,SH110X_BLACK);
    oled.drawCircle(oled.width()/2-1,oled.height()/2,i,SH110X_BLACK);
    oled.drawCircle(oled.width()/2,oled.height()/2+1,i,SH110X_BLACK);
    oled.drawCircle(oled.width()/2,oled.height()/2-1,i,SH110X_BLACK);
    delay(50);
    oled.display();
  }
  delay(500);
  
}

void screen::println(String str="",int sz=1){
  if(!progress){
  oled.setTextSize(sz);
  oled.println(str);
  if(refresh)
  oled.display();
  delay(1);
  }
  
}

void screen::println_selection(String str="",int sz=1){
  if(!progress){
  oled.setTextColor(SH110X_BLACK, SH110X_WHITE);
  oled.setTextSize(sz);
  oled.println(str);
  oled.setTextColor(SH110X_WHITE);
  oled.display();
  delay(1);
  }
  
}

void screen::print(String str="",int sz=1){
  if(!progress){
  oled.setTextSize(sz);
  oled.print(str);
  if(refresh)
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
  
  if(clear_display){
    oled.display();
  }
  
}
