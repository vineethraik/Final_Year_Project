
#include "libs.h"

WiFiUDP ntpUDP;
static NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);

class Time{
  private:
  function<void(void)> update=nullptr;
  DS1307 RTC;
  
  public:
  Time();
  String gettime();
  void print();
  void update_time(int,int,int);
  void addupdatefunction(function<void(void)>);
}TIME;

Time::Time(){
  RTC.begin();
}

String Time::gettime(){
  String temp;
  temp="";
  temp+=(RTC.getHours()<24)?String(RTC.getHours()):"00";
  temp+=":";
  temp+=(RTC.getMinutes()<60)?String(RTC.getMinutes()):"00";
  temp+=":";
  temp+=(RTC.getSeconds()<60)?String(RTC.getSeconds()):"00";
  return temp;
}

void Time::print(){
  disp.clear();
  disp.refresh=false;
  
  switch (RTC.getWeek())
  {
    case 1:
      disp.print("SUN");
      break;
    case 2:
      disp.print("MON");
      break;
    case 3:
      disp.print("TUE");
      break;
    case 4:
      disp.print("WED");
      break;
    case 5:
      disp.print("THU");
      break;
    case 6:
      disp.print("FRI");
      break;
    case 7:
      disp.print("SAT");
      break;
  }

  disp.print(" ");
  disp.print(String(RTC.getDay()));
  disp.print("-");
  disp.print(String(RTC.getMonth()));
  disp.print("-");
  disp.print(String(RTC.getYear()));
  disp.println("",2);
  disp.print(String(RTC.getHours()),2);
  disp.print(":",2);
  disp.print(String(RTC.getMinutes()),2);
  disp.print(":",2);
  disp.refresh=true;
  disp.print(String(RTC.getSeconds()),2);
}

void Time::update_time(int hour,int min,int sec){
  RTC.setHours(hour);
  RTC.setMinutes(min);
  RTC.setSeconds(sec);
}

void Time::addupdatefunction(function<void(void)> fn){
update=fn;
}