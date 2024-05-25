#pragma once

class MyThermometer
{
public:
  auto GetTemperature() -> float;

private:
  const int PIN = 0;
};
