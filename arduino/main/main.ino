
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFiUdp.h>
#include <WakeOnLan.h>
#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <cstddef>
#include <ArduinoJson.h>


const char* ssid = "POCO F3"; //Replace with your own SSID
const char* password = "1234567890"; //Replace with your 
AsyncWebServer server(80);
WiFiUDP UDP;
WakeOnLan WOL(UDP);

class Device
{
  private:
  char* name;
  char* mac;

  public:
  Device(char* name, char* mac)
  {
    this->name = name;
    this->mac= mac;
  }

  Device()
  {
    
  }



  void setname(char* name)
  {
    this->name = name;
  }

  void setmac(char* mac)
  {
    this->mac = mac;
  }

  char* getname()
  {
    return this->name;
  }
  char* getmac()
  {
    return this->mac;
  }
  
};


Device devicelist[15]; //* -> Pointer, weil Objekt nicht erstellt werden kann da keine Parameter übergeben werden, Array nicht initialisiert ist. Sonst wird Array mit bereits vorhandenen Objekten erstellt.


String ip;
String mac;
String name;


void setup() {
  
  Serial.begin(115200);
  if(!LittleFS.begin())
  {
    Serial.println("LittleFS Fehler");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP()); 


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/main.html", String(), false);
  });

  server.on("/wakeup", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    /*if(request->hasParam("ip"))
    {
        ip = request->getParam("ip")->value();
    }*/
    if(request->hasParam("mac"))
    {
      
      
        mac = request->getParam("mac")->value();
        //print(ip,mac);
        wakeupbyString(mac);
    }
    request->send(LittleFS, "/main.html", String(), false);
  });

  server.on("/newdevice", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    if(request->hasParam("name") && request->hasParam("mac"))
    {
      mac = request->getParam("mac")->value();
      name = request->getParam("name")->value();
      Serial.println("mac: ");
      Serial.print(mac);
      Serial.println("name: ");
      Serial.print(name);
      //fillArray(name,mac)
    }
    request->send(LittleFS, "/main.html", String(), false);
    
  });



  



  server.begin();
}

void print(String ip, String mac)
   {
      Serial.println("IP:" + ip + "| MAC: "+ mac);
   }

void wakeup(char* MACAdress)
{
  WOL.sendMagicPacket(MACAdress);
}

void wakeupbyString(String MACAdress)
{
    
         
  char *macchar;
  MACAdress.toCharArray(macchar, MACAdress.length() + 1);
  Serial.println("Ich lebe noch!");
  Serial.println(macchar);
  
  //wakeup(macchar);
}

void wakeupbydevice(Device device)
{
  WOL.sendMagicPacket(device.getmac());
}

char* convertStringtoChar(String Input)
{
  char *charstring;
  Input.toCharArray(charstring, Input.length() + 1); 
  //Serial.print("Convert methzode done: " );
  Serial.print(charstring);
  if ((charstring == NULL) ) 
    {
      Serial.print("c is empty\n");
    }
  return charstring;
}

void fillArray(char* name, char* mac)
{
  Device dev(name,mac);
  for(int i= 0; i<15; i++)
  {
    if(devicelist[i].getmac() == nullptr )
    {
      Serial.println("HUHUHU");
      devicelist[i] = dev; 
      return;
    }

  }
}

void ArrayToJson(Device* devicelist[])
{
  
}

void loop() 
{
   //Serial.print("test");
    //Device dev("test","test");

    //Serial.println(dev.getmac());
    //char *mac2 = "E4-B9-7A-DE-00-94";
    //wakeup(mac2);
    //Serial.print("Woken Up");
    //delay(3000);

    //fillArray("PC","A:5C:23489038495734985");
    //fillArray("PC2","A:5C:23254234532452345734985");
    //fillArray("PC3","A:5C:234823452345234585");
    //Serial.println("Devices");
    //Serial.println(devicelist[0].getmac());
    //Serial.println(devicelist[1].getmac());
    //Serial.println(devicelist[2].getmac());


    String test = "hallo";
    
    char* moin = convertStringtoChar(test);


    wakeupbyString(test);
    
    Serial.print(moin);
    
}
