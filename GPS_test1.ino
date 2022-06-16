#include <SoftwareSerial.h>

// Choose two Arduino pins to use for software serial
int RXPin = 2;
int TXPin = 3;

//Default baud of NEO-6M is 9600
int GPSBaud = 9600;
int count;
String serialData;
bool accident_detected = false;
String formatted_str;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);

String createMessageFromGpgga(String serialData, bool accident_detected);

//==================================================================

void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
}


//==================================================================

void loop()
{
  count++;
  // Displays information when new sentence is available.
  if(gpsSerial.available() > 0){
      serialData = gpsSerial.readStringUntil('\n');
      
      if(serialData.substring(1,6) == "GPGGA"){
//        Serial.println("Thilina");
        formatted_str = createMessageFromGpgga(serialData, true);
      } 
//      
//      Serial.println("==============");
//      Serial.println(serialData);
//      Serial.println(formatted_str);
//      Serial.println("==============");
      
  }
  else{
    Serial.print("Not detected\n");
  }

  if(count == 70 || accident_detected == 1){
    Serial.print(formatted_str);
    if (count == 70) {
      count = 0;
    }
  }
  delay(100);
}


//==================================================================


//$GPGGA,103839.00,0647.80528,N,07954.02007,E,1,07,1.21,19.2,M,-95,10,24,269,,11,02,100,*76
String createMessageFromGpgga(String serialData, bool accident_detected){
  String formatted_str=","+serialData.substring(7,13)+","+serialData.substring(17,27)+","+serialData.substring(30,41);
  if (accident_detected){
    return "1"+formatted_str;
  }else{
    return "0"+formatted_str;
  }
}


// GPGGA
/*
Here are a couple of popular NMEA messages similar to the $GPGGA message with GPS coordinates in them (these can possibly be used as an alternative to the $GPGGA message):
$GPGLL, $GPRMC
*/
