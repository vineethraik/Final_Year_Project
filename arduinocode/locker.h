#include "libs.h"



//vector<vector<String >> os;
ESP8266WebServer server(80);

#define user_length 3



struct os_base{
  vector<String> options;
  int disp_offset=0;
};

struct USER{
    int is_registered=-1;
    String id="";
    String phone="";
    String name="";
    String nickname="";
    String email="";
    String hash="";
};

class Locker{
  private:
  bool lock_state=true;
  function<void(void)> update=nullptr;
  vector<os_base> os;
  vector<int> os_state{0,0};
  void init_Locker();
  struct{
    int initialized=-1;
    String device_id="tamo";
    String device_name="Locker_x";
    USER user[user_length];
  }data;

  public:
  Locker();
  void readmem();
  void writemem();
  void run_os();
  void init();
  void addupdatefunction(function<void(void)>);

  private: //data read write functions
  String get_id();
  String get_name();
  USER* get_nonregistered_user();
  USER* get_all_users();
  USER* get_user_with_id(String);


  private:
  void unlock(bool,bool);
  void reset_os_offset();
  void init_Locker(Locker);

  public:
  void handle_server();

  private:         // Action functions
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
  void handle_dev_unlock();
  void handle_system_info();
  
  void handle_test_button();
  void handle_test_pin();
  void handle_test_display();
  void handle_test_number();
  void handle_test_util();


  private:         // server handlers
  static void handle_server_root();
  static void handle_server_ping();
  static void handle_server_add_user();
  static void handle_server_get_info();

}locker;



void Locker::init_Locker(){
  os.clear();
                                                                  //0
  os_base temp;                 
  temp.options.clear();
  temp.options.push_back(" Unlock ");
  temp.options.push_back(" Settings ");
  os.push_back(temp);

                                                                  //1
  temp.options.clear();
  temp.options.push_back(" Time ");
  temp.options.push_back(" OTA ");
  temp.options.push_back(" WiFi ");
  temp.options.push_back(" Dev ");
  temp.options.push_back(" IO ");
  os.push_back(temp);

                                                                  //2
  temp.options.clear();
  temp.options.push_back(" Update ");
  temp.options.push_back(" Print ");
  os.push_back(temp);

                                                                  //3
  temp.options.clear();
  temp.options.push_back(" Connect ");
  temp.options.push_back(" Print ");
  temp.options.push_back(" WiFi manager ");
  os.push_back(temp);

                                                                  //4
  temp.options.clear();
  temp.options.push_back(" Clear memory ");
  temp.options.push_back(" Clear a user ");
  temp.options.push_back(" Restart ");
  temp.options.push_back(" Reset ");
  temp.options.push_back(" Read Hash ");
  temp.options.push_back(" Unlock ");
  temp.options.push_back(" System Info ");
  os.push_back(temp);

                                                                  //5
  temp.options.clear();
  temp.options.push_back(" Test buttons ");
  temp.options.push_back(" Test pin ");
  temp.options.push_back(" Test display ");
  temp.options.push_back(" Test number ");
  temp.options.push_back(" Test utility ");
  
  os.push_back(temp);
}

Locker::Locker(){
  init_Locker();
  EEPROM.begin(1024*10);
}

void Locker::handle_server(){
  server.handleClient();
}

void Locker::readmem(){
  EEPROM.get(locker_data_addr,data);
}

void Locker::writemem(){
  EEPROM.put(locker_data_addr,data);
  EEPROM.commit();
}

void Locker::init(){
  ota.init();
  disp.init(0x3c);
  wifi.init();
  readmem();
  if(data.initialized==-1){
    data.device_id="1sdf5dg56";
    data.initialized=1;
    writemem();
  }

  server.begin();

  server.on("/",handle_server_root);
  server.on("/ping",handle_server_ping);
  server.on("/add-user",handle_server_add_user);
  server.on("/info",handle_server_get_info);
}

void Locker::addupdatefunction(function<void(void)> fn){
  update=fn;
  wifi.addupdatefunction(fn);
  io.addupdatefunction(fn);
  TIME.addupdatefunction(fn);
}

String Locker::get_id(){
  return data.device_id;
}

String Locker::get_name(){
  return data.device_name;
}

