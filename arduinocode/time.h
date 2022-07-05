#include "libs.h"

WiFiUDP ntpUDP;
static NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);

class Time {
private:
  function<void(void)> update = nullptr;
  DS1307 RTC;

public:
  Time();
  String gettime();
  String get_formated_time(int);
  String get_remainig_sec_from_next_min();
  void print();
  void update_time(int, int, int);
  void update_date(int, int, int, int);
  void addupdatefunction(function<void(void)>);
} TIME;

Time::Time() {
  RTC.begin();
}

String Time::gettime() {
  String temp;
  temp = "";
  temp += (RTC.getHours() < 24) ? String(RTC.getHours()) : "00";
  temp += ":";
  temp += (RTC.getMinutes() < 60) ? String(RTC.getMinutes()) : "00";
  temp += ":";
  temp += (RTC.getSeconds() < 60) ? String(RTC.getSeconds()) : "00";
  return temp;
}

String Time::get_formated_time(int min_offset = 0) {
  char buff[25];
  time_t t = RTC.getEpoch();
  t += (min_offset * 60);
  struct tm *ptm = gmtime((time_t *)&t);
  /**/
  int hour=(RTC.getHours()<24)?RTC.getHours():0;
  int minute=(RTC.getMinutes()<60)?RTC.getMinutes():0;
  int day=(RTC.getDay()<32)?RTC.getDay():1;
  int month=(RTC.getMonth()<13)?RTC.getMonth():1;
  int year=(RTC.getYear()>2000)?RTC.getYear():0;
  /*
  int hour = ptm->tm_hour;
  int minute = ptm->tm_min;
  int day = ptm->tm_mday;
  int month = ptm->tm_mon;
  int year = ptm->tm_year;
  /**/

    minute+=min_offset;
  if(minute==60){
    minute=0;
    hour++;
    if(hour==24){
      hour=0;
    }
  }
  if(minute==-1){
    minute=59;
    hour--;
    if(hour==0){
      hour=23;
    }
  }

  sprintf(buff, "%02d:%02d:%02d:%02d:%04d", minute, hour, day, month, year);
  return String(buff);
}

String Time::get_remainig_sec_from_next_min() {
  char buff[15];
  sprintf(buff, "%2d Seconds left", 60 - RTC.getSeconds());
  return String(buff);
}

void Time::print() {
  disp.clear();
  disp.refresh = false;
  char buff[100];
  disp.println();
  switch (RTC.getWeek()) {
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
  sprintf(buff, " %02d/%02d/%04d", RTC.getDay(), RTC.getMonth(), RTC.getYear());
  //disp.print(" ");
  //disp.print(String(RTC.getDay()));
  //disp.print("/");
  //disp.print(String(RTC.getMonth()));
  //disp.print("/");
  //disp.print(String(RTC.getYear()));
  disp.println(String(buff));
  disp.println("\n");
  sprintf(buff, " %02d:%02d:%02d", RTC.getHours(), RTC.getMinutes(), RTC.getSeconds());
  //disp.print(String(RTC.getHours()),2);
  //disp.print(":",2);
  //disp.print(String(RTC.getMinutes()),2);
  //disp.print(":",2);
  //disp.refresh=true;
  //disp.print(String(RTC.getSeconds()),2);
  disp.print(String(buff), 2);
  disp.refresh = true;
  disp.print();
}

void Time::update_time(int hour, int min, int sec) {
  RTC.setHours(hour);
  RTC.setMinutes(min);
  RTC.setSeconds(sec);
}

void Time::update_date(int day, int month, int year, int week = -1) {
  RTC.setYear(year);
  RTC.setMonth(month);
  RTC.setDay(day);
  if (week >= 0) {
    RTC.setWeek(week + 1);
  }
}

void Time::addupdatefunction(function<void(void)> fn) {
  update = fn;
}