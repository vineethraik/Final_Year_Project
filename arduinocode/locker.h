#include "libs.h"



//vector<vector<String >> os;
ESP8266WebServer server(80);
Servo lock_servo;

#define user_length 1

struct os_base{
  vector<String> options;
  int disp_offset=0;
};

class USER{
  public:
    int is_registered=-1;
    String id="";
    String phone="";
    String name="";
    String nickname="";
    String email="";
    String hash="";

    USER();
    void copy(USER);
};

USER::USER(){
  hash.reserve(45);
  email.reserve(40);
  nickname.reserve(30);
  name.reserve(25);
  id.reserve(20);
}

void USER::copy(USER temp){
  is_registered=-1;
  id=temp.id;
  phone=temp.phone;
  name=temp.name;
  nickname=temp.nickname;
  email=temp.email;
  hash=temp.hash;
}

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
    void handle_dev_unlock();
    void handle_system_info();
  
    void handle_test_button();
    void handle_test_pin();
    void handle_test_display();
    void handle_test_number();
    void handle_test_util();
    void handle_test_http();
    void handle_test_totp();


  private:         // server handlers
    
    static bool is_in_process;
    static void handle_server_root();
    static void handle_server_ping();
    static void handle_server_add_user();
    static void handle_server_get_info();
    #ifdef WEB_IO
    static void handle_server_io();
    #endif
    //

}locker;

bool Locker::is_in_process=false;

void Locker::init_Locker(){
  os.clear();
                                                                  //0
  os_base temp;                 
  temp.options.clear();
  temp.options.push_back(F(" Unlock "));
  temp.options.push_back(F(" Settings "));
  os.push_back(temp);

                                                                  //1
  temp.options.clear();
  temp.options.push_back(F(" Time "));
  temp.options.push_back(F(" OTA "));
  temp.options.push_back(F(" WiFi "));
  temp.options.push_back(F(" Dev "));
  temp.options.push_back(F(" Testing "));
  os.push_back(temp);

                                                                  //2
  temp.options.clear();
  temp.options.push_back(F(" Update "));
  temp.options.push_back(F(" Print "));
  os.push_back(temp);

                                                                  //3
  temp.options.clear();
  temp.options.push_back(F(" Connect "));
  temp.options.push_back(F(" Print "));
  temp.options.push_back(F(" WiFi manager "));
  os.push_back(temp);

                                                                  //4
  temp.options.clear();
  temp.options.push_back(F(" Clear memory "));
  temp.options.push_back(F(" Clear a user "));
  temp.options.push_back(F(" Restart "));
  temp.options.push_back(F(" Reset "));
  temp.options.push_back(F(" Unlock "));
  temp.options.push_back(F(" System Info "));
  os.push_back(temp);

                                                                  //5
  temp.options.clear();
  temp.options.push_back(F(" Test buttons "));
  temp.options.push_back(F(" Test pin "));
  temp.options.push_back(F(" Test display "));
  temp.options.push_back(F(" Test number "));
  temp.options.push_back(F(" Test utility "));
  temp.options.push_back(F(" Test http request "));
  temp.options.push_back(F(" Test TOTP "));
  
  os.push_back(temp);
}

Locker::Locker(){
  init_Locker();
  EEPROM.begin(1024*2);
}

void Locker::handle_server(){
  server.handleClient();
}

void Locker::readmem(){
struct usr{
    int is_registered=-1;
    char id[20]="";
    char phone[15]="";
    char name[40]="";
    char nickname[40]="";
    char email[40]="";
    char hash[45]="";
};

struct{
      int initialized=-1;
      char device_id[20]="tamo";
      char device_name[20]="Locker_x";
      usr user[user_length];
    }d;

  EEPROM.get(locker_data_addr,d);

  data.initialized=d.initialized;
  data.device_id=String(d.device_id);
  data.device_name=String(d.device_name);
  for(int i=0;i<user_length;i++){
    data.user[i].is_registered=d.user[i].is_registered;
    data.user[i].id=String(d.user[i].id);
    data.user[i].name=String(d.user[i].name);
    data.user[i].phone=String(d.user[i].phone);
    data.user[i].nickname=String(d.user[i].nickname);
    data.user[i].email=String(d.user[i].email);
    data.user[i].hash=String(d.user[i].hash);
  }
}

