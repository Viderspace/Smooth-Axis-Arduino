/*
 * 02_Extras/Plotter
 * * VISUALIZATION TOOL
 * ------------------
 * This sketch demonstrates the algorithm in real-time using the
 * Arduino IDE Serial Plotter.

 * * * INSTRUCTIONS:
 * 1. Upload this sketch.
 * 2. Go to 'Tools' -> 'Serial Plotter' (or press Ctrl+Shift+L).
 * 3. Set the baud rate to 115200.
 
 * while the plotter is open to capture the legend.
 * * * LEGEND:
 * - Blue/Yellow Lines (0, 1023): Fixed bounds to stop auto-scaling.
 * - Green Line (Raw): The noisy input.
 * - Pink Line (Smoothed): The stable output.
 * - Red Line (Threshold): Visualizes the dynamic noise gate (Scaled up).
 */

#include <SmoothAxis.h>

// --- Configuration ---
const int SENSOR_PIN = A0;
const int SENSOR_MAX = 1023;
const float SETTLE_TIME = 0.50;  // Slowed down for easier viewing

// Visualization Scaling
// The noise threshold is usually small (e.g., 1-5 units).
// To see it clearly on the graph, we scale it so that 30 units of noise
// fills the entire screen height (1023).
// Note: We use 30.0 to force floating point division.
const float THRESHOLD_SCALE = ((float)SENSOR_MAX * (float)SENSOR_MAX) / 30.0;

SmoothAxis axis(SENSOR_MAX, SETTLE_TIME);

// Tracks the last valid output for plotting
int lastStableValue = 0;

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }
    delay(100);  // Short safety delay
    
    // Print column headers. The Plotter uses these as the legend.
    // This MUST be the first thing printed.
    Serial.println("Min,ThresholdScaled,Raw,Max,Smoothed");
}

void loop() {
    int rawValue = analogRead(SENSOR_PIN);
    
    // 1. Update the filter
    axis.update(rawValue);
    
    // 2. Update output only on meaningful change
    if (axis.hasChanged()) {
        lastStableValue = axis.read();
    }
    
    // --- Plotting ---
    // Print 5 comma-separated values to drive the graph.
    
    // 1. Min Bound (Locks Y-Axis)
    Serial.print(0);
    Serial.print(",");
    
    // 2. Dynamic Threshold (Scaled Up)
    // Visualizes the "Brain": jumps up when noise is detected.
    float scaledThreshold = axis.getThreshold() * THRESHOLD_SCALE;
    Serial.print(scaledThreshold);
    Serial.print(",");
    
    // 3. Raw Input
    Serial.print(rawValue);
    Serial.print(",");
    
    // 4. Max Bound (Locks Y-Axis)
    Serial.print(SENSOR_MAX);
    Serial.print(",");
    
    // 5. Smoothed Output
    Serial.println(lastStableValue);
    
    // Limit frame rate for readability (approx 33fps)
    delay(30);
}