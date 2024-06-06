#pragma once

class MyThermometer
{
public:
  MyThermometer(int pin)
    : PIN(pin) {}

  auto GetTemperature() -> float;

private:
  int PIN;
};
