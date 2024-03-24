#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

unsigned long previousMillis = 0;
const long interval = 500;  // interval in milliseconds

float calibration_value = 21.34;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10], temp;

float ph_act;

const int methanePin = A0;   // Analog pin connected to MQ-4 sensor
const int turbidityPin = A1; // Analog input pin for turbidity sensor
const int phPin = A3;        // Analog input pin for pH sensor
#define ONE_WIRE_BUS 3        // Data wire is connected to pin 3 on the Arduino

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

const int buttonPin = A2;
int buttonState = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

bool hasStarted = false;

// Function prototypes
void readAndDisplayPH();
void readAndDisplayTurbidity();
void readAndDisplayMethane();
void readAndDisplayTemperature();
void displayResult();
void handleButtonPress();

// Variables for sensor statuses
char phStatus[7];
char TurStatus[7];
char metStatus[7];

void setup() {
  Serial.begin(9600); // Initialize serial communication with the PC
  sensors.begin();    // Initialize the DS18B20 sensor
  pinMode(methanePin, INPUT); // Set methane sensor pin as an input
  pinMode(turbidityPin, INPUT); // Set turbidity sensor pin as an input
  pinMode(buttonPin, INPUT_PULLUP); // Set push button pin as an input with internal pull-up resistor
  lcd.init();         // initialize the lcd
  lcd.backlight();    // Turn on the LCD screen backlight

  lcd.setCursor(0, 0);
  lcd.print("Press to start");
}

void loop() {
  
}

void readAndDisplayPH() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place PH sensor in milk");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press to read pH");

  // Handle button press
  void handleButtonPress() {
    // Increment press count
    static int pressCount = 0;
    pressCount++;

    // Display different sensor values based on the press count
    if (pressCount == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Loading PH value...");
      delay(2000); // Adjust this delay as needed for loading time

      int buffer_arr[10];
      int temp;
      int avgval = 0;

      for (int i = 0; i < 10; i++) { 
        buffer_arr[i] = analogRead(phPin);
        delay(30);
      }
      
      for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 10; j++) {
          if (buffer_arr[i] > buffer_arr[j]) {
            temp = buffer_arr[i];
            buffer_arr[i] = buffer_arr[j];
            buffer_arr[j] = temp;
          }
        }
      }

      for (int i = 2; i < 8; i++)
        avgval += buffer_arr[i];
      
      float volt = (float)avgval * 5.0 / 1024 / 6; 
      float ph_act = -5.70 * volt + calibration_value;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("pH: ");
      lcd.print(ph_act, 2);

      // Check if pH is within the acceptable range
      if (ph_act >= 6.4 && ph_act <= 6.8) {
        lcd.setCursor(0, 1);
        lcd.print("Good");
        strcpy(phStatus, "Good");
      } else if(ph_act >= 6.2 && ph_act <= 6.4){
        lcd.setCursor(0, 1);
        lcd.print("Average");
        strcpy(phStatus, "Average");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Bad");
        strcpy(phStatus, "Bad");
      }
    }
  }
}

void readAndDisplayMethane() {
  int methaneSum = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Loading...");

  // Delay for loading time
  delay(2000); // Adjust this delay as needed for loading time

  // Read and sum up 10 methane values
  for (int i = 0; i < 10; i++) {
    int methaneValue = analogRead(methanePin);
    methaneSum += methaneValue;
    delay(100); // Delay between readings, adjust as needed
  }

  // Calculate average methane value
  int avgMethane = methaneSum / 10;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Methane: ");
  lcd.print(avgMethane);

  // Display methane status
  if (avgMethane <= 200) {
    lcd.setCursor(0, 1);
    lcd.print("Good");
    strcpy(metStatus, "Good");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Bad");
    strcpy(metStatus, "Bad");
}
