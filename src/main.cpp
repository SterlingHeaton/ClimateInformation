#include <LiquidCrystal.h>
#include <Wire.h>
#include <MyClock.h>
#include <MyThermometer.h>

LiquidCrystal lcd( 2, 3, 4, 5, 6, 7 );

MyClock myClock;
MyThermometer myThermometer;

void setup()
{
  Serial.begin( 9600 );

  Wire.begin();

  lcd.begin( 16, 2 );
  lcd.println( "SETUP" );
  lcd.print("LOADING TIME");
  lcd.setCursor(0, 1);
  lcd.print("LOADING TEMP");

  Serial.println( "SETUP" );
}

void loop()
{
  // If we have a serial input, update real time clock to the timestamp provided.
  if( Serial.available() )
  {
    Serial.println( "LOOP->AVILABLE" );

    String input = Serial.readString();
    input.trim();

    myClock.UpdateClock( input );
  }

  float temperature = myThermometer.GetTemperature();

  String dateString = myClock.GetDate();
  String timeString = myClock.GetTime();

  lcd.clear();
  lcd.print( dateString + "  " + temperature + "F");
  lcd.setCursor( 0, 1 );
  lcd.print( timeString );

  delay( 1000 );
}
