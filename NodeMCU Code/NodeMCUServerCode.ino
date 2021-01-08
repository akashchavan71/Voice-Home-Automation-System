#include <ESP8266WiFi.h>
#include <String.h> 
#include <Adafruit_NeoPixel.h>
#include <SPI.h>

#define PIN D2
#define NUMPIXELS 12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
uint32_t Red = pixels.Color(255, 0, 0);
uint32_t Green = pixels.Color(0, 255, 0);
uint32_t Blue = pixels.Color(0, 0, 255);
uint32_t White = pixels.Color(255, 255, 255);
uint32_t Black = pixels.Color(0, 0, 0);

int port = 8888;  //Port number
WiFiServer server(port);

bool Is_First_Cycle = true;

//Server connect to WiFi Network

const char *ssid = "OnePlus";  //Enter your wifi SSID
const char *password = "wireless1";  //Enter your wifi Password

//const char *ssid = "Anonymous2.4";  //Enter your wifi SSID
//const char *password = "leonardo";  //Enter your wifi Password*/


int count=0;

void SetColor(uint32_t color)
{
  for(int i=0; i<pixels.numPixels(); i++) 
  { // For each pixel in strip...
    pixels.setPixelColor(i, color);         
    pixels.show();                          
    //delay(10);                           //  Pause for a moment
  }
}

void TurnOn()
{
  SetColor(White);
}

void TurnOff()
{
  SetColor(Black);
}

void setup() 
{

    pixels.begin();
    pixels.show(); 
    pixels.setBrightness(10);
    
    Serial.begin(74880);
    Serial.println();

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password); //Connect to wifi
    
    // Wait for connection  
    Serial.println("Connecting to Wifi");
    while (WiFi.status() != WL_CONNECTED) {   
      delay(500);
      Serial.print(".");
      delay(500);
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
  
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  
    server.begin();
    Serial.print("Open Telnet and connect to IP:");
    Serial.print(WiFi.localIP());
    Serial.print(" on port ");
    Serial.println(port);

}

void loop() 
{
  String command;
  size_t flag;
  WiFiClient client = server.available();
  if(Is_First_Cycle)
  {
    TurnOff();
    Is_First_Cycle = false;
  }  
            
  if (client) {
    if(client.connected())
    {
      Serial.println("Client Connected");
    }
    
    while(client.connected()){      
      //delay(100);
      client.setTimeout(10);
      while(client.available()>0){
        // read data from the connected client
        command = client.readString();
        client.setNoDelay(1);
        Serial.println("check1"); 
        if (command.substring(0) == "on") 
        {
          TurnOn();
          Serial.println("It's a On signal");
        }
        else if (command.substring(0) == "off") 
        {
          TurnOff();
          Serial.println("It's a Off signal");
        }
        else if (command.substring(0) == "red") 
        {
          SetColor(Red);
          Serial.println("Turning lights Red");
        }
        else if (command.substring(0) == "blue") 
        {
          SetColor(Blue);
          Serial.println("Turning lights Blue");
        }
        else if (command.substring(0) == "green") 
        {
          SetColor(Green);
          Serial.println("Turning lights Green");
        }
        Serial.println(command); 
      }
      //Send Data to connected client
      while(Serial.available()>0)
      {
        client.write(Serial.read());
      }
    }
    client.stop();
    Serial.println("Client disconnected");    
  }
}
