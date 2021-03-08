/*
   WARNING:
   If you are not receiving the Json Messages you probably have to increase the
   MQTT_MAX_PACKET_SIZE in the PubSubClient.h file to at least 1024
   (Located on Windows at C:\Users\*YOUR_USERNAME*\Documents\Arduino\libraries\pubsubclient-master\src\PubSubClient.h)
*/

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Adafruit_PCD8544.h> // https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library
#include <Adafruit_GFX.h>     // https://github.com/adafruit/Adafruit-GFX-Library
#include <PubSubClient.h>     // https://github.com/knolleary/pubsubclient
#include <OneButton.h>        // https://github.com/mathertel/OneButton
#include <ArduinoJson.h>      // https://github.com/bblanchon/ArduinoJson


  const char *ssid = "----WIFI_SSID_HERE----";
  const char *password = "----WIFI_PASSWORD_HERE----";

  //MQTT Server
  const char *mqtt_server = "----MQTT_SERVER_IP_HERE----";
  const char *mqtt_user = "----MQTT_USERNAME_HERE----";
  const char *mqtt_pass = "----MQTT_PASSWORD_HERE----";


int contrast = 65;

unsigned long display_light_max = 10000;  // Values in ms
unsigned long display_2_max = 10000;
unsigned long update_time_max = 600000;  // 600000 ms = 10 min

const int buttonpin = D6;
const int backlightpin = D4;
Adafruit_PCD8544 display = Adafruit_PCD8544(D5, D7, D3, D2, D1);

String update = "-";
String temp_2m = "TT.T";
String humidity = "FF.F";
String pressure = "DDDD.D";

String temp_0m = "T0.T";
String dewpoint = "tp.t";
String luminosity = "HHHHH.H";
String wind = "WW.W";
String windr = "WRWR";

bool startup = true;
unsigned long update_time = 0;

bool display_light = false;
unsigned long display_light_time = 0;

bool display_2 = false;
unsigned long display_2_time = 0;

int error = 0;

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonBuffer<1024> jsonBuffer;

OneButton button = OneButton(
                     buttonpin, // Input pin for the button
                     true,      // Button is active LOW
                     true       // Enable internal pull-up resistor
                   );

void setup()
{
  pinMode(backlightpin, OUTPUT);
  digitalWrite(backlightpin, LOW); //LOW = Light on!

  button.attachClick(button_click);
  button.attachLongPressStart(button_long);
  button.attachDoubleClick(button_doubleclick);

  display.begin();
  display.setContrast(contrast);

  display.setTextSize(2);
  display.clearDisplay();
  display.setTextColor(BLACK);
  display.setCursor(5, 0);
  display.print("Wetterstation");
  display.setTextSize(1);
  display.setCursor(5, 35);
  display.print("Starting ...");
  display.display();

  delay(1000);
  digitalWrite(backlightpin, HIGH);
  delay(1000);
  digitalWrite(backlightpin, LOW);
  delay(1000);
  digitalWrite(backlightpin, HIGH);
  delay(1000);
  digitalWrite(backlightpin, LOW);

  display.clearDisplay();
  display.setCursor(5, 0);
  display.print("Wetterstation");
  display.setCursor(1, 10);
  display.print("Connecting Wifi");
  display.setCursor(1, 20);
  display.display();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    display.print(".");
    display.display();
    delay(500);
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();

  display.clearDisplay();
  display.setCursor(5, 0);
  display.print("Wetterstation");
  display.setCursor(25, 10);
  display.print("Wifi");
  display.setCursor(15, 30);
  display.print("connected");
  display.display();
  delay(1000);

  digitalWrite(backlightpin, HIGH);
}

void button_doubleclick()
{
  display.clearDisplay();
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(5, 25);
  display.print("Updating ...");
  display.display();

  delay(100);
  update_time = millis() + update_time_max + 1;
}

void button_click()
{
  digitalWrite(backlightpin, LOW);
  display_light_time = millis();
  display_light = true;
}

void button_long()
{
  digitalWrite(backlightpin, LOW);
  display_light_time = millis();
  display_light = true;

  display_page2();
  display_2_time = millis();
  display_2 = true;
}

void loop()
{
  button.tick();

  if (!client.connected())
    reconnect();

  client.loop();

  if (millis() - update_time > update_time_max || startup)
  {
    startup = false;
    error = 3;

    display.setCursor(0, 10);
    display.print("U");
    display.display();
    delay(300);

    client.publish("wetter_display/get", "1");
    update_time = millis();
  }

  if (error == 2)
  {
    display_error("Message decoding Error");
    error = 0;
  }
  else if (error == 1)
  {
    updatedisplay();
    error = 0;
  }
  else if (error == 3 && millis() - update_time > 5000)
  {
    display_error("No Response from MQTT server");
    error = 0;
  }


  if (display_light && (millis() - display_light_time > display_light_max))
  {
    digitalWrite(backlightpin, HIGH);
    display_light = false;
  }

  if (display_2 && (millis() - display_2_time > display_2_max))
  {
    updatedisplay();
    display_2 = false;
  }

  delay(100);
}
