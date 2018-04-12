#include <Arduino.h>
#include <SPI.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>
//#include "DHT.h"
//DHT dht;

TinyGPS gps;

long syncTime=1000;


SoftwareSerial ss(4, 3); // Arduino RX, TX ,
static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);
#define LENG 31   //0x42 + 31 bytes equal to 32 bytes
unsigned char buf[LENG];

long temps=0;
void setup() {
    Serial.begin(9600);
    ss.begin(9600);
    //pinMode(BOOL_PIN, INPUT);                        //set pin to input
    //digitalWrite(BOOL_PIN, HIGH);
    temps=millis(); 

}

void loop() {
 if(millis() - temps > syncTime){
  String trame = getTrame();
  Serial.println(trame);
  temps=millis();
  
 }
 delay(100);

}


String getTrame(){
String dates;
  int year;
  byte month, day, hour, minute, second, hundredths;
float flat, flon;
  unsigned long age, date, time, chars = 0;
  gps.f_get_position(&flat, &flon, &age);
  String trame="";
  trame+=String(flat, 6);
  trame+=":";
  trame+=String(flon, 6);
  //trame+=":";
  smartdelay(1000);
 return trame;
}



static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
    {
      //ss.print(Serial.read());
      gps.encode(ss.read());
    }
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}

static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartdelay(0);
}

static void print_date(TinyGPS &gps)
{
   String dates;
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    Serial.print("********** ******** ");
  else
  {
    char sz[32];
    sprintf(sz, "%02d%02d%02d %02d/%02d/%02d ",
        hour, minute, second, month, day, year);
    //Serial.print(sz);
    dates+=sz;
    //Serial.println("la date");
    Serial.println(dates);
  }
  //print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  smartdelay(0);
}


static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartdelay(0);
}