USER* Locker::get_nonregistered_user(){
  int i;
  for(i=0;i<user_length;i++){
    if(data.user[i].is_registered==-1){
      break;
    }
  }

  if(i==user_length){
    return nullptr;
  }
  return &data.user[i];
}

USER* Locker::get_all_users(){
  return data.user;
}

USER* Locker::get_user_with_id(String _id){
  int i;
  for(i=0;i<user_length;i++){
    if(data.user[i].id.equals(_id)){
      break;
    }
  }

  if(i==user_length){
    return nullptr;
  }
  return &data.user[i];
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
  }
  }else{
    os_state[0]=0;
    os_state[1]=0;
    run_os();
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
    if(os_state[0]!=4&&os_state[0]!=5)
    reset_os_offset();
    if(os_state[0]==0){
      if(os_state[1]==0)handle_unlock();
      if(os_state[1]==1){os_state={1,0};}
    }else if(os_state[0]==1){
      if(os_state[1]==0){os_state={2,0};}
      if(os_state[1]==1)handle_ota();
      if(os_state[1]==2){os_state={3,0};}
      if(os_state[1]==3){os_state={4,0};}
      if(os_state[1]==4){os_state={5,0};}
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
      if(os_state[1]==5)handle_dev_unlock();
      if(os_state[1]==6)handle_system_info();
    }else if(os_state[0]==5){
      if(os_state[1]==0)handle_test_button();
      if(os_state[1]==1)handle_test_pin();
      if(os_state[1]==2)handle_test_display();
      if(os_state[1]==3)handle_test_number();
      if(os_state[1]==4)handle_test_util();
      
    }else{
      os_state[0]=0;
    }
    //os_state[1]=0;
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
    os[i].disp_offset=0;
  }
}

