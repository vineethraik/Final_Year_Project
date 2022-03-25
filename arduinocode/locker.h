#include "libs.h"

#define mem_addr 100;

//vector<vector<String >> os;



struct os_base{
  vector<String> options;
  int disp_offset=0;
};

class Locker{
  private:
  function<void(void)> update=nullptr;
  vector<os_base> os;
  int os_state[2]={0,0};
  void init_Locker();
  public:
  struct{
    int initialized=-1;
    String device_id="";
    struct{
    int is_registered=-1;
    String id="";
    String phone="";
    String name="";
    String nickname="";
    String email="";
    String hash="";
  }data[3];
  }data;

  Locker();
  void readmem();
  void writemem();
  void run_os();
  void init();
  void addupdatefunction(function<void(void)>);
  friend void init_Locker(Locker);

  private:
  void reset_os_offset();

  private:         // Action functions functions
  void handle_unlock();

  void handle_ota();

  void handle_update_time();
  void handle_print_time();

  void handle_connect_wifi();
  void handle_print_connection();
  void handle_WiFi_manager();

  void handle_clear_memory();
  void handle_clear_memory_individul();
  void handle_read_hash();
  
  void handle_test_button();
  void handle_test_pin();
  void handle_test_display();
  void handle_test_number();
  

}locker;



void Locker::init_Locker(){
  os.clear();
                                                                  //0
  os_base temp;                 
  temp.options.clear();
  temp.options.push_back("Unlock");
  temp.options.push_back("Settings");
  os.push_back(temp);

                                                                  //1
  temp.options.clear();
  temp.options.push_back("Time");
  temp.options.push_back("OTA");
  temp.options.push_back("WiFi");
  temp.options.push_back("Dev");
  temp.options.push_back("IO");
  os.push_back(temp);

                                                                  //2
  temp.options.clear();
  temp.options.push_back("Update");
  temp.options.push_back("Print");
  os.push_back(temp);

                                                                  //3
  temp.options.clear();
  temp.options.push_back("Connect");
  temp.options.push_back("Print");
  temp.options.push_back("WiFi manager");
  os.push_back(temp);

                                                                  //4
  temp.options.clear();
  temp.options.push_back("Clear memory");
  temp.options.push_back("Clear a user");
  temp.options.push_back("Restart");
  temp.options.push_back("Reset");
  temp.options.push_back("Read Hash");
  os.push_back(temp);

                                                                  //5
  temp.options.clear();
  temp.options.push_back("Test buttons");
  temp.options.push_back("Test pin");
  temp.options.push_back("Test display");
  temp.options.push_back("Test number");
  
  os.push_back(temp);
}

Locker::Locker(){
  init_Locker();
  EEPROM.begin(1024*100);
}

void Locker::readmem(){
  
}

void Locker::writemem(){
  
}

void Locker::init(){
  ota.init();
  disp.init(0x3c);
  wifi.init();
}

void Locker::addupdatefunction(function<void(void)> fn){
  update=fn;
  wifi.addupdatefunction(fn);
  io.addupdatefunction(fn);
  TIME.addupdatefunction(fn);
}

void Locker::run_os(){
  if(os_state[0]<os.size()){
    disp.clear();
  for(int i=os[os_state[0]].disp_offset;i<os[os_state[0]].options.size();i++){
    if(i==os_state[1]){
      disp.println_selection(os[os_state[0]].options[i]);
    }else{
      disp.println(os[os_state[0]].options[i]);
    }
    disp.println();
  }}else{
    os_state[0]=0;
    os_state[1]=0;
  }

while(true){
  if(io.check_back()){
    reset_os_offset();
    os_state[0]=(os_state[0]==0)?0:os_state[0];
    os_state[0]=(os_state[0]==1)?0:os_state[0];
    os_state[0]=(os_state[0]==2)?1:os_state[0];
    os_state[0]=(os_state[0]==3)?1:os_state[0];
    os_state[0]=(os_state[0]==4)?1:os_state[0];
    os_state[0]=(os_state[0]==5)?1:os_state[0];
    os_state[1]=0;
    return;                 
  }

  if(io.check_ok()){
    reset_os_offset();
    if(os_state[0]==0){
      if(os_state[1]==0)handle_unlock();
      os_state[0]=(os_state[1]==1)?1:os_state[0];
    }else if(os_state[0]==1){
      os_state[0]=(os_state[1]==0)?2:os_state[0];
      if(os_state[1]==1)handle_ota();
      os_state[0]=(os_state[1]==2)?3:os_state[0];
      os_state[0]=(os_state[1]==3)?4:os_state[0];
      os_state[0]=(os_state[1]==4)?5:os_state[0];
    }else if(os_state[0]==2){
      if(os_state[1]==0)handle_update_time();
      if(os_state[1]==1)handle_print_time();
    }else if(os_state[0]==3){
      if(os_state[1]==0)handle_connect_wifi();
      if(os_state[1]==1)handle_print_connection();
      if(os_state[1]==2)handle_WiFi_manager();
    }else if(os_state[0]==4){
      if(os_state[1]==0)handle_clear_memory();
      if(os_state[1]==1)handle_clear_memory_individul();
      if(os_state[1]==2)ESP.restart();
      if(os_state[1]==3)ESP.reset();
      if(os_state[1]==4)handle_read_hash();
    }else if(os_state[0]==5){
      if(os_state[1]==0)handle_test_button();
      if(os_state[1]==1)handle_test_pin();
      if(os_state[1]==2)handle_test_display();
      if(os_state[1]==3)handle_test_number();
      
    }else{
      os_state[0]=0;
    }
    os_state[1]=0;
    return;

  }

  if(io.check_up()){
    if(os_state[1]>0){
      os_state[1]--;
      if(os_state[1]<os[os_state[0]].disp_offset)os[os_state[0]].disp_offset--;
    }
    return;
  }

  if(io.check_down()){
    if(os_state[1]<(os[os_state[0]].options.size()-1)){
      os_state[1]++;
      if(os_state[1]>(os[os_state[0]].disp_offset+3))os[os_state[0]].disp_offset++;
    }
    return;
  }
  update();
}

}


