


// Include Libraries
  #include <RTClib.h>
  #include <Wire.h>
  #include <Servo.h>
  #include "U8glib.h"
  // Init constants
  const int TempPin = 8;
  const int ServoPin = 9;


  // Init global variables
  
  // construct objects
  U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
  Servo myServo;
  RTC_DS3231 rtc;

void setup() {
  // Attach objects
  myServo.attach(ServoPin);

  // init communication
  Serial.begin(9600);
  Wire.begin();
  // Init Hardware
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  u8g.setFont(u8g_font_unifont);
  u8g.firstPage(); 
}

void loop() {
  oledWrite(String(getTime()));
  servoWrite(measureTemp());
}


/*
*This function reads time from an ds3231 module and package the time as a String
*Parameters: Void
*Returns: time in hh:mm:ss as String
*/
String getTime() {
  DateTime now = rtc.now();
  Serial.print("Time");
  Serial.println(String(now.hour()) + " " + String(now.minute()) + " " + String(now.second()));
}

/*
* This function reads an analog pin connected to an analog temprature sensor and calculates the corresponding temp
*Parameters: Void
*Returns: temperature as float
*/
float measureTemp() {
  float temp = 0;
  float R1 = 10000;  // value of R1 on board
  float logR2, R2, T;
  float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;  //steinhart-hart coeficients for thermistor

  int Vo = analogRead(TempPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);  //calculate resistance on thermistor
  logR2 = log(R2);
  temp = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));  // temperature in Kelvin
  temp = temp - 273.15;                                           //convert Kelvin to Celcius

  return temp;
}


/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String text) {
  u8g.firstPage();
  do {

    u8g.drawStr(0, 22, text.c_str());
  } while (u8g.nextPage());
}

/*
* takes a temprature value and maps it to corresppnding degree on a servo
*Parameters: - value: temprature
*Returns: void
*/
void servoWrite(float value) {
  myServo.write(value);
}