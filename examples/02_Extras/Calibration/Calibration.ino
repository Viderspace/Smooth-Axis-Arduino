/*
 * 02_Extras/Calibration
 * * SOLVES: "My potentiometer doesn't reach 0 or 100%"
 * * SOLVES: "My sensor is jittery at the edges"
 * * This sketch demonstrates the fineTune() function, which lets you
 * clip unstable edges (Dead Zones) and snap endpoints (Sticky Zones).
 */

#include <SmoothAxis.h>

// --- Configuration ---
const int SENSOR_PIN  = A0;
const int SENSOR_MAX  = 1023;
const float SETTLE_TIME = 0.15;

// Initialize as normal
SmoothAxis axis(SENSOR_MAX, SETTLE_TIME);

void setup() {
    Serial.begin(9600);
    while (!Serial) { delay(10); }
    
    Serial.println("Calibrating sensor edges...");
    
    // --- THE FIX ---
    // Scenario: Your old potentiometer only goes down to 15 (not 0)
    // and only up to 1010 (not 1023).
    //
    // We need to cut off the bottom 2% and the top 2%.
    //
    // Parameters:
    // 1. Sticky Zone (0.003): Magnetic snap at exact 0 and 100% (Keep default)
    // 2. Full Off   (0.020):  Input below 2% will be treated as 0.
    // 3. Full On    (0.980):  Input above 98% will be treated as Max.
    // 4. Threshold  (0.003):  Sensitivity to movement (Keep default)
    
    axis.fineTune(0.003, 0.02, 0.98, 0.003);
    
    Serial.println("Dead zones applied: Bottom 2%, Top 2%");
}

void loop() {
    // Now, even if the hardware only outputs 15, the library reports 0.
    // Even if it only outputs 1010, the library reports 1023.
    axis.update(analogRead(SENSOR_PIN));
    
    if (axis.hasChanged()) {
        Serial.print("Calibrated Value: ");
        Serial.println(axis.read());
    }
}