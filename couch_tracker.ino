// "Couch Tracker" for Arduino.
// Mathieu Guillame-Bert (achoum@gmail.com)
// 2020
//
// Couch Tracker helps you track the amount of time you spend in a location. For
// example, it can be used to track the amount of time in front of your tv.
//
// This controller detects the presence of persons in front of it, and displays
// the duration of the last activation (if activated) and the sum of activation.
// Every 30minutes of activities, the controller emits a short buzz. In
// addition, the controller also displays the time, temperature, and humidity.
//
// The module requires:
// - A passive buzzer (pin 9).
// - DHT11: Temperature and humidity sensor (pin 10).
// - HC-SR501: PIR Sensor (pin 8).
// - DS3231 RTC:  Real Time Clock Module (i2c us).
// - LCD1602: 16x2 LCD display (pins 2-7).
// - 2 10k Potentiometers (for contrast and retro-lighting of the LCD).
//
// The display is as follow:
// ====================
// [AA:AA] [BB]°C [CC]%
// [D:DD:DD] [EE:EE:EE]
// ====================
//
//  With:
//  - A: Time
//  - B: Termperature
//  - C: Humidity
//  - D: Duration of continuous activity
//  - E: Sum of activity durations
//

#include <LiquidCrystal.h>
#include <Wire.h>
#include <DS3231.h>
#include <DHT.h>

// Uncomment to initialize the Serial bus and print debug information.
// #define SERIAL_DEBUG

// Uncomment to set the time of the internal clock to the compilatio time during the setup phase.
// #define SET_TIME

// Pins and module handelers.
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
static const int PIN_PIR = 8;
static const int PIN_BUZZER = 9;
static const int PIN_DHT = 10;
static const int PIN_BOARD_LED = 13;

DS3231 clock;

DHT dht(PIN_DHT, DHT11);

// Makes a short buzz every "ALERT_INTERVAL_MS" of continuous activity.
const unsigned long ALERT_INTERVAL_MS = 30ul * 60ul * 1000ul; // 30min

bool last_pir_active = false;
unsigned long last_start_pir = 0;
unsigned long sum_pir = 0;
int activity_intervals = 0;

struct PirActivity {
  // Duration of the current session in ms.
  // 0 if the session is inactive.
  unsigned long current_session_duration_ms;

  // Sum of the sesssions, in ms.
  unsigned long sum_duration_ms;
};

void setup() {

#ifdef SERIAL_DEBUG
  Serial.begin(9600);
  Serial.println("Initialize");
#endif

  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_PIR, INPUT);
  lcd.begin(16, 2);
  clock.begin();
  dht.begin();

#ifdef SET_TIME
  clock.setDateTime(__DATE__, __TIME__);
#endif
}

PirActivity udpate_pir(const bool active) {
  const unsigned long tick = millis();
  PirActivity activity;

  if (active) {
    if (!last_pir_active) {
      last_start_pir = tick;
      activity_intervals = 0;
    }
    activity.current_session_duration_ms = tick - last_start_pir;
    activity.sum_duration_ms = sum_pir + activity.current_session_duration_ms;

    int cur_activity_interval = activity.current_session_duration_ms / ALERT_INTERVAL_MS;
    if (activity_intervals != cur_activity_interval) {
      tone(PIN_BUZZER, 82, 100);
      activity_intervals = cur_activity_interval;
    }

  } else {
    if (last_pir_active) {
      sum_pir += tick - last_start_pir;
    }
    activity.current_session_duration_ms = 0;
    activity.sum_duration_ms = sum_pir;
  }
  last_pir_active = active;
  return activity;
}

// Converts a count of ms into a string "HH:MM:SS".
// The output buffer is valid until the next call to "duration_ms_to_str".
const char* duration_ms_to_str(const unsigned long ms) {
  static char buffer[256] = {0};
  const unsigned long total_s = ms / 1000;
  const unsigned long s = total_s % 60;
  const unsigned long total_m = total_s / 60;
  const unsigned long m = total_m % 60;
  const unsigned long h = total_m / 60;
  sprintf(buffer, "%02ul:%02ul:%02ul", h, m, s);
  return buffer;
}

void loop() {
  // Get values
  const RTCDateTime dt = clock.getDateTime();
  const bool pir = digitalRead(PIN_PIR) != 0;
  const float temperature = dht.readTemperature();
  const float humidity = dht.readHumidity();

  // Computation
  const PirActivity pir_activity = udpate_pir(pir);

  // Display
  char line_1[32 + 1];
  lcd.setCursor(0, 0);
  sprintf(line_1, "%02d:%02d %2dC %2d%%", dt.hour, dt.minute, (int)temperature, (int)humidity);
  lcd.print(line_1);

  lcd.setCursor(0, 1);
  if (pir) {
    lcd.print(duration_ms_to_str(pir_activity.current_session_duration_ms) + 1);
  } else {
    lcd.print("-:--:--");
  }
  lcd.print(" ");
  lcd.print(duration_ms_to_str(pir_activity.sum_duration_ms));

  delay(500);
}