void Locker::writemem(){
  struct usr{
    int is_registered=-1;
    char id[20]="";
    char phone[15]="";
    char name[40]="";
    char nickname[40]="";
    char email[40]="";
    char hash[45]="";
  };

  struct{
      int initialized=-1;
      char device_id[20]="tamo";
      char device_name[20]="Locker_x";
      usr user[user_length];
  }d;

  d.initialized=data.initialized;
  data.device_id.toCharArray(d.device_id,data.device_id.length()+1);
  data.device_name.toCharArray(d.device_name,data.device_name.length()+1);

  for(int i=0;i<user_length;i++){
    d.user[i].is_registered=data.user[i].is_registered;
    data.user[i].id.toCharArray(d.user[i].id,data.user[i].id.length()+1);
    data.user[i].name.toCharArray(d.user[i].name,data.user[i].name.length()+1);
    data.user[i].phone.toCharArray(d.user[i].phone,data.user[i].phone.length()+1);
    data.user[i].nickname.toCharArray(d.user[i].nickname,data.user[i].nickname.length()+1);
    data.user[i].email.toCharArray(d.user[i].email,data.user[i].email.length()+1);
    data.user[i].hash.toCharArray(d.user[i].hash,data.user[i].hash.length()+1);

  }


  EEPROM.put(locker_data_addr,d);
  EEPROM.commit();
}

void Locker::init(){
  //ESP.wdtDisable();

  lock_servo.attach(D4);
  lock_servo.write(0);
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
  server.on("/io",handle_server_io);
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
      return &data.user[i];
    }
  }
  return nullptr;
  
  
}

USER* Locker::get_all_users(){
  return data.user;
}

USER* Locker::get_user_with_id(String _id){
  int i;
  for(i=0;i<user_length;i++){
    if(data.user[i].id.equals(_id)){
      return &data.user[i];
    }
  }

  
  return nullptr;
  
  
}

void Locker::run_os(){
  if(os_state[0]<os.size()){
    disp.clear();
    disp.refresh=false;
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

  disp.refresh=true;
  disp.print();
  

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
      if(os_state[1]==4)handle_dev_unlock();
      if(os_state[1]==5)handle_system_info();
    }else if(os_state[0]==5){
      if(os_state[1]==0)handle_test_button();
      if(os_state[1]==1)handle_test_pin();
      if(os_state[1]==2)handle_test_display();
      if(os_state[1]==3)handle_test_number();
      if(os_state[1]==4)handle_test_util();
      if(os_state[1]==5)handle_test_http();
      if(os_state[1]==6)handle_test_totp();
      
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
    if(unlock){
      disp.println(F("Unlocked"),2); 
      lock_servo.write(180);
    }
    else{
      disp.println(F("Locked"),2);
      lock_servo.write(0);
    }
    delay(2000);
  }else{

  }
  

}
/////////////////////////////////////////////////////handler functions



void Locker::handle_unlock(){
  disp.clear(true);
 /*
  disp.println("this feature is\nunder development");
  while(true){
      update();
      if(io.check_back()){
        return;
      }
  }
 /**/
  
  
  int registered_user_count=0;
  for(int i=0,j=0;i<user_length;i++){
    if(get_all_users()[i].is_registered==1){
      registered_user_count++;
      
      j++;
    }
  }
  if(registered_user_count!=user_length){
    disp.println(F("Registration incomplete\n\nPress OK to UNLOCK\nPress BACK to exit"));
    while(true){
      update ();
      if(io.check_back()){
        return;
      }else if(io.check_ok()){
        unlock(true);
        return;
      }
  }
   
  }else{
    disp.println(F("Sending otp to:-"));
    for(int i=0;i<registered_user_count;i++){
      disp.println(i+". "+get_all_users()[i].name);
    }
    for(int i=0;i<registered_user_count;i++){
      disp.println(F("Se"));
    }
    while(true){
      update();
      if(io.check_back()){
        return;
      }
    }
  }

  
}