void Locker::reset_os_offset(){
  for(int i=0;i<os.size();i++){
    os[os_state[i]].disp_offset=0;
  }
}

/////////////////////////////////////////////////////handler functions



void Locker::handle_unlock(){
  disp.clear();
  disp.println("Unlock is currently \nunavailable");
  while(true){
    update();
    if(io.check_back()){
      break;
    }
  }
}

void Locker::handle_ota(){
  disp.clear();
  //handle_connect_wifi();
  disp.clear();
  wifi.printconnection();
  disp.println("\nOTA is active");
  while(true){
    ota.handle();
    update();
    if(io.check_back()){
      break;
    }
  }
}

void Locker::handle_update_time(){
  int offset=19800;
  long i=millis(),j=500;
  disp.clear();
  disp.print("Connecting to wifi");
  wifi.connect();
  wifi.printconnection();
  delay(AVGDELAY);
  //disp.clear();
  timeClient.begin();

  while(true){
    update();
    if(io.check_back()){
      break;
    }

    if((millis()-i)>j){
      i=millis();
      disp.clear();
      disp.refresh=false;
      timeClient.update();
      disp.println("NTP time : "+timeClient.getFormattedTime());
      disp.println("NTP Date : "+String(timeClient.getDay()));
      disp.println();
      disp.println("RTC time : "+TIME.gettime());
      disp.println();
      disp.refresh=true;
      disp.print("OFFSET : "+String(offset));
    }

    if(io.check_ok()){
      TIME.update_time(timeClient.getHours(),timeClient.getMinutes(),timeClient.getSeconds());
    }

    if(io.check_up()){
      offset++;
      timeClient.setTimeOffset(offset);
    }

    if(io.check_down()){
      offset--;
      timeClient.setTimeOffset(offset);
    }
    



  }
  timeClient.end();
}

void Locker::handle_print_time(){
  while(true){
    update();
    if(io.check_back()){
      break;
    }
    
      TIME.print();
     
    
  }
}

void Locker::handle_connect_wifi(){
  disp.clear();
  disp.println("Connecting to wifi");
  
  if(wifi.connect(true)){
    disp.clear();
  disp.println("Connected to:\n");
  disp.print(WiFi.SSID(),2);
  delay(AVGDELAY);
  }
  
  
  
}

void Locker::handle_print_connection(){
  wifi.printconnection();
  while(true){
    update();
    if(io.check_back()){
      break;
    }
  }
}

void Locker::handle_WiFi_manager(){
  disp.clear();
  disp.println("Wifi Manager");
  disp.println("facility is currently\nunavailable");
  while(true){
    update();
    if(io.check_back()){
      break;
    }
  }
}

void Locker::handle_clear_memory(){
  disp.clear();
  disp.println("currently \n unavailable");
  while(true){
    update();
    if(io.check_back()){
      break;
    }
  }
}

void Locker::handle_clear_memory_individul(){
  disp.clear();
  disp.println("currently \n unavailable");
  while(true){
    update();
    if(io.check_back()){
      break;
    }
  }
}

void Locker::handle_read_hash(){
  disp.clear();
  disp.println("currently \n unavailable");
  while(true){
    update();
    if(io.check_back()){
      break;
    }
  }
}

void Locker::handle_test_button(){
  disp.clear();
  while(true){
    update();
    disp.clear();
    disp.println("Press a button:");
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
    disp.print("do you wanna exit");
    while(true){
      update();
      if(io.check_ok()){
        disp.clear();
        return;
      }
      
      if(io.check_back()){
        disp.clear();
        disp.print("BACK pressed");
        break;
      }
    }
    break;
  }
  delay(AVGDELAY);

  }
}

void Locker::handle_test_pin(){
  String s;
  while(true){
    update();
    disp.clear();
    disp.print("PIN :");
    s=io.read_pin();
    disp.print("\n\nthe pin u entered:-\n"+s);
    delay(2000);
    if(io.check_back()){
      disp.clear();
      return;
    }
  }
}

void Locker::handle_test_display(){
  disp.clear();
  disp.print();
  bool xaxis=false;
  int i=0,j=0;
  
  while(true){
    update();
    disp.clear();
    disp.oled.fillRect(0,10,i,j,1);
    disp.print(String(i)+":"+String(j));
    if(io.check_back()){
      break;
    }
    if(io.check_ok()){
      xaxis=!xaxis;
    }

    if(io.check_up()){
      if(xaxis){
        (i>0)?i--:i+=0;
      }else{
        (j>0)?j--:j+=0;
      }

    }

    if(io.check_down()){
      if(xaxis){
        (i<128)?i++:i+=0;
      }else{
        (j<64)?j++:j+=0;
      }

    }


  }
  disp.clear();
}

void Locker::handle_test_number(){
  String s;
  while(true){
    update();
    disp.clear();
    disp.print("NUMBER :");
    s=io.read_number();
    disp.print("\n\nthe number u entered:-\n"+s);
    delay(2000);
    if(io.check_back()){
      disp.clear();
      return;
    }
  }
}


