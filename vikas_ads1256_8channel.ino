// Arudino Sample Code to use ADS1256 library
// Switching channel for ADS1256. 
// This sample code writes to MUX register and reads ADC values back.
// First for loop reads all channels in single ended mode.
// Second for loop reads 4 differential channels
// The purpose of this code is to show how to use switchChannel function.
// Written by Adien Akhmad, August 2015
// Modfified  Jan 2019 by Axel Sepulveda for ATMEGA328

#include <ADS1256.h>
#include <SPI.h>

// Construct and init ADS1256 object
float vRef = 2.5*100000;
//float _conversionFactor = -1.0;
//float _pga = 1;
ADS1256 adc(7.68, vRef, true ); // clockSpeed in Mhz,  VREF in volt, if use RESET PIN 

//double mapped = 0, to_map = 0;

float sensor1, sensor2, sensor3, sensor4;

void setup()
{
  Serial.begin(9600);

  adc.begin(ADS1256_DRATE_500SPS,ADS1256_GAIN_1,false); 
  
  // Sending SDATAC to stop reading contionus data, so we can send other command
  //adc.sendCommand(ADS1256_CMD_SDATAC);    //not sending SDATAC data in multiplexer mode
 // Serial.println("SDATAC command sent");
  adc.setChannel(0,1);
  
/*  Single Ended Mode
    ADS1256 support 8 single ended channel
    use setChannel(p) for this purpose, where p is the number of positive input channel between 0 and 7 (AIN0 to AIN7).
    AINCOM are automatically set as the negative input channel.
*/
/*
  Serial.println("Single ended mode.");

  for (int i = 0; i < 8; ++i)
  {
    adc.waitDRDY();
    adc.setChannel(i);
    Serial.print("Current Channel: ");

    Serial.print(i);
    Serial.print(" MUX: ");
    Serial.print(adc.readRegister(ADS1256_RADD_MUX),BIN); // Read the Input Multiplexer Control Register to see the current active channels
    Serial.print(" ADC Value: ");
    Serial.print(adc.readCurrentChannelRaw());
    Serial.println();
  }
*/
/*  Differential Mode
    ADS1256 support 4 differential channel
    use setChannel(p,n) for this purpose, where
      p is the number of positive input channel between 0 and 7 (AIN0 to AIN7),
      n is the number of negative input channel between 0 and 7 (AIN0 to AIN7).
*/
/*
  Serial.println("Changing to differential mode.");

  for (int i = 0; i < 8; i+=2)
  {
    adc.waitDRDY();
    adc.setChannel(i,i+1);
    Serial.print("Current Channel: ");
    Serial.print(i);
    Serial.print(" and ");  
    Serial.print((i+1));
    Serial.print(" MUX: ");
    Serial.print(adc.readRegister(ADS1256_RADD_MUX),BIN); // Read the Input Multiplexer Control Register to see the current active channels    
    Serial.print(" ADC Value: ");      
    Serial.print(adc.readCurrentChannelRaw());
    Serial.println();

  }

  // Please note that AINCOM is defined as channel number 8
  // When you read the serial output,
  // b 0000 1xxx   means AIN0 - AINCOM
  // b 0001 1xxx means AIN1 - AINCOM
  // b 0010 1xxx means AIN2 - AINCOM
  // etc
*/
}

void loop()
{/*
  for (int i = 0; i < 8; ++i)
  {
    adc.waitDRDY();
    adc.setChannel(i);
    /*
    Serial.print(" Current Channel: ");

    Serial.print(i);
   
    Serial.print(" MUX: ");
    //adc.readRegister(ADS1256_RADD_MUX);
    Serial.print(adc.readRegister(ADS1256_RADD_MUX),BIN); // Read the Input Multiplexer Control Register to see the current active channels
    
    Serial.print(" ADC Value: ");

    */
    /*
    to_map = (adc.readCurrentChannel(),5);   //     ((adc.readCurrentChannelRaw() / 0x7FFFFF) * ((4*vRef) / (float)_pga)) *_conversionFactor
    mapped = (to_map,-2.20,2.30,0,5);
    Serial.print(to_map);
    Serial.print(" ");

    */
/*
    Serial.print(adc.readCurrentChannel(),10);
    Serial.print(" ");
    
  }
Serial.println();
*/


  adc.waitDRDY(); // wait for DRDY to go low before changing multiplexer register
  adc.setChannel(2,3);   // Set the MUX for differential between ch2 and 3 
  sensor1 = adc.readCurrentChannel(); // DOUT arriving here are from MUX AIN0 and AIN1

  adc.waitDRDY();
  adc.setChannel(4,5);
  sensor2 = adc.readCurrentChannel(); //// DOUT arriving here are from MUX AIN2 and AIN3

  adc.waitDRDY();
  adc.setChannel(6,7);
  sensor3 = adc.readCurrentChannel(); // DOUT arriving here are from MUX AIN4 and AIN5

  adc.waitDRDY();
  adc.setChannel(0,1); // switch back to MUX AIN0 and AIN1
  sensor4 = adc.readCurrentChannel(); // DOUT arriving here are from MUX AIN6 and AIN7

  sensor2 = map(sensor2, -4.9, 0, 2.5, 0);
  sensor3 = map(sensor3, -4.9, 0, 2.5, 0);
  sensor4 = map(sensor4, -4.9, 0, 2.5, 0);

  //print the result.
  Serial.print(sensor1/100000, 5);
  Serial.print(" \t");
  Serial.print(sensor2/100000, 5);
  Serial.print(" \t");
  Serial.print(sensor3/100000, 5);
  Serial.print(" \t");
  Serial.println(sensor4/100000, 5);

}
