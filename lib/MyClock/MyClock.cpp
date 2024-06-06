#include <MyClock.h>

auto MyClock::UpdateClock( String input ) -> void
{
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
  ds3231Clock.setYear( temp1 * 10 + temp2 );

  // now month
  temp1 = ( byte )inString[ 2 ] - 48;
  temp2 = ( byte )inString[ 3 ] - 48;
  ds3231Clock.setMonth( temp1 * 10 + temp2 );

  // now date
  temp1 = ( byte )inString[ 4 ] - 48;
  temp2 = ( byte )inString[ 5 ] - 48;
  ds3231Clock.setDate( temp1 * 10 + temp2 );

  // now Day of Week
  ds3231Clock.setDoW( ( byte )inString[ 6 ] - 48 );

  // now hour
  temp1 = ( byte )inString[ 7 ] - 48;
  temp2 = ( byte )inString[ 8 ] - 48;
  ds3231Clock.setHour( temp1 * 10 + temp2 );

  // now minute
  temp1 = ( byte )inString[ 9 ] - 48;
  temp2 = ( byte )inString[ 10 ] - 48;
  ds3231Clock.setMinute( temp1 * 10 + temp2 );

  // now second
  temp1 = ( byte )inString[ 11 ] - 48;
  temp2 = ( byte )inString[ 12 ] - 48;
  ds3231Clock.setSecond( temp1 * 10 + temp2 );

  ds3231Clock.setClockMode( true ); // set to 12h
}

auto MyClock::GetDate() -> String
{
  String dateFormat = "%M %D, 20%Y";

  dateFormat.replace( "%Y", String { ds3231Clock.getYear() } );
  dateFormat.replace( "%M", GetShortMonth( ds3231Clock.getMonth( century ) - 1 ) );
  dateFormat.replace( "%D", String { ds3231Clock.getDate() < 10 ? "0" + String { ds3231Clock.getDate() } : ds3231Clock.getDate() } );

  return dateFormat;
}

auto MyClock::GetTime( bool meridiemShortHand ) -> String
{
  String timeFormat = "%H:%M:%S%P";

  timeFormat.replace( "%H", String { ds3231Clock.getHour( h12Flag, pmFlag ) < 10 ? "0" + String { ds3231Clock.getHour( h12Flag, pmFlag ) } : ds3231Clock.getHour( h12Flag, pmFlag ) } );
  timeFormat.replace( "%M", String { ds3231Clock.getMinute() < 10 ? "0" + String { ds3231Clock.getMinute() } : ds3231Clock.getMinute() } );
  timeFormat.replace( "%S", String { ds3231Clock.getSecond() < 10 ? "0" + String { ds3231Clock.getSecond() } : ds3231Clock.getSecond() } );

  if( meridiemShortHand )
  {
    timeFormat.replace( "%P", pmFlag ? "p" : "a" );
  }
  else
  {
    timeFormat.replace( "%P", pmFlag ? "PM" : "AM" );
  }

  return timeFormat;
}

auto MyClock::GetShortMonth( uint8_t month ) -> String
{
  switch( month )
  {
    case 0: return "Jan";
    case 1: return "Feb";
    case 2: return "Mar";
    case 3: return "Apr";
    case 4: return "May";
    case 5: return "Jun";
    case 6: return "Jul";
    case 7: return "Aug";
    case 8: return "Sep";
    case 9: return "Oct";
    case 10: return "Nov";
    case 11: return "Dec";
    default: return "ERR";
  }
}
