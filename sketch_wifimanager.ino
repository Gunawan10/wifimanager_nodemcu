/**
   Written and Verified by Gunawan Puspo Prajoko
   
   This code is open for fair use.
   If you are using it for commercial purpose, then it's mandatory to give credits
**/

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

//setting pada port 80
ESP8266WebServer server(80);
DynamicJsonBuffer jBuffer;

//define pin
#define LED D1

//json key response
String key_ssid, key_ip, key_type;
boolean key_access;

//variable
int wifiset = 0; //if value 1, ConnectWifiFromRequest()
String req_ssid, req_pass;

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(9600);
  EEPROM.begin(512);
  delay(10);
  Serial.println();

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  
  //turn off hotspot
  WiFi.softAPdisconnect(true);
  delay(100);
  
  //Connect wifi stored in EEPROM
  boolean connectEEPROM = ConnectWifiFromEEPROM();
  if(connectEEPROM == false){
    //turning the hotspot on if can't connect wifi
    Serial.println("Turning the Hotspot on");
    connectLocal();
  }
  
}

void loop() {
  //if there is a request to connect to wifi from /setwifi
  if(wifiset == 1){
    //stop the server
    server.close();
    delay(100);
    
    ConnectWifiFromRequest(req_ssid, req_pass);
  }else{
    //read signal code LED
    if (WiFi.status() != WL_CONNECTED){
      mustConnectWifi();
    }else{
      hasConnectedWifi();
    } 

    server.handleClient();
  }
}

/*signal code LED*/
void mustConnectWifi(){
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
}

void connectingWifi(){
  digitalWrite(LED, HIGH);
  delay(250);
  digitalWrite(LED, LOW);
  delay(250);
}

void hasConnectedWifi(){
  digitalWrite(LED, HIGH);
  Serial.print("Check Internet Access : ");
  boolean ret = Ping.ping("www.google.com", 1);
  if(ret == true){
    Serial.println("Internet Access");
    //for json response if the connection has internet
    key_access = true; 
    
    //place your code like blink the LED or something
    
  }else{
    Serial.println("No Internet Access");   
    //for json response if the connection has internet
    key_access = false;
  }
}
/*end signal code*/

/*connection*/
boolean ConnectWifiFromEEPROM(){
  Serial.println("Read SSID and Password stored from EEPROM");

  //read ssid
  String essid;
  for (int i=0; i<32; ++i){
    essid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID : ");
  Serial.println(essid);

  //read pass
  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("Pass : ");
  Serial.println(epass);

  //try to connect
  if(essid != ""){
    boolean tryConnect = connectWifi(essid, epass);
    if (tryConnect == false){
      return false;
    }
  }else{
    return false;
  }

  return true;
}

boolean connectWifi(String ssid, String pass){
  //before connect to new connection, disconnect from current connection
  WiFi.disconnect();
  delay(100);
    
  Serial.println();
  Serial.println("Connecting to Wifi : ");
  Serial.print("SSID : ");
  Serial.println(ssid);
  Serial.print("Pass : ");
  Serial.println(pass);
  
  WiFi.begin(ssid.c_str(), pass.c_str());
  //set status connectingWifi
  int time = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if(time < 12 && WiFi.status() != WL_CONNECTED){
      connectingWifi();
      Serial.print(".");
    }else{
      break;
    }
  
  time = time+1;
  
  }
  
  if(WiFi.status() != WL_CONNECTED){
    Serial.println();
    Serial.println();
    Serial.println("Can't Connect to this network");
    
    return false;
      
  }else{
    //turn off hotspot
    WiFi.softAPdisconnect(true);
    delay(100);
    Serial.println();
    Serial.println("Connected");
    
    //print ip address
    Serial.println();
    Serial.print("Wifi IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Wifi MAC address: ");
    Serial.println(WiFi.macAddress());
    
    //json response
    key_ssid = ssid;
    key_ip = IpAddress2String(WiFi.localIP());
    key_type = "wifi";
    
    //wifiset back to normal
    wifiset = 0;
      
    //start server
    startServer();
      
  }
    
  return true;
}