void Locker::unlock(bool unlock,bool isdev=false){
  if(isdev){
    disp.clear();
    if(unlock)
    disp.println("Unlocked",2);
    else
    disp.println("Locked",2);
    delay(2000);
  }else{

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
      disp.println("NTP Day : "+String(timeClient.getDay()));
      disp.println();
      disp.println("RTC time : "+TIME.gettime());
      disp.println();
      disp.refresh=true;
      disp.print("OFFSET : "+String(offset));
    }

    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime ((time_t *)&epochTime);

    if(io.check_ok()){
      TIME.update_time(timeClient.getHours(),timeClient.getMinutes(),timeClient.getSeconds());
      TIME.update_date(ptm->tm_mday,ptm->tm_mon+1,ptm->tm_year+1900,timeClient.getDay());
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
  disp.println("currently \nunavailable");
  while(true){
    update();
    if(io.check_back()){
      break;
    }
  }
}

void Locker::handle_dev_unlock(){
  disp.clear();
  static bool state=true;
  while(true){
    update();
    disp.clear();
    disp.print("Press OK to ");
    if(state){
      disp.println("Unlock");
    }else{
      disp.println("Lock");
    }
    disp.println("BACK to exit");
    
    while(true){
      update();
      if(io.check_back()){
      return;
      }
      if(io.check_ok()){
      unlock(state,true);
      state=!state;
      break;
      }
    }
  }
}

void Locker::handle_system_info(){
  disp.clear();
  int n=0;
  for(int i=0;i<user_length;i++){
    if(data.user[i].is_registered==1)
    n++;
  }
  disp.println("No of users:"+n+"out of"+user_length);
  for(int i=0,j=0,i<n;i++,j++){
    while(data.user[j].is_registered!=1){
      j++;
    }
    disp.println(i+"."+data.user[j].name);
  }
  disp.
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
    int d=2000;
    while(d--){
      delay(1);
      if(io.check_back()){
      disp.clear();
      return;
    }
      if(io.check_ok()){
      disp.clear();
      break;
    }
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
    disp.oled.fillRect(0,0,i,j,1);
    disp.println_selection(String(i)+":"+String(j));
    if(io.check_back()){
      break;
    }
    if(io.check_ok()){
      xaxis=!xaxis;
    }

    if(io.check_up(100)){
      if(xaxis){
        (i>0)?i--:i+=0;
      }else{
        (j>0)?j--:j+=0;
      }

    }

    if(io.check_down(100)){
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
    int d=2000;
    while(d--){
      delay(1);
      if(io.check_back()){
      disp.clear();
      return;
    }
      if(io.check_ok()){
      disp.clear();
      break;
    }
    }
  }
}

void Locker::handle_test_util(){
  while(true){
    String s;
    update();
    disp.clear();
    disp.print("string: ");
    s=util.get_rand_string(5);
    disp.println(s);
    disp.print("hash: ");
    s=util.get_hash(s);
    disp.println(s);
    disp.print("pin: ");
    s=util.get_pin_from_hex(s);
    disp.println(s);
    int d=2000;
    while(d--){
      update();
      delay(1);
      if(io.check_back()){
      disp.clear();
      return;
    }
      if(io.check_ok()){
      disp.clear();
      break;
    }
    }
  }
}
/////////////////////////////////////////////////////server handler functions

void Locker::handle_server_root(){
  server.send(200,"text/plain","Server Running");
}

void Locker::handle_server_ping(){
  server.send(200,"text/plain","OK");
}

void Locker::handle_server_add_user(){
  String temp;
  temp=server.arg("target");
  temp.toLowerCase();
  if(temp.equals("id")){
    temp=server.arg("id");
    USER* usr=locker.get_nonregistered_user();
    if(usr==nullptr){
      server.send(200,"text/plain","no_user_space_free");
      loop()
      return;
    }
    usr->id=temp;
    server.send(200,"text/plain","OK");

  }else if(temp.equals("user_info")){
    temp=server.arg("id");
    USER* usr=locker.get_user_with_id(temp);
    if(usr==nullptr){
      server.send(200,"text/plain","id_not_registered");
      return;
    }else{
      temp=server.arg("name");
      if(temp.equals("")){
        server.send(200,"text/plain","missing_values");
        return;
      }
      usr->name=temp;
      temp=server.arg("phone");
      if(temp.equals("")){
        server.send(200,"text/plain","missing_values");
        return;
      }
      usr->phone=temp;
      temp=server.arg("email");
      if(temp.equals("")){
        server.send(200,"text/plain","missing_values");
        return;
      }
      usr->email=temp;
      temp=server.arg("nickname");
      if(temp.equals("")){
        server.send(200,"text/plain","missing_values");
        return;
      }
      usr->nickname=temp;

      String r_str=util.get_rand_string();
      disp.clear();
      disp.println("Secret code:-");
      disp.println(r_str,2);
      usr->hash=util.get_hash(r_str);
      disp.println("press OK to verify");
      server.send(200,"text/plain","CODE_GENERATED");
      while(true){
        locker.update();
        delay(1);
        if(io.check_back()){
          disp.clear();
          //server.send(200,"text/plain","process_interupted");
          return;
        }
        if(io.check_ok()){
          disp.clear();
          break;
        }
      }
      reenter:
      disp.clear();
      disp.print("Enter the code:-");
      String code=io.read_pin();
      disp.clear();
      disp.println("code:-\n"+code);
      disp.print("press OK to confirm\npress BACK to reenter");

      while(true){
        locker.update();
        delay(1);
        if(io.check_back()){
          disp.clear();
          goto reenter;
        }
        if(io.check_ok()){
          disp.clear();
          disp.println("verifing passcode");
          break;
        }
      }
      if(code.equals(util.get_pin_from_hex(usr->hash))){
        usr->is_registered=1;
        //locker.writemem();
        //server.send(200,"text/plain","registration_successfull");
        disp.clear();
        disp.println("Successfull");
        disp.clear();
        delay(AVGDELAY);
        loop();
        return;
      }else{
        disp.clear();
        //server.send(200,"text/plain","registration_failed");
        disp.println("Wrong code");
        delay(AVGDELAY);
        loop();
        return;
      }

    }

  }else{
    server.send(200,"text/plain","INVALID_ARGUMENT");
  }
}

void Locker::handle_server_get_info(){
  String temp;
  temp=server.arg("q");
  temp.toLowerCase();
  if(temp.equals("id")){
    server.send(200,"text/plain",locker.data.device_id);

  }else if(temp.equals("time")){
    server.send(200,"text/plain",TIME.gettime());
  
  }else if(temp.equals("name")){
    server.send(200,"text/plain",locker.get_name());
  
  }else if(temp.equals("is_registered")){
    if(locker.get_user_with_id(server.arg("id"))->is_registered=1)
    server.send(200,"text/plain","YES");
    else
    server.send(200,"text/plain","NO");
  }else{
    server.send(200,"text/plain","INVALID_ARGUMENT");

  }
}

