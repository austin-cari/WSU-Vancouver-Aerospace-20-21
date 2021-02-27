/* This code is for reading temperature and humidity data from a DHT11 digital sensor
 *  and writing that information to an attached SD card in a HW-125 SD data loggin module.
 *  
 *  The Circuit:
 *    SD Card attached to SPI Bus as follows (applicable only to Arduino Mega)
 *    MOSI  - Pin 50
 *    MISO  - Pin 51
 *    CLK   - Pin 52
 *    CS    - Pin 53
 *    
 *    Temp Sensor attached as follows (as seen from front, pins are 1,2,3 going left to right)
 *    DHT11 only accurate to +/- 1 degree celcius and +/- 1 % humidity
 *    Pin 1 - Pin 2 on Arduino (can change to any digital port to use an input)
 *    Pin 2 - VCC
 *    Pin 3 - GND
 *      
 */

/* Set up for Temp Sensor */
#include <dht11.h>    //include library for temp sensor
#define DHT11_PIN 2   //macro for input pin
dht11 DHT11;          //global object of type dht11

/*Set up for SD Reader */
#include <SPI.h>                // allows use of Serial Peripheral Interface
#include <SD.h>                 // library for use of SD card reader
#define SD_PIN 53               // Change this line to be CS or SS pin on chip reader
#define FILENAME "csvLog.csv"   // Change name of file being written to
File myFile;                    // initialize global file variable

const int chipSelect = SD_PIN;  // SS pin, change as necessary to match wiring

void setup() {
  Serial.begin(9600);         // start serial at 9600 BAUD

  //wait for serial to connect
  while( !Serial ){
    ;
  }

  //Attempt to connect to card
  Serial.print("\nInitializing SD card...");
    
  if (!SD.begin(SD_PIN)){
    Serial.println("Initialization Failed");
    return;
  }    
  Serial.println("Initialized Properly!");


  // If the file does not already exist, create it
  if( !SD.exists(FILENAME) ){
    Serial.print("Creating file: ");
    Serial.println(FILENAME);
    myFile = SD.open(FILENAME, FILE_WRITE);
    myFile.println("Temperature,Humidity");
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
  }
}
  

void loop() {
  
  // put your main code here, to run repeatedly:
  int check = DHT11.read(DHT11_PIN);    // get serial reading

  // build string with sensor data, comma is for .csv file
  String dataString = "";        // empty string to add data to
  dataString += DHT11.temperature;
  dataString += ',';
  dataString += DHT11.humidity;

  // Open file to write, print string and newline, then close  
  myFile = SD.open(FILENAME, FILE_WRITE);
  if( myFile ){
    myFile.println(dataString);
    myFile.close();
  }

  //print string to serial too to see on serial monitor
  Serial.println(dataString);
  
  delay(500);
  
}
