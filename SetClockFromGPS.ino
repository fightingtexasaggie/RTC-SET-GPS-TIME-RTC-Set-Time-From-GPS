#include <RTClib.h>
#include <Wire.h>
#include <TinyGPS.h>
#include <Time.h>

#define SerialGPS Serial

int ledPin = 13;                 // LED connected to digital pin 13

RTC_DS1307 RTC;
TinyGPS gps; 

// Offset hours from gps time (UTC)
const int offset = -6;   

void setup()
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  digitalWrite(ledPin, LOW); // initalize off
  SerialGPS.begin(4800);
  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
   RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
}

void loop()
{
  while (SerialGPS.available()) {
    if (gps.encode(SerialGPS.read())) { // process gps messages
      // when TinyGPS reports new data...
      unsigned long age;
      int Year;
      byte Month, Day, Hour, Minute, Second;
      gps.crack_datetime(&Year, &Month, &Day, &Hour, &Minute, &Second, NULL, &age);
      if (age < 500) {
        // set the Time to the latest GPS reading
        setTime(Hour, Minute, Second, Day, Month, Year);
        adjustTime(offset * 3600);
        RTC.adjust(now());
        digitalWrite(ledPin, HIGH);
      }
    }
  }
}
