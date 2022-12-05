/**
 * header comment
 */

#include <NewPing.h>
#include <PololuLedStrip.h>
#include <assert.h>

#define BUZZER_CONTROL_PIN        3
#define CONTROL_BUTTON_PIN        4
#define BUZZER_PIN                5
#define LEFT_ECHO_PIN             11
#define LEFT_TRIGGER_PIN          12
#define RIGHT_ECHO_PIN            13
#define RIGHT_TRIGGER_PIN         14

#define LED_COUNT 60
#define MAX_DISTANCE 200
#define MAX_BUZZ 8

#define requires(expr) assert(expr)
#define ensures(expr)  assert(expr)

PololuLedStrip<13> led_strip;
NewPing sonar_left(LEFT_TRIGGER_PIN, LEFT_ECHO_PIN, MAX_DISTANCE);
NewPing sonar_right(RIGHT_TRIGGER_PIN, RIGHT_ECHO_PIN, MAX_DISTANCE);
rgb_color colors[LED_COUNT];

void fill(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Declare pin modes
 */
void setup() {
  Serial.begin(115200);
  pinMode(CONTROL_BUTTON_PIN, INPUT);
  pinMode(BUZZER_CONTROL_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

/**
 * @brief Main routine
 */
void loop() {
  delay(100);
  if (digitalRead(CONTROL_BUTTON_PIN) == HIGH) {
    unsigned int left_distance = (sonar_left.ping() / US_ROUNDTRIP_CM);
    unsigned int right_distance = (sonar_right.ping() / US_ROUNDTRIP_CM);
    while ((10 < left_distance && left_distance < 60) || (10 < right_distance && right_distance < 60)) {
      if (digitalRead(BUZZER_CONTROL_PIN) == HIGH) {
        buzz(min(left_distance, right_distance));
      }
      fill(255, 0, 0);
      left_distance  = (sonar_left.ping() / US_ROUNDTRIP_CM);
      right_distance = (sonar_right.ping() / US_ROUNDTRIP_CM);
    }
  }
  cleanup();
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

/**
 * @brief Turns the buzzer and led strip off
 */
void cleanup() {
  digitalWrite(BUZZER_PIN, LOW);
  fill(0, 0, 0);
}
