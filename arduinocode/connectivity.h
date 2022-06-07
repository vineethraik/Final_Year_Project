#include "libs.h"

ESP8266WiFiMulti wifiMulti;


class connectivity{
  private:
    function<void(void)> update=nullptr;
    
  public:
    enum event{EMAIL_OTP,MOBILE_OTP,EMAIL_NOTIFICATION,MOBILE_NOTIFICATION};

  public:
    void init();
    bool connect(bool);
    void printconnection();
    bool run(int);
    void addupdatefunction(function<void(void)>);

    String urlEncode(const char *msg);
    String urlEncode(String);
    String make_url(event,String,String,String);
    String request_url(String,int);

   
  
}wifi;


void connectivity::init(){
  WiFi.persistent(true);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(F("Locker"),"");
  #ifdef STASSID1
  wifiMulti.addAP(STASSID1,STAPSK1);
  #endif
  #ifdef STASSID2
  wifiMulti.addAP(STASSID2,STAPSK2);
  #endif
  #ifdef STASSID3
  wifiMulti.addAP(STASSID3,STAPSK3);
  #endif
  #ifdef STASSID4
  wifiMulti.addAP(STASSID4,STAPSK4);
  #endif
  #ifdef STASSID5
  wifiMulti.addAP(STASSID5,STAPSK5);
  #endif
  #ifdef STASSID6
  wifiMulti.addAP(STASSID6,STAPSK6);
  #endif

  
}

bool connectivity::connect(bool is_os_connect=false){
  while (wifiMulti.run(3000)!=WL_CONNECTED){
    delay(1);
    update();
    if(is_os_connect){
      if(io.check_back(false)){
        disp.clear();
        disp.print(F("process interupted"));
        delay(AVGDELAY);
        return false;
      }
    }
    }
    return true;
}

void connectivity::printconnection(){
  disp.clear();
  disp.println("SSID:"+WiFi.SSID());
  disp.println("\nSTAIP:"+WiFi.localIP().toString());
  disp.println("\nAPIP:"+WiFi.softAPIP().toString());
}

bool connectivity::run(int d=1){
  return wifiMulti.run(d);
}

void connectivity::addupdatefunction(function<void(void)> fn){
  update=fn;
}

String connectivity::urlEncode(const char *msg)
{
    const char *hex = "0123456789ABCDEF";
    String encodedMsg = "";

    while (*msg != '\0')
    {
        if (('a' <= *msg && *msg <= 'z') || ('A' <= *msg && *msg <= 'Z') || ('0' <= *msg && *msg <= '9') || *msg == '-' || *msg == '_' || *msg == '.' || *msg == '~')
        {
            encodedMsg += *msg;
        }
        else
        {
            encodedMsg += '%';
            encodedMsg += hex[*msg >> 4];
            encodedMsg += hex[*msg & 0xf];
        }
        msg++;
    }
    return encodedMsg;
}

String connectivity::urlEncode(String msg)
{
    return urlEncode(msg.c_str());
}

String connectivity::make_url(event evt,String value1,String value2,String value3){
  String ifttt_host=F("https://maker.ifttt.com/trigger/");
  String ifttt_key=KEY;
  String email_otp_event=F("locker_otp_mail");
  String mobile_otp_event=F("locker_otp_mobile");
  String email_notification_event=F("locker_mail_notification");
  String mobile_notification_event=F("locker_mobile_notification");

  String event="";

  switch(evt){
    case EMAIL_OTP:
      event=email_otp_event;
      break;
    case MOBILE_OTP:
      event=mobile_otp_event;
      break;
    case EMAIL_NOTIFICATION:
      event=email_notification_event;
      break;
    case MOBILE_NOTIFICATION:
      event=mobile_notification_event;
      break;
  }
  
  String url=ifttt_host;
  url+=event;
  url+=F("/with/key/");
  url+=ifttt_key;
  url+=F("?value1=");
  url+=urlEncode(value1);
  url+=F("&value2=");
  url+=urlEncode(value2);
  url+=F("&value3=");
  url+=urlEncode(value3);

  

  

  return String(url.c_str());
  
}

String connectivity::request_url(String _url,int timeout=1000){

  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

  client->setInsecure();

  client->setTimeout(timeout);

  HTTPClient https;

  if(https.begin(*client,_url)){
    
    int httpcode = https.GET();
    String responce="hi";
    if(httpcode == HTTP_CODE_OK || httpcode == HTTP_CODE_MOVED_PERMANENTLY){
      
      responce="("+String(httpcode)+"):"+https.getString();
    }else{
      responce="ERROR_CODE:"+httpcode;
      
    }
    https.end();
    delete &client;
    delete &https;
    return responce;
  }else{
    
    delete &client;
    delete &https;
    return F("NOT_ABLE_TO_CONNECT");
  }

  
}

