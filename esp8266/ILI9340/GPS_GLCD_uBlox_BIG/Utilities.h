// From Arduino Forum
float TempCelsius (void) {
  uint8_t i;
  float average;
  int samples[] = { 0, 0, 0, 0, 0} ;
 
  // take 5 samples in a row, with a slight delay
  for (i=0; i<5; i++) {
   samples[i] = analogRead(0);
   delay(10);
  }
 
  // average all the samples
  average = 0;
  for (i=0; i< 5; i++) {
     average += samples[i] ;
  }
  average /= 5 ;
  average = 1023 / average - 1 ;
  average = 10000.0 / average ;
  float steinhart ;
  steinhart = average / 10000.0 ;      // (R/Ro)
  steinhart = log(steinhart) ;         // ln(R/Ro)
  steinhart /= 3950.0 ;                // 1/B * ln(R/Ro)
  steinhart += 1.0 / (25.0 + 273.15) ; // + (1/To)
  steinhart = 1.0 / steinhart ;        // Invert
  steinhart -= 273.15 ;                // convert to C
  return (steinhart) ;
}


// From Arduino forum
int dow(uint8_t y, uint8_t m, uint8_t d)
{
  static uint8_t t[] = {
    0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4  };
  y -= m < 3;
  return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;    // 0 = Sunday, 1 = Monday, etc
}

// Written by Ray
static String DayOfWeek[] =
{
  "Sun",    // 0
  "Mon",
  "Tue",
  "Wed",
  "Thu",
  "Fri",
  "Sat" }
;

// Written by Ray
static String NameOfMonth[] =
{
  "JAN",  // index 0
  "FEB",
  "MAR",
  "APR",
  "MAY",
  "JUN",
  "JUL",
  "AUG",
  "SEP",
  "OCT",
  "NOV",
  "DEC"  }
;

// Arduino Forum
static int DaysInMonth[] =
{
  31, // index 0 == January
  28, 
  31,
  30,
  31,
  30,
  31,
  31,
  30,
  31,
  30,
  31  }
;

/*
WST	West Samoa Time			Pacific		 13
NZST	New Zealand Standard Time	Pacific		 12
SBT	Solomon IslandsTime		Pacific		 11
PGT	Papua New Guinea Time		Pacific		 10
JST	Japan Standard Time		Asia		 9
CST	China Standard Time		Asia		 8
ICT	Indochina Time			Asia		 7
BST	Bangladesh Standard Time	Asia		 6
UZT	Uzbekistan Time			Asia		 5
MSK	Moscow Standard Time		Europe		 4
AST	Arabia Standard Time		Asia		 3
EET	Eastern European Time		Europe		 2
CET	Central European Time		Europe		 1
GMT	Greenwich Mean Time		Europe		 0
AZOT	Azores Time			Atlantic	-1
FNT	Fernando de Noronha Time	South America	-2
UYT	Uruguay Time			South America	-3
AST	Atlantic Standard Time		North America	-4
EST	Eastern Standard Time		North America	-5
CST	Central Standard Time		North America	-6
MST	Mountain Standard Time		North America	-7
PST	Pacific Standard Time		North America	-8
AKST	Alaska Standard Time		North America	-9
HAST	Hawaii-Aleutian Standard Time	North America	-10
SST	Samoa Standard Time		Pacific		-11
Y	Yankee Time Zone		Military	-12
*/
