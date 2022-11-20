
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>

AsyncWebServer server(80);

const char* ssid = "Dorma";
const char* password = "nahipata";

const char* username = "username";
const char* pin = "pin";

const char* set_user = "set_user";
const char* set_pass = "set_pass";

String inp_pin;
String inp_username;
String userpass;
String amount;
String trans_act;
String set_password;
String set_username;
int credited;
int debited;
int current_balance;
String prev_balance;
String  html_summary;
String flag="" ;
String un="";

String new_password;
int otp_no = random(1111,9999);
String otp =String(otp_no);
String otp_inp;
int a ;
String dir ="'/select_wo?otp="+otp+"'";


String GOOGLE_SCRIPT_ID = "AKfycbx3RS80L5Pe2wwOxQwM6BMY5THz923qSzUauvWyWwBRbaO_blNiS6bRdMDfKXQqqpZn5Q";


void get(String c){
   if (c=="user_pass"){
   HTTPClient http;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?cmd=user_pass&un="+un;

	http.begin(url.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
	int httpCode = http.GET();
    if (httpCode > 0) { 
        userpass = http.getString();
        Serial.println("Userpass : "+userpass);
      }
	http.end();
   }

  if(c=="set_user"){
     HTTPClient http;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?cmd=set_user"+"&set_username="+set_username+"&set_pass="+set_password;
	Serial.print("saving username");
	http.begin(url.c_str()); 
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
	int httpCode = http.GET();
    if (httpCode > 0) { 
        Serial.println("New user seted");
      }
	http.end();

  }

   if(c=="get_un"){
     HTTPClient http;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?cmd=get_un"+"&user="+inp_username;

	http.begin(url.c_str()); 
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
	int httpCode = http.GET();
    if (httpCode > 0) { 
        un = http.getString();
  
      }
	http.end();

  }

  if(c=="get_bal"){

  HTTPClient http;
  String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?cmd=get_bal&un="+un;

	http.begin(url.c_str()); 
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
	int httpCode = http.GET();
  prev_balance=http.getString();
  Serial.println("balance  "+prev_balance);
	http.end();
  }

  if (c=="send"){
     HTTPClient http;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?cmd=send&prev_b=" + prev_balance + "&debited=" + String(debited)+ "&credited=" + String(credited)+"&current_b=" + String(current_balance)+"&user="+inp_username;
   
	http.begin(url.c_str()); 
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
	int httpCode = http.GET();
	http.end();

  }

   if(c=="new_pass"){

  HTTPClient http;
  String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?cmd=up_pass&new_p="+new_password+"&un="+un;

	http.begin(url.c_str()); 
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
	int httpCode = http.GET();

  Serial.println("updated password ");
	http.end();
  }

 if(c=="up_bal"){

  HTTPClient http;
  String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?cmd=up_bal&bal="+String(current_balance)+"&un="+un;

	http.begin(url.c_str()); 
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
	int httpCode = http.GET(); 
  Serial.println("balance updated "+current_balance);
	http.end();
  }
 }

void cal_details(){
 if (trans_act=="debit"){
   get("get_bal");
   current_balance = prev_balance.toInt() - debited;
   }
   if (trans_act=="credit"){
    get("get_bal");
   current_balance = prev_balance.toInt() + credited;
   }
   get("send");   
   Serial.println("=====================TRANSCATION SUMMARY=======================");
   Serial.println("Previous balance: "+String(prev_balance));
  Serial.println("Credited: "+String(credited));
  Serial.println("Debited: "+String(debited));
  Serial.println("Current balance: "+String(current_balance));
   get("up_bal");
}

const char  html_login[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP32 ATM</title>
  </head><body>
  <h2>ATM with ESP32 (BT20ECE036) </h2> 
   <form action="/otp">
    <label>Username:</label>
     <input type="text" name="username">

    <label>Password:</label>
     <input type="text" name="pin"><br>
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";


const char  html_home[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP32 ATM</title>
  </head><body>
  <h2>Welcome to ATM with ESP32 (BT20ECE036) </h2> 
   <form action="/add_user">
    <input type="submit" value="Add a New USER">
  </form><br>
   <form action="/login_home">
    <input type="submit" value="Login">
  </form>
</body></html>)rawliteral";


const char  html_select_wo[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP32 ATM</title>
  </head><body>
  <h2>What you want to do? </h2> 
   <form action='/make_trans'>
      <input type="submit" value="Make TRANSACTON">
  </form><br>
   <form action='/change_pass'>
      <input type="submit" value="Change PASSWORD">
  </form><br>
   <form action='/check_bal'>
      <input type="submit" value="Check BALANCE">
  </form><br>
    <form action='/logout'>
      <input type="submit" value="Logout">
  </form>
</body></html>)rawliteral";

const char  html_change_pass[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP32 ATM</title>
  </head><body>
  <h2>Change Password</h2> 
   <form action='/changed_pass'>
    <label>Set a new Password:</label>
     <input type="text" name="new_password">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";


const char  html_add_user[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP32 ATM</title>
  </head><body>
  <h2>Add a New User </h2> 
   <form action='/set'>
    <label>Set a Username:</label>
     <input type="text" name="set_user">
    <label>Set a Password:</label>
     <input type="text" name="set_pass"><br>
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

const char  html_trans[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Enter the Details </title>
  </head><body>
  <h2>ATM with ESP32 (BT20ECE036) </h2> 
   <form action="/trans">
    <label>Enter the amount:</label>
     <input type="number" name="amount"><br>
    <label>What do you want to do? </label>
    <select name="trans" >
  <option value="credit">Credit</option>
  <option value="debit">Debit</option>
  </select><br>
  <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}




void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connecting...");
    return;
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html",  html_home);
   
  });


   server.on("/add_user", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html",  html_add_user);
  });


  server.on("/select_wo", HTTP_GET, [](AsyncWebServerRequest *request){
     
    otp_inp = request->getParam("otp")->value();

    String user_inp = inp_username+" "+inp_pin;
    if(userpass ==user_inp && otp_inp==otp){
   
      request->send(200, "text/html",  html_select_wo);
    }
    else if(un=="no user"){
       request->send(200, "text/html", "<h3>No user found</h3><br><a href='/login_home'>Try Again</a><br><a href='/'>Return to HOME</a><br>");
    }
    else{
       request->send(200, "text/html", "<h3>you have entered wrong password or username</h3><br><a href='/login_home'>Try Again</a><br><a href='/'>Return to HOME</a><br>");
    
    }
   
   
  });


server.on("/otp", HTTP_GET, [](AsyncWebServerRequest *request){/////////////////////////////////////
   if (request->hasParam(username)) {
      inp_username = request->getParam(username)->value();
      inp_pin = request->getParam(pin)->value();
    }
    else {
      inp_username= "none";
      inp_pin = "none";
    }
    flag="user_pass";
    String s = "<h3>Enter the OTP: "+otp+"  below </h3><br> <form action='/select_wo'><input type='text' name='otp'><input type='submit' value='Submit'></form>";
    request->send(200, "text/html",s);
  });

   server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request){
     if(request->hasParam(set_user)){
    set_username = request->getParam(set_user)->value();
    set_password = request->getParam(set_pass)->value();
    
    flag="set_newuser";
    } 
    request->send_P(200, "text/html", "<h3>New USER has been added successfully </h3><br><a href='/'>Return to MAIN</a>");
  });


   server.on("/login_home", HTTP_GET, [](AsyncWebServerRequest *request){
    

    request->send_P(200, "text/html",  html_login);
  });

   server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request){
    
    request->send_P(200, "text/html", "<h3>Logut successfuly </h3><br><a href='/'>Return to Main</a>");
  });
  
  
   server.on("/make_trans", HTTP_GET, [](AsyncWebServerRequest *request){
     a=0;     
    request->send_P(200, "text/html",  html_trans);
  });

   server.on("/check_bal", HTTP_GET, [](AsyncWebServerRequest *request){
     flag="get_balance";
     String s2 = "<h3>Your current bank balance is :"+prev_balance+" </h3><br><a href="+dir+">BACK TO HOME</a><br><a href='/logout'>LOGOUT</a><br>";
    request->send(200, "text/html", s2);
  });

    server.on("/changed_pass", HTTP_GET, [](AsyncWebServerRequest *request){
       if(request->hasParam("new_password")){
       
    new_password = request->getParam("new_password")->value();
     flag="new_pass";
    String s1 ="<h3>Password has been updated successfully</h3>.<br><a href="+dir+">BACK to HOME </a><br><a href='/logout'>LOGOUT</a><br>";
    request->send(200, "text/html",s1  );

       }
  });

   server.on("/change_pass", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html",  html_change_pass);
  });

   server.on("/trans", HTTP_GET, [](AsyncWebServerRequest *request){
    amount = request->getParam("amount")->value();
    trans_act = request->getParam("trans")->value();
    if (trans_act=="debit"){
      debited = amount.toInt();
      credited = 0;
    }
    else{
      debited = 0;
      credited = amount.toInt();
    }
    if(a==0){
    flag="cal_trans send";
    a=1;
    }
    
    request->send(200, "text/html", html_summary );
  });
   server.on("/trans_summary", HTTP_GET, [](AsyncWebServerRequest *request){
  
   request->send(200, "text/html","<h2>Thanks for Using the ATM with ESP32(BT20ECE036)</h2><br><a href='/'>Return to the MAIN </a>" );
  });

  server.onNotFound(notFound);
  server.begin();
}



