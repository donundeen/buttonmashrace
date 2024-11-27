#include <Adafruit_NeoPixel.h>

#define BUTTON_A_PIN 2      // Button A connected to digital pin 2
#define BUTTON_B_PIN 3      // Button B connected to digital pin 3
#define NEOPIXEL_PIN 6      // NeoPixel strip connected to digital pin 6
#define NUM_PIXELS 20       // Number of pixels in the strip

// Initialize NeoPixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Game state variables
int playerA_position = 0;    // Number of green lights from top
int playerB_position = 0;    // Number of blue lights from bottom
bool gameActive = false;

// Button state variables
bool buttonA_last = HIGH;
bool buttonB_last = HIGH;

// Colors
uint32_t RED = strip.Color(255, 0, 0);
uint32_t GREEN = strip.Color(0, 255, 0);
uint32_t BLUE = strip.Color(0, 0, 255);

void setup() {
  // Initialize buttons with internal pullup resistors
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
  
  strip.begin();
  resetGame();
}

void loop() {
  // Read button states (buttons are active LOW due to pullup)
  bool buttonA = !digitalRead(BUTTON_A_PIN);
  bool buttonB = !digitalRead(BUTTON_B_PIN);
  
  // Check for button press events (transition from HIGH to LOW)
  if (buttonA && !buttonA_last) {
    handlePlayerA();
  }
  if (buttonB && !buttonB_last) {
    handlePlayerB();
  }
  
  // Update button states
  buttonA_last = buttonA;
  buttonB_last = buttonB;
}

void handlePlayerA() {
  if (!gameActive) {
    gameActive = true;
  }
  
  int nextPosition = NUM_PIXELS - 1 - playerA_position;
  if (strip.getPixelColor(nextPosition) == BLUE) {
    // Convert blue to green and push back player B
    playerB_position--;
  }
  playerA_position++;
  
  updateStrip();
  checkWinner();
}

void handlePlayerB() {
  if (!gameActive) {
    gameActive = true;
  }
  
  int nextPosition = playerB_position;
  if (strip.getPixelColor(nextPosition) == GREEN) {
    // Convert green to blue and push back player A
    playerA_position--;
  }
  playerB_position++;
  
  updateStrip();
  checkWinner();
}

void updateStrip() {
  // Fill strip with initial red
  strip.fill(RED);
  
  // Set player A's green lights from top
  for (int i = NUM_PIXELS - 1; i >= NUM_PIXELS - playerA_position; i--) {
    strip.setPixelColor(i, GREEN);
  }
  
  // Set player B's blue lights from bottom
  for (int i = 0; i < playerB_position; i++) {
    strip.setPixelColor(i, BLUE);
  }
  
  strip.show();
}

void checkWinner() {
  if (playerA_position >= NUM_PIXELS) {
    // Player A wins, flash all lights green 10 times
    flashLights(GREEN);
    resetGame();
  } else if (playerB_position >= NUM_PIXELS) {
    // Player B wins, flash all lights blue 10 times
    flashLights(BLUE);
    resetGame();
  }
}

void flashLights(uint32_t color) {
  for (int i = 0; i < 10; i++) {
    strip.fill(color); // Set all lights to the winning color
    strip.show();
    delay(100); // Wait for 100 milliseconds
    strip.fill(RED); // Reset to red
    strip.show();
    delay(100); // Wait for 100 milliseconds
  }
}

void resetGame() {
  // Disable buttons
  buttonA_last = HIGH; // Simulate button not pressed
  buttonB_last = HIGH; // Simulate button not pressed

  // Turn lights red one at a time, starting from the light furthest from the winner
  int startPosition = (playerA_position >= NUM_PIXELS) ? 0 : NUM_PIXELS - 1; // Start from the furthest light
  int endPosition = (playerA_position >= NUM_PIXELS) ? NUM_PIXELS : NUM_PIXELS - playerA_position; // End position based on winner

  for (int i = startPosition; i < endPosition; i++) {
    strip.setPixelColor(i, RED); // Set the light to red
    strip.show();
    delay(200); // Delay to create a slow transition effect
  }

  // Reset player positions and game state
  playerA_position = 0;
  playerB_position = 0;
  gameActive = false;

  // Reactivate buttons
  buttonA_last = HIGH; // Reset button state
  buttonB_last = HIGH; // Reset button state

  updateStrip(); // Update the strip to show the initial state
}
