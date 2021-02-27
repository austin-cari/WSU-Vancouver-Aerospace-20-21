/* This code reads temperature and pressure from an MS5611 high sensitivity barometer
 *  and prints the results to the serial monitor, a SSD1306 OLED display, and an SD card
 *  
 *  The Circuit:
 *    To be used with Arduino Nano
 *    
 *    MS5611: Communicates via I2C
 *      VCC -> 5v
 *      GND -> GND
 *      SDA -> A4
 *      SCL -> A5
 *      
 *    SSD1306: Communicates via I2C
 *      VCC -> 5v
 *      GND -> GND
 *      SDA -> A4
 *      SCL -> A5
 *      
 *    HW-125: Communicates via SPI
 *      VCC   -> 5V
 *      GND   -> GND
 *      MISO  -> D12
 *      MOSI  -> D11
 *      SCK   -> D13
 *      CS    -> D10
 */

/* Declarations for SD reader */
#include <SPI.h>                // Serial Peripheral Interface library (for SD reader)
#include <SD.h>                 // Library for using HW-125 SD breakout
#define SD_PIN 10         // Change to whatever CS pin being used
#define FILENAME "csvLog.csv"
File myFile;

/* Barometer global declaration */
#include "MS5611.h"             // library for MS5611 barometer/thermometer
MS5611 MS5611(0x77);   // 0x76 = CSB to VCC; 0x77 = CSB to GND
#define SEA_LEVEL 1013.25
int result;
float celcius, pressure, altitude;

/* Declaration for an SSD1306 display connected to I2C (SDA, SCL pins) */
/*
#include <Wire.h>            

#include "SSD1306Ascii.h"       // Must use this library NOT adafruit library
#include "SSD1306AsciiWire.h"
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define I2C_ADDRESS 0x3C  // Default address for 128x64 display. Change if needed
SSD1306AsciiWire oled;
*/

/* Declarations for moving average for pressure */
#include <movingAvg.h>
movingAvg avgAlt(10);    // moving average with 10 data points

/* Define globals */
uint32_t start, stop, count;  // variables for time elapsed and data point number
String fileLine;

/* Used to calculate apogee. Must be global so they persist between loops */
int alt1, alt2;
double vel1, vel2;
double acc;
unsigned long time1, time2;
boolean deployed = false;

/*<><><><><><><><><><><><><><><><><><><>SETUP<><><><><><><><><><><><><><><><><><><><><>*/

void setup()
{
  // Start serial
  Serial.begin(115200);
  while( !Serial ){ ; }   // loop until serial is connected

  /* ----------------Initialize and test Barometer -------------------*/
  Serial.println("Initializing Barometer...");
  if( ! MS5611.begin() ){
    Serial.println("Barometer Not Found");
    for(;;); // Barometer not found is fatal error. Loop forever
  }
  Serial.println("Barometer Found");

  
  /* ---------------Initialize and test SD reader------------------ */
  Serial.println("Initializing SD card...");
  if( ! SD.begin(SD_PIN) ){
    Serial.println("SD Not found!");
  }
  else{
    Serial.println("SD Found!");
  }
   // If the file does not already exist, create it
  if( !SD.exists(FILENAME) ){
    Serial.print("Creating file: ");
    Serial.println(FILENAME);
    myFile = SD.open(FILENAME, FILE_WRITE);
    myFile.println("DUR,TEMP,PRES,ALT,AVG_ALT,VEL,ACC,PROJ_APOGEE");
    myFile.close();
  }

  //ouput code for initialization of file
  if( SD.exists(FILENAME) ){
    Serial.print("File: ");
    Serial.print(FILENAME);
    Serial.println(" Exists on SD");
  }
  else{
    Serial.print("File: ");
    Serial.print(FILENAME);
    Serial.println(" does not exist and was not initialized");
    //for(;;);
  } 
  
  // Start count for serial, print header to serial
  Serial.println("CNT\tDUR\tTEMP\tPRES\tAltitude\tAlt Avg\t");

  /* Start moving average object */
  avgAlt.begin();

  /* Start counter */
  start = millis();
  
  delay(1000);
}

/*<><><><><><><><><><><><><><><><><><><>LOOP<><><><><><><><><><><><><><><><><><><><><>*/

void loop()
{
  stop = millis();     // stop counter for this loop

  /* Get data from Barometer */
  result = MS5611.read();
  celcius = MS5611.getTemperature();
  pressure = MS5611.getPressure();

  /* Calculate Altitude, Round appropriately for integer conversion, then add to average */
  altitude = (1 - pow((pressure / SEA_LEVEL), 0.190284)) * 145366.45;
  int altToInt = ( altitude - floor(altitude) > 0.5 ) ? ceil(altitude) : floor(altitude);   // round appropriately
  int avg = avgAlt.reading(altToInt);

  /* Using time and altitude, calculate velocity and acceleration */

  time2 = time1;
  time1 = millis();

  //Make sure to add checks that you have valid values for alt, vel and acc
  alt2 = alt1;
  alt1 = avgAlt.getAvg();

  vel2 = vel1;
  double vel1 = (alt1 - alt2)/(time1 - time2);

  double acc = (vel1 - vel2)/(time1 - time2);

  /* Using alt, vel and acc calculate apogee */
  double projectedApogee = ((vel1 * vel1)/(-2 * acc)) + alt1;

  if(projectedApogee > (10000)){
    // Already deployed
    if(deployed == true){
    }
    else {
      // Deploy the airbreaks
      deployed = true;
    }
  }
  else {
    // If airbreaks are deployed, retract them back into the body tube
    if(deployed == true){
      // Retract the airbreaks
      deployed = false;
    }
  }

  /* Print results to serial for debugging */
  Serial.print(count);
  count++;
  Serial.print("\t");
  Serial.print((stop - start)/1000.0);
  Serial.print("\tTemp:");
  Serial.print(celcius, 2);
  Serial.print("\tPressure: ");
  Serial.print(pressure, 2);
  Serial.print("\tAltitude: ");
  Serial.print(altitude);
  Serial.print("\taltToInt: ");
  Serial.print(altToInt);
  Serial.print("\tAlt Avg: ");
  Serial.println(avg);
    

  /* Print results to file for storage */  
  myFile = SD.open(FILENAME, FILE_WRITE);
  myFile.print((stop - start) / 1000.0, 2);      // print time elapsed. divide 1000 because millis
  myFile.print(",");
  myFile.print(celcius);
  myFile.print(",");
  myFile.print(pressure);
  myFile.print(",");
  myFile.print(altitude);
  myFile.print(",");
  myFile.print(avg);
  myFile.print(",");
  myFile.print(vel1);
  myFile.print(",");
  myFile.print(acc);
  myFile.print(",");
  myFile.println(projectedApogee);
  myFile.close();

  //delay(100);
  
}

// -- END OF FILE --

/*NOTES:

  1) Must add error checkers during loop to make sure that all readings have valid readings
  
  2) Should add a variable that is calibrated to the ground level, not sea level. Currently,
  the projected apogee is compared against sea level when it should be compared against ground
  level.
  
  3) Other methods to calculate velocity include installing a pitot tube or integrating over 
  all of the accelerometer point readings. These should be explored, however some cons to 
  integrating over all the data readings is that this could be expensive in time and a pitot
  tube may impact air flow of the rocket if extended from the cone. Also, pitot tubes are rather
  expensive.
  
END OF NOTES*/
