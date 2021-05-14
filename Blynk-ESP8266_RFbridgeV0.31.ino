#define NAMEandVERSION "RF_Bridge_v0.31"
/*
 * Work in progress
  Simple code for using the Sonoff Bridge with Blynk

V1  = Blynk terminal
V2  = Blynk Reset Alarm button
V6  = Force OFF Blynk Buton for the PUMP Blynk device
V10 = Blynk Button for monitoring the Flood Sensor 
V11 = Blynk Button for monitoring the Flood Sensor 
V12 = Blynk Button for monitoring the Flood Sensor 
V50 = WidgetBridge pump

  
*/

//#define BLYNK_DEBUG
#define BLYNK_TIMEOUT_MS  500  // must be BEFORE BlynkSimpleEsp8266.h doesn't work !!!
#define BLYNK_HEARTBEAT   17   // must be BEFORE BlynkSimpleEsp8266.h works OK as 17s

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
String receivedString = "";
String finishedCode = "";
int i = 0;


int digit;

#include "config.h"

//#define SerialRF Serial
BlynkTimer timer;
#define BLYNK_PRINT terminal    
#define SerialRF Serial
WidgetBridge pump(V50);


bool on = 0;
bool online = 0;


// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V1);

//For SerialRF Readings

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  //Serial.begin(115200);
  //Serial.println();
  WiFi.hostname(NAMEandVERSION);
  WiFi.mode(WIFI_STA);
  Blynk.config(auth, serveraddr, port);  // I am using the local Server
  CheckConnection();// It needs to run first to initiate the connection.Same function works for checking the connection!
  timer.setInterval(20000L, CheckConnection); 
  timer.setInterval(6000L, floodMonitor);
  SerialRF.begin(19200);
}

void loop() {
  if(Blynk.connected()){
    Blynk.run();
  }
  timer.run();

  char serialIn; // Set and wipe variable
  while (SerialRF.available() > 0) {
    serialIn = SerialRF.read(); // Takes in an int
    print_hex(serialIn,8);
    //Serial.println();
  }

}

void print_hex(int v, int num_places)
{
    int mask=0, n, num_nibbles;

    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    v = v & mask; // truncate v to specified number of places

    num_nibbles = num_places / 4;
    if ((num_places % 4) != 0)
    {
        ++num_nibbles;
    }

    do
    {
        digit = ((v >> (num_nibbles-1) * 4)) & 0x0f;
        processReceived();
    } while(--num_nibbles);
}

void processReceived()
{
  receivedString = receivedString + String(digit, HEX);
  i++;
  if (i == 24){
    finishedCode = receivedString;
    receivedString = "";
    finishedCode.toUpperCase();
    terminal.println(finishedCode);
    terminal.flush();
    i=0;
    compareCodes2();
    receiveKeyValue();
  }
  if (i > 24){
    i = 1;
  }
  
}

void receiveKeyValue(){
  byte message[12] = {0XAA, 0XA0, 0X55};  // confirmation message to the RF MCU
  Serial.write(message, sizeof(message));
}

void compareCodes2()
{
//   terminal.println(String(finishedCode.substring(16,24)));
//   terminal.flush();
  if (finishedCode.substring(0,3 )!= "AAA" )
  {
    terminal.println("Code readings failed! Please Reset!");
    terminal.flush();
    finishedCode="";
    receivedString="";
    char serialIn[30];
    i=0;
  }
   
   if (finishedCode.substring(16,24 )== targetCode1 )
   {
    terminal.println("Button1!");
    terminal.flush();
   }
   else if (finishedCode.substring(16,24 )== targetCode2) {
    terminal.println("Button2!");
    terminal.flush();
   }
   else if (finishedCode.substring(16,24 )== targetCode3) {
    terminal.println("Button3!");
    terminal.flush();
   }
   else if (finishedCode.substring(16,24 )== targetCode4) {
    terminal.println("Button4!");
    terminal.flush();
   }
   else if (finishedCode.substring(16,24 )== targetCode5) {
    terminal.println("Flood 1!");
    terminal.flush();
    Blynk.notify("Flood 1!");
    Blynk.virtualWrite(V10, 1);
    flood1 = 1;
   }
   else if (finishedCode.substring(16,24 )== targetCode6) {
    terminal.println("Flood 2!");
    terminal.flush();
    Blynk.notify("Flood 2!");
    Blynk.virtualWrite(V11, 1);
    flood2 = 1;
   }
   else if (finishedCode.substring(16,24 )== targetCode7) {
    terminal.println("Flood 3!");
    terminal.flush();
    Blynk.notify("Flood 3!");
    Blynk.virtualWrite(V12, 1);
    flood3 = 1;
   }
}

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // ResetButton
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  if (pinValue == 1)
  {
    Blynk.virtualWrite(V10, 0);
    flood1 = 0;
    Blynk.virtualWrite(V11, 0);
    flood2 = 0;
    Blynk.virtualWrite(V12, 0);
    flood3 = 0;    
  }
}

void CheckConnection(){    // check every 11s if connected to Blynk server
  if(!Blynk.connected()){
    online = 0;
    yield();
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Not connected to Wifi! Connect...");
      //Blynk.connectWiFi(ssid, pass); // used with Blynk.connect() in place of Blynk.begin(auth, ssid, pass, server, port);
      //WiFi.config(arduino_ip, gateway_ip, subnet_mask);
      //Serial.println(ssid);
      //Serial.println(pass);
      WiFi.begin(ssid, pass);
      delay(400); //give it some time to connect
      if (WiFi.status() != WL_CONNECTED)
      {
        Serial.println("Cannot connect to WIFI!");
        online = 0;
      }
      else
      {
        Serial.println("Connected to wifi!");
      }
    }
    
    if ( WiFi.status() == WL_CONNECTED && !Blynk.connected() )
    {
      Serial.println("Not connected to Blynk Server! Connecting..."); 
      Blynk.connect();  // // It has 3 attempts of the defined BLYNK_TIMEOUT_MS to connect to the server, otherwise it goes to the enxt line 
      if(!Blynk.connected()){
        Serial.println("Connection failed!"); 
        online = 0;
      }
      else
      {
        online = 1;
      }
    }
  }
  else{
    Serial.println("Connected to Blynk server!"); 
    online = 1;    
  }
}




// You can send commands from Terminal to your hardware. Just use
// the same Virtual Pin as your Terminal Widget
BLYNK_WRITE(V1)
{
  // if you type "Marco" into Terminal Widget - it will respond: "Polo:"
  if (String("Marco") == param.asStr()) {
    terminal.println("You said: 'Marco'") ;
    terminal.println("I said: 'Polo'") ;
    terminal.flush();
  } else {
    // Send it back
    terminal.print("You said:");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
    terminal.flush();
  }
  // Ensure everything is sent
  terminal.flush();
}

BLYNK_CONNECTED() {
  pump.setAuthToken(pumpauth); // Place the AuthToken of the second hardware here
}

void floodMonitor() //
{
  // Send value to another device
  if (flood1 == 1 || flood2 == 1 || flood3 == 1) {
    //    pump.digitalWrite(9, HIGH);  // Digital Pin 9 on the second board will be set HIGH
    pump.virtualWrite(V6, 1); // 
    Serial.println("FLOOD!");
    terminal.println("FLOOD");
    terminal.flush();
    
    if (flood1 == 1)
    {
      Blynk.notify("Flood in Technical Room ! ");
    }
    
    if (flood2 == 1)
    {
      Blynk.notify("Flood Kitchen !");
    }

    if (flood3 == 1)
    {
      Blynk.notify("Flood Bathroom !");
    }
  } 
}
