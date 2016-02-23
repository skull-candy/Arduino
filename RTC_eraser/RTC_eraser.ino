
#include <Wire.h>
#define DS1307_I2C_ADDRESS 0x68

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  Serial.println("Erasing DS1307 RAM");
  Wire.beginTransmission(DS1307_I2C_ADDRESS);  
  Wire.write(0x08); // set register pointer to 0x08 register
  for (byte n = 0; n <56; n++){    
    Wire.write(0x00);     
    //delay(10);
  }
  Wire.endTransmission();  
  Serial.println("done erasing");
  
  delay(1000);
  
  Serial.println("Reading DS1307 RAM");
  Wire.beginTransmission(DS1307_I2C_ADDRESS);  
  Wire.write(0x08); // RAM location 0x08
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 56); // request 56 bytes
  
  for (byte n = 0x08; n <=0x3F; n++){    
    byte DS1307_RAM_byte = Wire.read();
    Serial.print(n, HEX);
    Serial.print(" = ");
    Serial.println(DS1307_RAM_byte);
   // delay(10);
  }  
  Serial.println("done reading");
}

void loop() {
  
}
