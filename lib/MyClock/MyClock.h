#pragma once

#include <DS3231.h>

class MyClock
{
public:
  auto UpdateClock( String input ) -> void;
  auto GetDate() -> String;
  auto GetTime() -> String;

private:
  DS3231 ds3231Clock;

  bool century = false;
  bool h12Flag = false;
  bool pmFlag  = true;
};