void loop() {
 
    if(flag=="user_pass"){
    get("get_un");
   get(flag);
   flag="";
   }
   if(flag=="get_balance"){
   get("get_bal");
   flag="";
   }
   if(flag=="cal_trans send"){
   cal_details();

   flag="";
   }
  
   if(flag=="set_newuser"){
   Serial.println("setting usser");
   get("set_user");
   flag="";
   }

   if(flag=="get_un"){
   Serial.println("getting un");
   get("get_un");
   flag="";
   }
  
   if(flag=="new_pass"){
   Serial.println("saving password");
   get(flag);
   flag="";
   }
  
   html_summary=  "<!DOCTYPE HTML><html><head>\
  <title>ATM with ESP32 (BT20ECE036)</title>\
  </head><body>\
  <h2>Transaction Summary</h2>\
  <h4>"+amount+" has been successfully "+trans_act+"ed from your account</h4>\
  <p>\
   <b>Previous Balance </b>:" +prev_balance+"<br>\
   <b>Debited </b>:"+String(debited)+"<br>\
   <b>Credited</b>:"+String(credited)+"<br>\
   <b>Current Balance </b>:"+String(current_balance)+"<br>\
  </p>\
  <form action='/trans_summary'>\
  <input type='submit' value='Logout'>\
  </form>\ 
  </body></html>";

}
