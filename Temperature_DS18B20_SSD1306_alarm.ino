#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_SSD1306.h>

#define ONE_WIRE_BUS 8                // DS18B20 data wire is connected to input 8
#define OLED_RESET 4                  // Adafruit needs this but we don't use for I2C

DeviceAddress thermometerAddress;     // custom array type to hold 64 bit device address

Adafruit_SSD1306 display(OLED_RESET); // create a display instance
OneWire oneWire(ONE_WIRE_BUS);        // create a oneWire instance to communicate with temperature IC
DallasTemperature tempSensor(&oneWire);  // pass the oneWire reference to Dallas Temperature

float THRESHOLD = 65.00;


void setup()   {


  Serial.begin(9600);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C address of the display
  display.clearDisplay();                     // clear the display buffer
  display.display();                          // update display

  tempSensor.begin();                         // initialize the temp sensor
  tempSensor.getAddress(thermometerAddress, 0);
  tempSensor.setResolution(thermometerAddress, 12);      // set the temperature resolution (9-12)
}


void loop() {

  tempSensor.requestTemperatures();                      // request temperature sample from sensor on the one wire bus
  displayTemp(tempSensor.getTempC(thermometerAddress));  // show temperature on OLED display

  delay(500);                                            // update readings every half a second
}

void displayTemp(float temperatureReading) {             // temperature comes in as a float with 2 decimal places

  // set up OLED text size and print the temperature data
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // show temperature °C
  display.print(temperatureReading, 2);  // rounded to 2 decimal places
  display.print((char)247);              // degree symbol
  display.println(" C");

  if(temperatureReading<=THRESHOLD)
  {digitalWrite(6,LOW); digitalWrite(7,HIGH);}
    else
  {digitalWrite(6,HIGH); digitalWrite(7,LOW);}   

  // show temperature °F
  display.print(DallasTemperature::toFahrenheit(temperatureReading), 2); // rounded to 2 decimal places
  display.print((char)247);                                              // degree symbol
  display.println(" F");

  display.display();                    // update the OLED display with all the new text
}