void Locker::handle_ota(){
  disp.clear();
  //handle_connect_wifi();
  disp.clear();
  wifi.printconnection();
  disp.println(F("\nOTA is active"));
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
  unsigned long i=millis(),j=500;
  disp.clear();
  disp.print(F("Connecting to wifi"));
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
      disp.println(F("NTP time : ")+timeClient.getFormattedTime());
      disp.println(F("NTP Day : ")+String(timeClient.getDay()));
      disp.println();
      disp.println(F("RTC time : ")+TIME.gettime());
      disp.println();
      disp.refresh=true;
      disp.print(F("OFFSET : ")+String(offset));
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
  disp.println(F("Connecting to wifi"));
  
  if(wifi.connect(true)){
    disp.clear();
  disp.println(F("Connected to:\n"));
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
  disp.println(F("Wifi Manager"));
  disp.println(F("facility is currently\nunavailable"));
  while(true){
    update();
    if(io.check_back()){
      break;
    }
  }
}

void Locker::handle_clear_memory(){
  disp.clear();
  disp.println(F("currently \n unavailable"));
  while(true){
    update();
    if(io.check_back()){
      break;
    }
  }
}

void Locker::handle_clear_memory_individul(){
  disp.clear();
  readmem();
  int n=0;
  for(int i=0;i<user_length;i++){
    if(data.user[i].is_registered==1)
    n++;
  }
  disp.println("No of users:"+String(n)+"out of"+user_length);
  for(int i=0;i<n;i++){
    while(data.user[i].is_registered!=1){
      i++;
    }
    disp.println((i+1)+"."+data.user[i].name);
  }
  disp.println(F("\nEnter respective \nnumber to remove or 0 to exit"));
  disp.println(F("your choice:-"));
  String choice=io.read_number(1);
  disp.clear();
  if(choice=="nill")return;
  int index=choice.toInt()-1;
  if(index==-1)return;

  disp.println(F("Removing user:")+data.user[index].name+"\n");
  disp.println(F("Press ok to remove\nPress back to skip"));
  while(true){
    update();
    if(io.check_ok()){
      data.user[index].is_registered=-1;
      writemem();
      return;
    }

    if(io.check_back()){
      return;
    }
  }
  
}

void Locker::handle_dev_unlock(){
  disp.clear();
  static bool state=true;
  while(true){
    update();
    disp.clear();
    disp.print(F("Press OK to "));
    if(state){
      disp.println(F("Unlock"));
    }else{
      disp.println(F("Lock"));
    }
    disp.println(F("BACK to exit"));
    
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
  disp.println(F("Info"));
  readmem();
  vector<int> link(user_length,-1);
  int j=1,n=0;
  for(int i=0;i<user_length;i++){
    if(data.user[i].is_registered==1){
      disp.println(String(j)+"."+data.user[i].name);
      link[j-1]=i;
      j++;
      n++;
    }
    
  }
  disp.println(F("\nEnter respective \nnumber or 0 to exit"));
  disp.println(F("your choice:-"));
  String choice=io.read_number(1);
  disp.clear();
  if(choice=="nill")return;
  int index=choice.toInt()-1;
  if(index==-1)return;
  if(index>n-1){handle_system_info();return;}
  index=link[index];
  disp.println(data.user[index].id);
  disp.println(data.user[index].name);
  disp.println(data.user[index].nickname);
  disp.println(data.user[index].email);
  disp.println(data.user[index].phone);
  disp.println(data.user[index].hash);
  while(true){
    update();
    
    if(io.check_ok()){
      handle_system_info();
      return;
    }

    if(io.check_back()){
      return;
    }
  }
}

void Locker::handle_test_button(){
  disp.clear();
  while(true){
    update();
    disp.clear();
    disp.println(F("Press a button:"));
    switch(io.read_switch()){
    case io.NILL:
    disp.clear();
    disp.print(F("no switch pressed"));
    break;

    case io.OK:
    disp.clear();
    disp.print(F("OK pressed"));
    break;

    case io.UP:
    disp.clear();
    disp.print(F("UP pressed"));
    break;

    case io.DOWN:
    disp.clear();
    disp.print(F("DOWN pressed"));
    break;

    case io.BACK:
    disp.clear();
    disp.print(F("do you wanna exit"));
    while(true){
      update();
      if(io.check_ok()){
        disp.clear();
        return;
      }
      
      if(io.check_back()){
        disp.clear();
        disp.print(F("BACK pressed"));
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
    disp.print(F("PIN :"));
    s=io.read_pin();
    disp.print(F("\n\nthe pin u entered:-\n")+s);
    int d=2000;
    while(d--){
      delay(1);
      update();
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
    disp.print(F("NUMBER :"));
    s=io.read_number();
    disp.print(F("\n\nthe number u entered:-\n")+s);
    int d=2000;
    while(d--){
      delay(1);
      update();
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
    disp.print(F("string: "));
    s=util.get_rand_string(5);
    disp.println(s);
    disp.print(F("hash: "));
    s=util.get_hash(s);
    disp.println(s);
    disp.print(F("pin: "));
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

void Locker::handle_test_http(){
  
  disp.clear();
  if(WiFi.status()!=WL_CONNECTED){
    handle_connect_wifi();
  }
  disp.clear();
  //disp.print("Enter phone number\n:");
  //String phone=io.read_number(10);
  disp.clear();
  //String temp=wifi.make_url("locker_mobile_notification",phone,"Locker","hello,a test notification from locker");
  //delay(10000);
  //String responce="wifi.request_url(temp)";
  //disp.println("\nResponce:-"+responce);
  //
  //disp.println(temp);
  //while(!io.check_ok())update();
  disp.clear();
  String responce;//=wifi.request_url(temp);
  //disp.println("responce1:-"+responce);
  //delay(5000);
 //responce = wifi.request_url(wifi.make_url("locker_mail_notification","vineethraik23082000@gmail.com","Locker","notification sent to mobile number:7337688347"));
 responce = wifi.request_url(wifi.make_url(wifi.EMAIL_NOTIFICATION,F("vineethraik23082000@gmail.com"),F("Locker"),F("notification sent to mobile number:7337688347")));
  disp.clear();
  disp.println(F("responce2:-")+responce);
  //disp.println("mail sent to vineethraik23082000@gmail.com about the message\n:"+responce);
  
  while(true){
    update();

    if(io.check_ok()){
      handle_test_http();
      return;
    }
    if(io.check_back()){
      return;
    }
  }

}

void Locker::handle_test_totp(){
  disp.clear();
  disp.println(F("Users"));
  readmem();
  int link[user_length];
  int user_count=0;
  int j=1,n=0;
  for(int i=0;i<user_length;i++){
    if(data.user[i].is_registered==1){
      disp.println(String(j)+"."+data.user[i].name);
      link[j-1]=i;
      user_count++;
      j++;
      n++;
    }
    
  }

  if(user_count==0){
    disp.clear();
    disp.print(F("no registered user\nfound"));
    delay(1000);
    return;
  }
  disp.println(F("\nEnter respective \nnumber or 0 to exit"));
  disp.println(F("your choice:-"));
  String choice=io.read_number(1);
  disp.clear();
  if(choice=="nill")return;
  int index=choice.toInt()-1;
  if(index==-1)return;
  if(index>n-1){handle_test_totp();return;}
  index=link[index];

  char pin[]{'0','0','0','0','0','0','\0'};

  for(int i=0;i<6;i++){
    pin[i]=(char)((random(0,10000)%10)+48);
  }
unsigned long t=millis();
  while(true){
    update();

    if((millis()-t)>1000){
      t=millis();
      
      disp.clear();
      disp.refresh=false;
      disp.println(data.user[index].name);
      disp.print("OTP: ");
      disp.println(pin,2);
      disp.print("TOTP: ");
      disp.println(util.get_pin_from_hex(util.get_hash(data.user[index].hash+":"+pin+TIME.get_formated_time())),2);
      disp.refresh=true;
      disp.println(TIME.get_remainig_sec_from_next_min());
    }

    if(io.check_ok()){
      handle_test_totp();
      return;
    }

    if(io.check_back()){
      return;
    }
  }
}
/////////////////////////////////////////////////////server handler functions

void Locker::handle_server_root(){
  server.send(200,F("text/plain"),F("Server Running"));
}

void Locker::handle_server_ping(){
  server.send(200,F("text/plain"),F("OK"));
}

void Locker::handle_server_add_user(){
  if(!is_in_process)
  {disp.save_buffer();
  is_in_process=true;}
  String temp;
  temp=server.arg(F("target"));
  temp.toLowerCase();
  if(temp.equals("id")){
    temp=server.arg("id");
    USER* usr=locker.get_nonregistered_user();
    if(usr==nullptr){
      server.send(200,F("text/plain"),F("no_user_space_free"));
      disp.load_buffer();
      is_in_process=false;
      return;
    }
    usr->id=temp;
    server.send(200,F("text/plain"),"OK");

  }else if(temp.equals(F("user_info"))){
    temp=server.arg("id");
    USER* usr=locker.get_user_with_id(temp);
    if(usr==nullptr){
      server.send(200,F("text/plain"),F("id_not_registered"));
      disp.load_buffer();
      is_in_process=false;
      return;
    }else{
      temp=server.arg("name");
      if(temp.equals("")){
        server.send(200,F("text/plain"),F("missing_values"));
        disp.load_buffer();
        is_in_process=false;
        return;
      }
      usr->name=temp;
      temp=server.arg(F("phone"));
      if(temp.equals("")){
        server.send(200,F("text/plain"),F("missing_values"));
        disp.load_buffer();
        is_in_process=false;
        return;
      }
      usr->phone=temp;
      temp=server.arg("email");
      if(temp.equals("")){
        server.send(200,F("text/plain"),F("missing_values"));
        disp.load_buffer();
        is_in_process=false;
        return;
      }
      usr->email=temp;
      temp=server.arg(F("nickname"));
      if(temp.equals("")){
        server.send(200,F("text/plain"),F("missing_values"));
        disp.load_buffer();
        is_in_process=false;
        return;
      }
      usr->nickname=temp;

      String r_str=util.get_rand_string();
      disp.clear();
      disp.println(F("Secret code:-"));
      disp.println(r_str,2);
      usr->hash=util.get_hash(r_str);
      disp.println(F("press OK to verify"));
      server.send(200,F("text/plain"),F("CODE_GENERATED"));
      while(true){
        locker.update();
        delay(1);
        if(io.check_back()){
          disp.clear();
          //server.send(200,F("text/plain"),"process_interupted");
          disp.load_buffer();
          is_in_process=false;
          return;
        }
        if(io.check_ok()){
          disp.clear();
          break;
        }
      }
      reenter:
      disp.clear();
      disp.print(F("Enter the code:-"));
      String code=io.read_pin();
      disp.clear();
      disp.println(F("code:-\n")+code);
      disp.print(F("press OK to confirm\npress BACK to reenter"));

      while(true){
        locker.update();
        delay(1);
        if(io.check_back()){
          disp.clear();
          goto reenter;
        }
        if(io.check_ok()){
          disp.clear();
          disp.println(F("verifing passcode"));
          break;
        }
      }
      if(code.equals(util.get_pin_from_hex(usr->hash))){
        usr->is_registered=1;
        locker.writemem();
        //server.send(200,F("text/plain"),"registration_successfull");
        disp.clear();
        disp.println(F("Successfull"));
        disp.clear();
        delay(AVGDELAY);
        disp.load_buffer();
        is_in_process=false;
        return;
      }else{
        disp.clear();
        //server.send(200,F("text/plain"),"registration_failed");
        disp.println(F("Wrong code"));
        delay(AVGDELAY);
        disp.load_buffer();
        is_in_process=false;
        return;
      }

    }

  }else{
    server.send(200,F("text/plain"),F("INVALID_ARGUMENT"));
  }
}

void Locker::handle_server_get_info(){
  String temp;
  temp=server.arg("q");
  temp.toLowerCase();
  if(temp.equals("id")){
    server.send(200,F("text/plain"),locker.data.device_id);

  }else if(temp.equals("time")){
    server.send(200,F("text/plain"),TIME.gettime());
  
  }else if(temp.equals("name")){
    server.send(200,F("text/plain"),locker.get_name());
  
  }else if(temp.equals(F("is_registered"))){
    if(locker.get_user_with_id(server.arg("id"))->is_registered==1)
    server.send(200,F("text/plain"),"YES");
    else
    server.send(200,F("text/plain"),"NO");
  }else{
    server.send(200,F("text/plain"),F("INVALID_ARGUMENT"));

  }
}

#ifdef WEB_IO
void Locker::handle_server_io(){
  String temp;
  temp=server.arg("q");
  temp.toLowerCase();
  if(temp.equals("keypad")){
    temp=server.arg("value");
    temp.toLowerCase();
    temp.trim();
    if((temp[0]>='0')&&(temp[0]<='9')){
      if(io.web_number_present){
        server.send(200,F("text/plain"),F("buffer_full"));
        return;
      }else{
        io.web_number_present=true;
        io.web_number=temp[0];
        server.send(200,F("text/plain"),F("ok"));
        return;
      }      
    }else{
      server.send(200,F("text/plain"),F("invalid_value"));
      return;
    }
  }else if(temp.equals("button")){
    temp=server.arg("value");
    temp.toLowerCase();
    temp.trim();
    input::state temp1=io.NILL;
    if(temp.equals("ok")){
      temp1=io.OK;
    }else if(temp.equals("back")){
      temp1=io.BACK;
    }else if(temp.equals("up")){
      temp1=io.UP;
    }else if(temp.equals("down")){
      temp1=io.DOWN;
    }

    if(temp1==io.NILL){
      server.send(200,F("text/plain"),F("invalid_value"));
      return;
    }else{
      if(io.web_button_present){
        server.send(200,F("text/plain"),F("buffer_full"));
        return;
      }else{
        io.web_button_present=true;
        io.web_button=temp1;
        server.send(200,F("text/plain"),F("ok"));
        return;

      }
    }
  }else if(temp.equals("clear")){
    temp=server.arg("target");
    temp.toLowerCase();
    temp.trim();
    if(temp.equals("keypad")){
      io.web_number_present=false;
      server.send(200,F("text/plain"),F("ok"));
    }else if(temp.equals("button")){
      io.web_button_present=false;
      server.send(200,F("text/plain"),F("ok"));
    }
  }
}
#endif

