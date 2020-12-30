# Couch Tracker for Arduino

Mathieu Guillame-Bert (achoum@gmail.com)
2020

Track the amount of time you spend in your couch.

Couch Tracker helps you track the amount of time you spend in a location. For example, it can be used to track the amount of time in front of your tv.

This controller detects the presence of persons in front of it, and displays the duration of the last activation (if activated) and the sum of activation. Every 30minutes of activities, the controller emits a short buzz. In addition, the controller also displays the time, temperature, and humidity.

The module requires:
- A passive buzzer (pin 9).
- DHT11: Temperature and humidity sensor (pin 10).
- HC-SR501: PIR Sensor (pin 8).
- DS3231 RTC:  Real Time Clock Module (i2c bus).
- LCD1602: 16x2 LCD display (pins 2-7).
- 2 10k Potentiometers (for contrast and retro-lighting of the LCD).

The display is as follow:

```
====================
[AA:AA] [BB]°C [CC]%
[D:DD:DD] [EE:EE:EE]
====================
```

With:
- A: Time
- B: Termperature
- C: Humidity
- D: Duration of continuous activity
- E: Sum of activity durations


![Outside](https://github.com/achoum/couch-tracker/blob/master/outside.jpg)

![Inside](https://github.com/achoum/couch-tracker/blob/master/inside.jpg)