void connectLocal(){
  String essid = "MyNodeIOT";
  String epass = "tes123456";

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  WiFi.softAP(essid.c_str(), epass.c_str());

  //print ip address
  Serial.println();
  Serial.print("Local IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("Local MAC address: ");
  Serial.println(WiFi.softAPmacAddress());

  //json response
  key_ssid = essid;
  key_ip = IpAddress2String(WiFi.softAPIP());
  key_access = false;
  key_type = "local";

  //wifiset back to normal
  wifiset = 0;

  //start server
  startServer();
  
}

void ConnectWifiFromRequest(String ssid, String pass){
  boolean tryConnect = connectWifi(ssid, pass);
  if(tryConnect == true){
    //write
    writeWifiSettingToEEPROM(ssid, pass);
    
  }else if(tryConnect == false){
    //Connect wifi stored in EEPROM
    boolean connectEEPROM = ConnectWifiFromEEPROM();
    if(connectEEPROM == false){
      //turning the hotspot on if can't connect wifi
      Serial.println("Turning the Hotspot on");
      connectLocal();
    }
  }
}
/*end connection*/

/*controller endpoint*/
void handleRoot() {
  //give feedback to client
  String msg = "{\"message\":\"Oops.. Page Not Available\"}";
  responseJson(400, msg);
}

void statusWifi(){
  //json response
  JsonObject& data = jBuffer.createObject();
  data["ssid"] = key_ssid;
  data["status"] = "connected";
  data["ip_address"] = key_ip;
  data["internet_access"] = key_access;
  data["connection_type"] = key_type;
  
  //print json
  String content;
  data.prettyPrintTo(content);
  
  responseJson(200, content);
}

void setWifi() {
  // check if there is a data post named ssid and pass
  if (server.hasArg("ssid") && server.hasArg("pass")) {
    //get wifi
    String ssid = server.arg("ssid");
    String pass = server.arg("pass");
    req_ssid = ssid;
    req_pass = pass;

    //give feedback to client
    String msg = "{\"message\":\"request successfully added\"}";
    responseJson(200, msg);
    
    //notify the system that there is a request to connect wifi
    wifiset = 1;
    
  }else{
    //give feedback to client
    String msg = "{\"message\":\"missing or invalid parameters\"}";
    responseJson(400, msg);
  }
}

void handleNotFound() {
  //give feedback to client
  String msg = "{\"message\":\"Oops.. Page Not Found\"}";
  responseJson(400, msg);
}
/*end controller endpoint*/

/*Helper*/
void startServer(){
  //create endpoint server
  server.on("/", handleRoot);
  server.on("/status", HTTP_GET, statusWifi);
  server.on("/setwifi", HTTP_POST, setWifi);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("Server Listening..");
}

void writeWifiSettingToEEPROM(String qssid, String qpass){
  Serial.println("Clearing Current Wifi Setting from EEPROM");
  for (int i=0; i<96; ++i){
    EEPROM.write(i, 0);
  }

  Serial.println("Writing SSID to EEPROM");
  for (int i=0; i<qssid.length(); ++i){
    EEPROM.write(i, qssid[i]);
    Serial.print("Wrote: ");
    Serial.println(qssid[i]);
  }
  Serial.println();

  Serial.println("Writing Password to EEPROM");
  for (int i=0; i<qpass.length(); ++i){
    EEPROM.write(32 + i, qpass[i]);
    Serial.print("Wrote: ");
    Serial.println(qpass[i]);
  }
  Serial.println();
  EEPROM.commit();
  
  ESP.reset();
}

void responseJson(int statusCode, String content){
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(statusCode, "application/json", content);
  Serial.println();
}

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ;
}
/*end helper*/
