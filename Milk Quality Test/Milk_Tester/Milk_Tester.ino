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
