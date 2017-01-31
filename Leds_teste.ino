/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "daf";
const char* password = "paulo.dinis";
IPAddress ip(192,168,1,41);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255, 255, 255, 0);
ESP8266WebServer server(80);

String webPage = "";
bool rele1 = HIGH;
bool rele2 = HIGH;
int gpio0_pin = 0;
int gpio2_pin = 2;

void setup(void){
  String socket1On = "";
  String socket1Off = "";
  String socket2On = "";
  String socket2Off = "";
  socket1On += random(1000);
  socket1Off += random(1000);
  socket2On += random(1000);
  socket2Off += random(1000);
  
  webPage += "<h1>ESP8266 Web Server</h1><p>Rele 1 <a href=\"" + socket1On + "\"><button>ON</button></a>&nbsp;<a href=\"" + socket1Off + "\"><button>OFF</button></a></p>";
  webPage += "<p>Rele 2 <a href=\"" + socket2On + "\"><button>ON</button></a>&nbsp;<a href=\"" + socket2Off + "\"><button>OFF</button></a></p>";

  socket1On = "/" + socket1On;
  socket1Off = "/" + socket1Off;
  socket2On = "/" + socket2On;
  socket2Off = "/" + socket2Off;
  
  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, HIGH);
  pinMode(gpio2_pin, OUTPUT);
  digitalWrite(gpio2_pin, HIGH);
  
  delay(1000);
  Serial.begin(115200);
  WiFi.config(ip,gateway,subnet);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  Serial.println(WiFi.macAddress());
  
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  server.on(socket1On.c_str(), [](){
    server.send(200, "text/html", webPage);
    if(rele2 == LOW) {digitalWrite(gpio2_pin,HIGH);rele2 = HIGH;}
    digitalWrite(gpio0_pin, LOW);
    rele1 = LOW;
    delay(1000);
  });
  server.on(socket1Off.c_str(), [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio0_pin, HIGH);
    rele1 = HIGH;
    delay(1000); 
  });
  server.on(socket2On.c_str(), [](){
    server.send(200, "text/html", webPage);
    if(rele1 == LOW) {digitalWrite(gpio0_pin,HIGH);rele1 = HIGH;}
    digitalWrite(gpio2_pin, LOW);
    rele2 = LOW;
    delay(1000);
  });
  server.on(socket2Off.c_str(), [](){
    server.send(200, "text/html", webPage);
    digitalWrite(gpio2_pin, HIGH);
    rele2 = HIGH;
    delay(1000); 
  });
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
    if(WiFi.status() != WL_CONNECTED){
      Serial.println("Desconectado");
      WiFi.config(ip,gateway,subnet);
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    }
    server.handleClient();
  
} 
