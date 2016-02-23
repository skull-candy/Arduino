#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance

void setup()
{  
  Serial.begin(9600);
  
  emon1.voltage(2, 225.26, 1.7);  // Voltage: input pin, calibration, phase_shift
//  emon1.current(1, 000.0);       // Current: input pin, calibration.
}

void loop()
{
//  emon1.calcVI(20,2000);         // Calculate all. No.of wavelengths, time-out
//  emon1.serialprint();           // Print out all variables
 Serial.println(emon1.voltage); 
}
