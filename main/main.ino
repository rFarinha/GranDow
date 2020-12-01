//********* LIBRARIES ************
// include the library for the LCD:
#include <LiquidCrystal.h>
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
RTC_DS3231 rtc;

//********* SETTINGS *************
#define language 1 // 0 = English, 1 = Portuguese, 2 = Spanish, 3 = German, 4 = Italian, 5 = French

bool showSeconds = true;

// LCD Pins
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7; // this pin numbers are for an LCD shield
const int numRows = 2;
const int numCols = 16;

//**** INITIALIZE VARIABLES******
//char daysOfTheWeek[7][12];

#if language == 0   // ENGLISH
  char daysOfTheWeek[7][12] = {"SUNDAY", "MONDAY", "TUESDAY", "WEDNSDAY", "THURSDAY", "SEXTA", "SABADO"};
#elif language == 1 // PORTUGUESE
  char daysOfTheWeek[7][12] = {"DOMINGO", "SEGUNDA", "TERCA", "QUARTA", "QUINTA", "SEXTA", "SABADO"};
#elif language == 2 // SPANISH
  char daysOfTheWeek[7][12] = {"DOMINGO", "LUNES", "MARTES", "MIERCOLES", "JUEVES", "VIERNES", "SABADO"};
#elif language == 3 // GERMAN
  char daysOfTheWeek[7][12] = {"SONNTAG", "MONTAG", "DIENSTAG", "MITTWOCH", "DONNERSTAG", "FREITAG", "SAMSTAG"};
#elif language == 4 // ITALIAN
  char daysOfTheWeek[7][12] = {"DOMENICA", "LUNEDI", "MARTEDI", "MERCOLEDI", "GIOVEDI", "VENERDI", "SABATO"};
#elif language == 5 // FRENCH
  char daysOfTheWeek[7][12] = {"DIMANCHE", "LUNDI", "MARDI", "MERCREDI", "JEUDI", "VENDREDI", "SAMEDI"};
#endif

// initialize the library by associating any needed LCD interface pin
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(numCols, numRows);
  
  Serial.begin(57600);
  #ifndef ESP8266
    while (!Serial); // wait for serial port to connect. Needed for native USB
  #endif
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  // if adjusting RTC time needed
  //rtc.adjust(DateTime(2020, 11, 25, 23, 8, 30)); // YEAR, MONTH, DAY, HOUR, MINUTE, SECONDS
  
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // January 21, 2014 at 3am you would call:
  }

}


void loop() {
  // Add lcd clear to clean previous weekday letters
  lcd.clear();
  // Read Time
  DateTime now = rtc.now();

  // use dateBuffer to write leading 0 when value is single digit
  char dateBuffer[12];
  sprintf(dateBuffer,"%02u/%02u/%02u ",now.day(),now.month(),now.year()-2000);
  // Serial.print for debugging
  Serial.println(dateBuffer);
  
  // Print DAY OF WEEK
  lcd.setCursor(0, 0);
  // print the letter:
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);

    // Print TIME
  // Some languages dont have space to show seconds
  if(showSeconds){
    lcd.setCursor(8, 0);
    sprintf(dateBuffer,"%02u:%02u:%02u ",now.hour(),now.minute(),now.second());
  }else{
    lcd.setCursor(11, 0);
    sprintf(dateBuffer,"%02u:%02u ",now.hour(),now.minute());
  }
  lcd.print(dateBuffer);

  // Print DATE
  lcd.setCursor(4, 1);
  sprintf(dateBuffer,"%02u/%02u/%02u ",now.day(),now.month(),now.year()-2000);
  lcd.print(dateBuffer);
  delay(1000);
}
