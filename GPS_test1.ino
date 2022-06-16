#include <SoftwareSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Choose two Arduino pins to use for software serial
#define GpsRXPin 2
#define GpsTXPin 3

// Variables
int count=0;
String serialData;
bool accident_detected = false;
String formatted_str;

// Accelerometer
Adafruit_MPU6050 gy_521;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(GpsRXPin, GpsTXPin);

String createMessageFromGpgga(String serialData, bool accident_detected);
String createMessageFromGprmc(String serialData, bool accident_detected);

//==================================================================

void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's and bluethooth module's default baud
  gpsSerial.begin(9600);
  Serial.begin(9600);

  gy_521.begin();
//  if (gy_521.begin())
//  {
////      Serial.print("Accelerometer found\n");
//  }
//  else
//  {
////      Serial.print("Error\n");
//  }
}


//==================================================================

void loop()
{
  count++;

  // Accelerometer code
  sensors_event_t a, g, temp;
  gy_521.getEvent(&a, &g, &temp);

  float acc_x = a.acceleration.x;
  float acc_y = a.acceleration.y;
//  float acc_z = a.acceleration.z;

  // Identifying an accident
  if (acc_x > 5 || acc_y > 5){
    accident_detected = true;  
  }

  
  // GPS Module code
  if(gpsSerial.available() > 0){
      serialData = gpsSerial.readStringUntil('\n');
//      Serial.println(serialData);
      
      if(serialData.substring(1,6) == "GPRMC"){
        formatted_str = createMessageFromGprmc(serialData, accident_detected);
      } 
  }
  
  if(count == 50 || accident_detected){
    if (count == 50) {
      Serial.println("0"+formatted_str);
      count = 0;
    }else if(accident_detected){
      Serial.println("1"+formatted_str);
      accident_detected = false;  
    }
  }
  
  delay(150);
}


//==================================================================


// $GPGGA,103839.00,0647.80528,N,07954.02007,E,1,07,1.21,19.2,M,-95,10,24,269,,11,02,100,*76
// $GPRMC,180351.00,A,0647.67833,N,07954.18281,E,0.066,,160622,,,A
String createMessageFromGpgga(String serialData, bool accident_detected){
  return ","+serialData.substring(7,13)+","+serialData.substring(17,27)+","+serialData.substring(30,41);
}

String createMessageFromGprmc(String serialData, bool accident_detected){
  return ","+serialData.substring(7,13)+","+serialData.substring(19,29)+","+serialData.substring(32,43);
}


// GPGGA
/*
Here are a couple of popular NMEA messages similar to the $GPGGA message with GPS coordinates in them (these can possibly be used as an alternative to the $GPGGA message):
$GPGLL, $GPRMC
*/
