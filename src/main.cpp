#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <DS3231.h>

LiquidCrystal lcd( 2, 3, 4, 5, 6, 7 );

const int TEMP_PIN = 0;

DS3231 myClock;

bool century = false;
bool h12Flag = true;
bool pmFlag = true;

String lcdLine1;
String lcdLine2;

auto UpdateTimeFromSerial() -> void;
auto GetTemperature() -> float;

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
  // Serial.println( "LOOP" );

  // If we have a serial input, update real time clock to the timestamp provided.
  if( Serial.available() )
  {
    Serial.println( "LOOP->AVILABLE" );
    UpdateTimeFromSerial();
  }

  float temperature = GetTemperature();

  String dateTempLine = "%Y-%M-%D  %TF";
  String timeLine     = "%H:%M:%S      %P";

  dateTempLine.replace( "%Y", String { myClock.getYear() } );
  dateTempLine.replace( "%M", String { myClock.getMonth( century ) < 10 ? "0" + String { myClock.getMonth( century ) } : myClock.getMonth( century ) } );
  dateTempLine.replace( "%D", String { myClock.getDate() < 10 ? "0" + String { myClock.getDate() } : myClock.getDate() } );
  dateTempLine.replace( "%T", String { temperature } );

  timeLine.replace( "%H", String { myClock.getHour( h12Flag, pmFlag ) < 10 ? "0" + String { myClock.getHour( h12Flag, pmFlag ) } : myClock.getHour( h12Flag, pmFlag ) } );
  timeLine.replace( "%M", String { myClock.getMinute() < 10 ? "0" + String { myClock.getMinute() } : myClock.getMinute() } );
  timeLine.replace( "%S", String { myClock.getSecond() < 10 ? "0" + String { myClock.getSecond() } : myClock.getSecond() } );
  timeLine.replace( "%P", pmFlag ? "PM" : "AM" );

  if( lcdLine1 != dateTempLine || lcdLine2 != timeLine )
  {
    lcdLine1 = dateTempLine;
    lcdLine2 = timeLine;

    lcd.clear();
    lcd.print( lcdLine1 );
    lcd.setCursor( 0, 1 );
    lcd.print( lcdLine2 );
  }

  delay( 1000 );
}

auto UpdateTimeFromSerial() -> void
{
  Serial.println( "GETDATESTUFF" );
  // Call this if you notice something coming in on
  // the serial port. The stuff coming in should be in
  // the order YYMMDDwHHMMSS, with an 'x' at the end.

  String input = Serial.readString();
  input.trim();

  byte temp1, temp2;
  char inString[ 20 ];

  for( unsigned int i = 0; i < input.length(); i++ )
  {
    inString[ i ] = input.charAt( i );
  }

  // now year
  temp1 = ( byte )inString[ 0 ] - 48;
  temp2 = ( byte )inString[ 1 ] - 48;
  myClock.setYear( temp1 * 10 + temp2 );

  // now month
  temp1 = ( byte )inString[ 2 ] - 48;
  temp2 = ( byte )inString[ 3 ] - 48;
  myClock.setMonth( temp1 * 10 + temp2 );

  // now date
  temp1 = ( byte )inString[ 4 ] - 48;
  temp2 = ( byte )inString[ 5 ] - 48;
  myClock.setDate( temp1 * 10 + temp2 );

  // now Day of Week
  myClock.setDoW( ( byte )inString[ 6 ] - 48 );

  // now hour
  temp1 = ( byte )inString[ 7 ] - 48;
  temp2 = ( byte )inString[ 8 ] - 48;
  myClock.setHour( temp1 * 10 + temp2 );

  // now minute
  temp1 = ( byte )inString[ 9 ] - 48;
  temp2 = ( byte )inString[ 10 ] - 48;
  myClock.setMinute( temp1 * 10 + temp2 );

  // now second
  temp1 = ( byte )inString[ 11 ] - 48;
  temp2 = ( byte )inString[ 12 ] - 48;
  myClock.setSecond( temp1 * 10 + temp2 );

  myClock.setClockMode( true ); // set to 12h
}

auto GetTemperature() -> float
{
  int tempReading = analogRead(TEMP_PIN);

  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit

  return tempF;
}
