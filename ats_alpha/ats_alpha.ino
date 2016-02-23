// variables 
int supplyMode = 0;				//1 for wapda , 2 for generator.
int wapdaVoltage ;
bool generatorStart = true; 		        // only true if generator can be started 
/* GeneratorRelay ; 				// controlling variable , value range "High"/"Low" . Set relay
WapdaRelay ; 					// controlling variable , value range "High"/"Low" . Set relay
GenRetry ; 				        // controlling variable , will be used used to read states of generator start Allow Button. (GenReset Button Originally) */

int GeneratorRelay = 10;                        // Generator Ignition pin location
int WapdaRelay = 9;                             // Wapda  pin location
int PLED = 13;                                  // Power LED pin location								

void Setup()
{
  pinMode(GeneratorRelay, OUTPUT);
  pinMode(WapdaRelay, OUTPUT); 
  pinMode(PLED, OUTPUT) ;
  
  }
  
 void loop()
{
  //WapdaRelay = checkWapda () ;
  //VOLTAGE PROTECTION SYSTEM CODE STARTS HERE
  if (wapdaVoltage >150 && wapdaVoltage <250);
  {
    if(supplyMode == 2) //if the supply mode is two , i.e if the generator is running
    {
    digitalWrite (GeneratorRelay , HIGH);
    }
  }
}
  
