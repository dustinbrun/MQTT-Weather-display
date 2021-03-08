void callback(String topic, byte* message, unsigned int length)
{
  String messageTemp;
  for (int i = 0; i < length; i++)
    messageTemp += (char)message[i];
  
  if (topic == "wetter_display/set")
  {
    JsonObject& json = jsonBuffer.parseObject(messageTemp);
    if (!json.success()) {
      error = 2;
      return;
    }

    temp_2m = json["Temp_2m"].as<String>();
    temp_0m = json["Temp_0m"].as<String>();
    pressure = json["Pressure"].as<String>();
    humidity = json["Humidity"].as<String>();
    dewpoint = json["Dewpoint"].as<String>();
    luminosity = json["Luminosity"].as<String>();
    wind = json["Wind_kmh"].as<String>();
    windr = json["Wind_r"].as<String>();
    error = 1;

    jsonBuffer.clear();

  }
}

void reconnect()
{
  while (!client.connected())
  {
    if (client.connect("WetterDisplay", mqtt_user, mqtt_pass))
      client.subscribe("wetter_display/set");
    else
    {
      display_error("MQTT Error");
      delay(5000);
    }
  }
}
