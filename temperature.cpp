/* This program converts a Fahrenheit temperature to a Celsius temperature and decides 
   if the temperature is above the boiling point or below the freezing point */
#include <iostream>

int main() {
  // Request and input a temperature.
  std::cout << "Please enter a Fahrenheit temperature:  ";
  float fahrenheit_temp;
  std::cin >> fahrenheit_temp;
  // Convert it to Celsius and output it.
  float celsius_temp = (fahrenheit_temp - 32) * 5.0 / 9.0;
  std::cout << "The equivalent Celsius temperature is " << celsius_temp << " degrees." << std::endl;
  // Output a message if the temperature is above boiling or below freezing.
  const int BoilingPointC = 100;
  const int FreezingPointC = 0;
  if (celsius_temp > BoilingPointC)
    std::cout << "That is above the boiling point of water.\n";
  else if (celsius_temp < FreezingPointC)
    std::cout << "That is below the freezing point of water.\n";
  return 0;  /* normal non-error exit code */
}
