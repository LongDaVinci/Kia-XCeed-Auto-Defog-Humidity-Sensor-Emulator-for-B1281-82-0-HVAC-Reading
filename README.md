# Kia-XCeed-Auto-Defog-Humidity-Sensor-Emulator-for-B1281-82-0-HVAC-Reading
Auto Defog humidity sensor emulator for Kia XCeed/Ceed 2022 CD with B1281 or fixed 0% HVAC humidity reading. Uses a DHT22 and low-power microcontroller to generate a compatible PWM signal for the climate control unit and restore proper humidity detection.
A DIY Auto Defog humidity sensor emulator for Kia XCeed / Ceed CD vehicles.

This project was created to solve a real issue on a Kia XCeed 2022 with climate control unit series **97250-J7...**, where the HVAC live data parameter for the **Auto Defog humidity sensor** stayed fixed at **0%**, and the system stored **B1281**, even though power, ground, and wiring continuity were all correct.

After practical testing, it turned out that the HVAC control unit does **not** accept a simple analog signal for this input.  
It correctly reads a **PWM signal** only within a valid operating window.

This emulator reads ambient humidity with a **DHT22** and outputs a PWM signal compatible with the Kia climate control module.

---

# Problem

Observed symptoms:

- Auto Defog humidity reading fixed at **0%** in HVAC diagnostics
- **B1281** stored in the climate control module
- Original windshield humidity sensor powered correctly but not recognized by the HVAC unit
- Issue appeared or became visible after windshield replacement
- The replacement sensor mounted on the windshield appeared suspicious or incompatible

---

# Solution

Build a small electronic emulator that:

- reads ambient humidity using a **DHT22**
- converts humidity into a **PWM signal**
- sends that signal directly to the HVAC humidity sensor input

In my case, this solution worked correctly:

- HVAC now reads about **48–50% humidity**
- the value updates normally
- no more sensor error
- **B1281 disappears**
- Auto Defog logic works again

---

# Tested on

Tested on:

- **Kia XCeed CD 2022**
- Engine **G3LE**
- Climate control unit family **97250-J7...**

Likely relevant to other vehicles in the same family:

- Kia Ceed CD
- Kia ProCeed CD
- Kia XCeed CD

---

# Important notice

This project is a practical workaround for a specific real-world issue.

Use it only after verifying that:

- sensor power is present
- sensor ground is present
- signal wiring continuity to the HVAC module is OK
- the HVAC input is functional

If your original system is working correctly, do **not** replace it with this emulator.

This is an experimental DIY automotive electronics project.  
Build and install it at your own risk.

---

# How it works

The original HVAC humidity sensor uses three wires:

- **+5V**
- **GND**
- **signal**

From real testing on the vehicle:

- a plain **analog** voltage was **not accepted**
- a **PWM** signal **was accepted**
- if PWM goes outside a valid range, the HVAC module falls back to **0%** and treats it as an error

In my tests, the valid PWM window was approximately:

- minimum valid PWM: **25**
- maximum valid PWM: **225**

So humidity from **0% to 100%** is mapped into a PWM range of **25 to 225**.

---

# Hardware required

- 1x **LGT8F328** microcontroller board
- 1x **DHT22** humidity sensor
- wires / connectors
- small perfboard or custom PCB
- enclosure or protected mounting solution for in-car use

Optional but strongly recommended:

- inline fuse
- automotive transient protection
- heat shrink tubing
- proper vibration-resistant mounting
- secure insulated wiring

---

# Wiring

## DHT22 -> microcontroller

- VCC -> 5V
- GND -> GND
- DATA -> pin defined in the code (`DHT22_PIN`)

## Microcontroller -> vehicle

- vehicle sensor +5V -> microcontroller VCC
- vehicle sensor GND -> microcontroller GND
- microcontroller PWM output -> vehicle humidity signal wire

In my code:

- DHT22 on **pin 5**
- PWM output on **pin 3**

---

# Arduino code

```cpp
#include "DHT.h" //adafruit library

#define DHT22_PIN 5
#define DHTTYPE DHT22

#define PWM_PIN 3
DHT dht(DHT22_PIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(DHT22_PIN, INPUT);
  pinMode(PWM_PIN, OUTPUT);
  dht.begin();
}

void loop() {

  float h = dht.readHumidity();

  analogWrite(PWM_PIN, map(h, 0, 100, 225, 25));
  Serial.print(map(h, 0, 100, 225, 25));
  Serial.print(" | ");
  Serial.print(h);
  Serial.println();
  delay(1000);
}
