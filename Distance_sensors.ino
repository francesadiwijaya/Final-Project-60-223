// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------


#include <PololuLedStrip.h>

// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<13> ledStrip;

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT 60
rgb_color colors[LED_COUNT];


#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
const int BLUELED = 4;
const int REDLED = 5;
const int WAITTIME = 1000;
int eventTimer = 0;
const int BUZZERPIN = 5;

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  pinMode(BLUELED, OUTPUT);
  pinMode(REDLED, OUTPUT);
}

void loop() {
  delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  Serial.print("Ping: ");
  float distance = sonar.ping_cm();
  for (uint16_t i = 0; i < LED_COUNT; i++)
  {
    colors[i] = rgb_color(0, 0, 0);
  }

  // Write the colors to the LED strip.
  ledStrip.write(colors, LED_COUNT);
  digitalWrite(BUZZERPIN, LOW);

  while (distance < 50) {

    // Update the colors.
    for (uint16_t i = 0; i < LED_COUNT; i++)
    {
      colors[i] = rgb_color(210, 4, 45);
    }

    // Write the colors to the LED strip.
    ledStrip.write(colors, LED_COUNT);

    distance = sonar.ping_cm();
    Serial.print("Ping: ");
    Serial.print(distance); // Send ping, get distance in cm and print result (0 = outside set distance range)
    Serial.println("cm");

    digitalWrite(BUZZERPIN, HIGH);
  }

  Serial.print(distance); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
}
