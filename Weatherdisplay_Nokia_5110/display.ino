void updatedisplay()
{
  if (update == "-")  update = "--";
  else if (update == "--")  update = "---";
  else if (update == "---")  update = "----";
  else if (update == "----")  update = "-----";
  else if (update == "-----")  update = "-";

  display.clearDisplay();
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temp");
  display.setCursor(28, 0);
  display.setTextSize(2);
  display.print(temp_2m); //Temperature

  display.setTextSize(1);
  display.setCursor(12, 10);
  display.print("*C");

  display.setCursor(0, 18);
  display.print("Feu");
  display.setCursor(28, 18);
  display.print(humidity); //Humidity
  display.print(" %");

  display.setCursor(0, 28);
  display.print("Dr");
  display.setCursor(28, 28);
  display.print(pressure); //Pressure
  display.print("hPa");

  display.setCursor(0, 38);
  display.print("Wind");
  display.setCursor(28, 38);
  display.print(wind); //Windspeed
  display.print("km/h");

  display.display();
}

void display_page2()
{
  display.clearDisplay();
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Windr");
  display.setCursor(32, 0);
  display.print(windr); //Wind direction

  display.setCursor(0, 10);
  display.print("TempB");
  display.setCursor(32, 10);
  display.print(temp_0m); //Temperature 0m
  display.print(" *C");

  display.setCursor(0, 20);
  display.print("Taup");
  display.setCursor(28, 20);
  display.print(dewpoint); //Dewpoint

  display.setCursor(0, 30);
  display.print("Hell");
  display.setCursor(28, 30);
  display.print(luminosity); //Luminosity

  display.setCursor(0, 40);
  display.print(update); //Windrichtung

  display.display();
}

void display_error(String error)
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(5, 0);
  display.print("Error");
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print(error);
  display.display();
}
