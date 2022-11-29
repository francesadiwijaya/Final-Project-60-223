#include <NewPing.h>
#include <PololuLedStrip.h>
#include <assert.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define BUZZER_PIN   5

#define LED_COUNT 60
#define MAX_DISTANCE 200
#define MAX_BUZZ 6

#define requires(expr) assert(expr)
#define ensures(expr)  assert(expr)

PololuLedStrip<13> led_strip;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
rgb_color colors[LED_COUNT];

void fill(uint8_t r, uint8_t g, uint8_t b);

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
}

/**
 * @brief Main routine
 */
void loop() {
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
  unsigned int uS = (sonar.ping() / US_ROUNDTRIP_CM);
  fill(0, 0, 0);
  while (10 < uS && uS < 60) {
    fill(255, 0, 0);
    buzz(uS);
    uS = (sonar.ping() / US_ROUNDTRIP_CM);
  }
}

/**
 * @brief     Assigns a new rgb value to every element in the color array
 * @param[in] r Amount of red
 * @param[in] g Amount of green
 * @param[in] b Amount of blue
 */
void fill(uint8_t r, uint8_t g, uint8_t b) {
  for (uint16_t i = 0; i < LED_COUNT; i++) {
     colors[i] = rgb_color(r, g, b);
  }
  led_strip.write(colors, LED_COUNT);
}

/**
 * @brief     Activate the buzzer
 * @param[in] distance
 * @pre       `distance` is non-negative
 * @pre       `distance` is less than `MAX_DISTANCE`
 */
void buzz(unsigned long distance) {
  requires(distance < MAX_DISTANCE);
  int x = MAX_BUZZ - int_log2(distance);

  warn(x);
}

/**
 * @brief     Calculate log2 of an integer
 * @param[in] x
 * @return    log2(`x`)
 * @pre       `x` is non-negative
 */
int int_log2(int x) {
  requires(-1 < x);

  int c = 0;
  if (x == 0) return 1; 
  while ((x >>= 1)) { c++; }
  return c;
}

/**
 * @brief     Run the buzzer a number of times
 * @param[in] x The number of times
 * @pre       `x` is non-negative
 * @pre       `x` is less than or equal to 7
 */
void warn(int x) {
  requires(-1 < x);

  for (int i = 0; i < x; i++) {
    Serial.print("buzz ");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
  }
  Serial.println("");
  delay(500);
}
