# Couch Tracker for Arduino

Mathieu Guillame-Bert (achoum@gmail.com)
2020

The Couch Tracker tracks the amount of time spent in your couch (or any other locations).

The system works by detecting the presence of persons in front of it, and summing and displaying the duration of activation (both last and total sum). Every 30minutes of activation, the controller emits a short buzz. The system also displays the time, temperature, and humidity level.

The module requires:

- A passive buzzer (pin 9).
- DHT11: Temperature and humidity sensor (pin 10).
- HC-SR501: PIR Sensor (pin 8).
- DS3231 RTC:  Real Time Clock Module (i2c bus).
- LCD1602: 16x2 LCD display (pins 2-7).
- 2 10k Potentiometers (for contrast and retro-lighting of the LCD).

The display is as follow:

```raw
====================
[AA:AA] [BB]°C [CC]%
[D:DD:DD] [EE:EE:EE]
====================
```

With:

- A: Time
- B: Temperature
- C: Humidity
- D: Duration of continuous activity
- E: Sum of activity durations

![Outside](https://github.com/achoum/couch-tracker/blob/master/outside.jpg)

![Inside](https://github.com/achoum/couch-tracker/blob/master/inside.jpg)
