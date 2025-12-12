# SmoothAxis

![Arduino](https://img.shields.io/badge/Arduino-Compatible-00979D.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)
![No Dependencies](https://img.shields.io/badge/dependencies-none-brightgreen.svg)

**Stabilize noisy potentiometers, faders, and joysticks.**

Stop your analog readings from flickering. SmoothAxis filters noise while keeping your controls responsive—no more fighting with magic numbers or watching your servo twitch.

Specify smoothing as a settle time in seconds. The filter adapts to your noise level automatically.

---


## Installation
**Arduino Library Manager:**   Search for '```SmoothAxis```' → Install


Or [download manually](https://github.com/Viderspace/Smooth-Axis-Arduino) and place in your Arduino `libraries` folder.


## Quick Start

```cpp
#include <SmoothAxis.h>

const int SENSOR_PIN = A0;
const int SENSOR_MAX = 1023;    // 10-bit ADC (use 4095 for ESP32)
const float SETTLE_TIME = 0.25; // Seconds to stabilize

SmoothAxis axis(SENSOR_MAX, SETTLE_TIME);

void setup() {
    Serial.begin(9600);
}

void loop() {
    axis.update(analogRead(SENSOR_PIN));

    if (axis.hasChanged()) {
        Serial.println(axis.read());  // Only prints on real movement
    }
}
```

That's it. No tuning required unless you want to.


## Choosing Your Settle Time

| Settle Time | Feel | Good For |
|-------------|------|----------|
| 0.05–0.15s | Snappy | Gaming, fast UI response |
| 0.20–0.35s | Balanced | Most projects |
| 0.50–1.0s | Smooth | Cinematic movement, lighting |

## Accuracy

![Settle-time accuracy](docs/step_response_accuracy.png)

Tested across clean and noisy conditions: **~1% timing error**, **zero false triggers**.

## Examples

Ready-to-run sketches in **File → Examples → SmoothAxis**:

| Example | What It Shows |
|---------|---------------|
| `Simple` | Basic potentiometer smoothing |
| `Precision` | Manual timing for variable loop rates |
| `Plotter` | Visualize raw vs. smoothed in Serial Plotter |
| `Calibration` | Fix pots that don't reach 0% or 100% |

## Features

- **Easy tuning** — set how fast SmoothAxis reacts to changes, in seconds
- **Sticky zones** — optional magnetic snap at 0% and 100% endpoints
- **Dead zones** — optionally clip unreliable sensor edges to clean values

- **No false triggers** — `hasChanged()` ignores jitter, catches real movement
- **Frame-rate independent** — works and feels the same on any board, at any processor speed


---
## Learn more here


<details>
<summary><strong>Select Mode</strong></summary>

### AUTO Mode (Default)

The simplest option. SmoothAxis measures your loop timing automatically.

```cpp
#include <SmoothAxis.h>

const int SENSOR_PIN = A0;
const int SENSOR_MAX = 1023;    // 10-bit ADC (use 4095 for ESP32)
const float SETTLE_TIME = 0.25; // Seconds to stabilize

// Default mode - timing handled internally via millis()
SmoothAxis axis(SENSOR_MAX, SETTLE_TIME);

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Read sensor and update filter
    axis.update(analogRead(SENSOR_PIN));

    // Only fires on meaningful change, not noise
    if (axis.hasChanged()) {
        Serial.println(axis.read());
    }
}
```

### LIVE Mode (Precision)

For fast chips or busy programs with varying workloads. You provide the exact time elapsed each frame, ensuring consistent smoothing even when loop speed fluctuates.

```cpp
#include <SmoothAxis.h>

const int SENSOR_PIN = A0;
const int SENSOR_MAX = 1023;
const float SETTLE_TIME = 0.25;

// LIVE mode - you provide delta time manually
SmoothAxis axis(SENSOR_MAX, SETTLE_TIME, SmoothAxis::LIVE);

unsigned long lastMicros = 0;

void setup() {
    Serial.begin(9600);
    lastMicros = micros();
}

void loop() {
    // Calculate exact time since last update
    unsigned long now = micros();
    float deltaTime = (now - lastMicros) / 1000000.0;  // Convert to seconds
    lastMicros = now;

    // Pass delta time with update
    axis.update(analogRead(SENSOR_PIN), deltaTime);

    if (axis.hasChanged()) {
        Serial.println(axis.read());
    }
}
```

</details>

<details>
<summary><strong>Utility & Diagnostics</strong></summary>

### Reading Values

```cpp
uint16_t value = axis.read();        // Smoothed value [0..maxRaw]
float normalized = axis.readFloat(); // Smoothed value [0.0..1.0]
bool changed = axis.hasChanged();    // True once per meaningful change
```

### Reset

```cpp
axis.reset();           // Clear history, start at zero
axis.reset(currentRaw); // Clear history, start at specific position
```

Useful after sleep/wake or mode changes.

### Diagnostics

```cpp
float noise = axis.getNoiseLevel();  // Current noise estimate [0.0..1.0]
float thresh = axis.getThreshold();  // Active detection threshold [0.0..1.0]
```

</details>

<details>
<summary><strong>Fine-Tuning: Dead Zones & Sticky Edges</strong></summary>

Use `fineTune()` if your sensor has issues at the endpoints—for example, a potentiometer that only reads 15–1010 instead of 0–1023.

```cpp
SmoothAxis axis(1023, 0.25);

void setup() {
    axis.fineTune(
        0.003,  // Sticky zone: magnetic snap at 0% and 100%
        0.02,   // Full off: treat bottom 2% as zero
        0.98    // Full on: treat top 2% as max
    );
}
```

| Parameter | Default | Purpose |
|-----------|---------|---------|
| `stickyZone` | 0.003 | Hysteresis at endpoints (prevents dithering) |
| `fullOff` | 0.0 | Dead zone at low end |
| `fullOn` | 1.0 | Dead zone at high end |

</details>

---

## More Information

This is the Arduino wrapper for the [smooth_axis](https://github.com/Viderspace/smooth_axis) C library. Visit the main repository for algorithm details, test methodology, and platform-agnostic documentation.


## Author

[Jonatan Vider](https://github.com/Viderspace) - [LinkedIn](http://www.linkedin.com/in/viderspace) -
Viderspace@gmail.com
