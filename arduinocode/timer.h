#include "libs.h"

typedef function<bool(void)> TIMER_FUNCTION;

struct TIMER_UNIT {
  int id = 0;
  bool repeate = false;
  bool first_run = true;
  bool run = false;
  unsigned long reference_time = 0;
  int start_delay = 0;
  int repeate_delay = 0;
  TIMER_FUNCTION timer_function = nullptr;
};

class Timer {
private:
  vector<TIMER_UNIT> events;

public:
  void run();
  int set_new_event(TIMER_UNIT);
  int set_new_event(int, TIMER_FUNCTION);
  int set_new_event(int, int, TIMER_FUNCTION);
  void end_event(int);
  
  int get_new_id();
  TIMER_UNIT get_event(int);
} timer;

void Timer::run() {
  for (int i = 0; i < events.size(); i++) {
    if (events[i].run) {
      if (events[i].first_run) {
        if ((millis() - events[i].reference_time) > events[i].start_delay) {
          events[i].reference_time = millis();
          events[i].first_run = false;
          events[i].run = events[i].timer_function();
        }
      } else {
        if (events[i].repeate) {
          if ((millis() - events[i].reference_time) > events[i].repeate_delay) {
            events[i].reference_time = millis();
            events[i].run = events[i].timer_function();
          }
        }
      }
    }else{
      events.erase(events.begin() + i);
    }
  }
}

int Timer::set_new_event(TIMER_UNIT unit) {
  events.push_back(unit);
  return unit.id;
}

int Timer::set_new_event(int s_delay, TIMER_FUNCTION function) {
  TIMER_UNIT tm;
  tm.id = get_new_id();
  tm.run = true;
  tm.reference_time = millis();
  tm.start_delay = s_delay;
  tm.repeate = false;
  tm.repeate_delay = 0;
  tm.timer_function = function;

  set_new_event(tm);
  return tm.id;
}

int Timer::set_new_event(int s_delay, int r_delay, TIMER_FUNCTION function) {
  TIMER_UNIT tm;
  tm.id=get_new_id();
  tm.run=true;
  tm.reference_time=millis();
  tm.start_delay=s_delay;
  tm.repeate=true;
  tm.repeate_delay=r_delay;
  tm.timer_function=function;

  set_new_event(tm);
  return tm.id;
}

void Timer::end_event(int _id) {
  for (int i = 0; i < events.size(); i++) {
    if (events[i].id == _id) {
      events.erase(events.begin() + i);
    }
  }
}

int Timer::get_new_id() {
  int x = random(1, 60000);
  for (int i = 0; i < events.size(); i++) {
    if (events[i].id == x) {
      return get_new_id();
    }
  }
  return x;
}

TIMER_UNIT Timer::get_event(int id) {
  for (int i = 0; i < events.size(); i++) {
  }
  TIMER_UNIT tm;
  return tm;
}