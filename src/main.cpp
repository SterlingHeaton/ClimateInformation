#include <LiquidCrystal.h>
#include <Wire.h>
#include <MyClock.h>
#include <MyThermometer.h>

LiquidCrystal lcd( 32, 33, 36, 37, 38, 39 );
MyClock       myClock;
MyThermometer myThermometer( 0 );

unsigned long lastUpdate  = 0;
float         temperature = 0;

void setup()
{
  pinMode( LED_BUILTIN, OUTPUT );
  digitalWrite( LED_BUILTIN, LOW );

  Serial.begin( 9600 );

  Wire.begin();

  lcd.begin( 16, 2 );
  lcd.println( "SETUP" );
  lcd.print( "LOADING TIME" );
  lcd.setCursor( 0, 1 );
  lcd.print( "LOADING TEMP" );

  Serial.println( "SETUP" );

  analogWrite( 11, 70 );
}

void loop()
{
  auto startTime = millis();
  // If we have a serial input, update real time clock to the timestamp provided.
  if( Serial.available() )
  {
    Serial.println( "LOOP->AVILABLE" );

    String input = Serial.readString();
    input.trim();

    myClock.UpdateClock( input );
  }
  if( millis() - lastUpdate >= 1000 )
  {
    temperature = myThermometer.GetTemperature();
    lastUpdate  = millis();
  }

  String dateString = myClock.GetDate();
  String timeString = myClock.GetTime( true );

  lcd.clear();
  lcd.print( dateString );
  lcd.setCursor( 0, 1 );
  lcd.print( timeString + "    " + static_cast<int16_t>( temperature ) + "F" );

  Serial.println( "Loop: " + String { millis() - startTime } + "ms" );

  delay( 100 );
}
