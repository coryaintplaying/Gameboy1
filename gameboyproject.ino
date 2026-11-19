//=========================================//
//==================NOTES==================//
//=========================================//
/*game boy will have
1. Snake Game  https://github.com/Enjoy-Mechatronics/OLED-Snake-Game/tree/main
2. Flappy Bird https://github.com/adidax/oled-flappy-bird-arduino
3. Racing Game https://github.com/adidax/oled-retro-pixel-racer

oled links:
https://www.youtube.com/watch?v=HVHVkKt-ldc   FINISHED
https://www.youtube.com/watch?v=vHRyj3e3ZTU   FINISHED (nothing useful)
https://www.youtube.com/watch?v=0JuH5_lv8BE
*/
//=========================================//




//=========================================//
//================LIBRARIES================//
//=========================================//
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
Adafruit_SSD1306 display(128, 64, &Wire, -1);
//=========================================//




//=========================================//
//=================SET UP==================//
//=========================================//
#define JOYSTICK_X A0 //horizontal
#define JOYSTICK_Y A1 //vertical
#define JOYSTICK_B A2 //button pin
#define ANALOG_X_CORRECTION 128 
#define ANALOG_Y_CORRECTION 128 
#define SNAKE_MAX_LENGTH 12
const int number_items = 3; //number of items
int selected_item = 0; //which item is selected
int prev_selected_item; //previous item before the selected item
int next_selected_item; //next item after the selected item
int chosen_game = -1;
int up_clicked = 1;
int down_clicked = 1;
int snake_x[SNAKE_MAX_LENGTH];
int snake_y[SNAKE_MAX_LENGTH];
int snake_length = 2;
int score = 0;
int snake_dir = 0;  // 0 = right, 1 = up, 2 = left, 3 = down
int food_x;
int food_y;
int xVal = analogRead(JOYSTICK_X);
int yVal = analogRead(JOYSTICK_Y);
bool bVal = digitalRead(JOYSTICK_B); 

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(JOYSTICK_B, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setFont(&FreeMonoBold12pt7b);

  display.setCursor(30,37); //30,37?
  display.print("gameboy")

  display.display();

  delay(2000);

  //randomSeed(analogRead(0));
  food_x = random(0, display.width() / 8);
  food_y = random(0, display.height() / 8);

  snake_x[0] = 0;
  snake_y[0] = 0;
  snake_x[1] = 1;
  snake_y[1] = 0;

  //drawSnake();
  //drawFood();
}
//=========================================//




//=========================================//
//================ICONS====================//
//=========================================//
//===============================//
//============MENU===============//
//===============================//
// 'bird icon', 14x13px
const unsigned char bird_icon [] PROGMEM = {
	0x3e, 0x00, 0x41, 0x00, 0x91, 0x00, 0xc0, 0x80, 0x40, 0x40, 0x40, 0x40, 0x90, 0x20, 0x90, 0x20, 
	0x88, 0x10, 0x44, 0x08, 0x32, 0x7c, 0x0d, 0x80, 0x1b, 0x00
};
// 'car icon', 14x12px
const unsigned char car_icon [] PROGMEM = {
	0x03, 0xc0, 0x04, 0x20, 0x08, 0x10, 0x08, 0x10, 0x73, 0xf8, 0xa0, 0x04, 0xe0, 0x04, 0x80, 0x04, 
	0xb7, 0x68, 0x48, 0x90, 0x48, 0x90, 0x30, 0x60
};
// 'snake icon', 14x14px
const unsigned char snake_icon [] PROGMEM = {
	0x71, 0xc0, 0x8a, 0x30, 0xda, 0x18, 0x8b, 0x8c, 0x88, 0xc4, 0x88, 0x64, 0xcc, 0x24, 0x44, 0x24, 
	0x44, 0x24, 0x44, 0x64, 0x47, 0xc4, 0x40, 0x04, 0x40, 0x04, 0x7f, 0xf8
};
// 'scrollbar', 8x64px
const unsigned char scrollbar [] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff
};
// 'scrolloutline', 120x21px
const unsigned char scrolloutline [] PROGMEM = {
	0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x40, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0
};
//===============================//


//===============================//
//========GAME2:FlappyBird=======//
//===============================//

//===============================//


//===============================//
//========GAME3:RacingGame=======//
//===============================//

//===============================//
//=========================================//




//=========================================//
//================MENU=====================//
//=========================================//
const unsigned char* bitmap_icons[3] = {
	bird_icon,
	car_icon,
	snake_icon,
};

char menu_items[number_items] [20] = {
	{"Bird"},
	{"Car"},
	{"Snake"}
};

// Define a structure to hold the icon configuration
struct Icon {
  const unsigned char* bitmap;
  uint8_t width;
  uint8_t height;
};

// Create an array mapping each icon to its custom dimensions
Icon menuIcons[] = {
  { bird_icon,  14, 13 }, // Index 0: Bird
  { car_icon,   14, 12 }, // Index 1: Car
  { snake_icon, 14, 14 }  // Index 2: Snake
};


void draw(int itemIndex1, uint8_t iconYcood, uint8_t textYcood) {
  u8g.drawStr(24, textYcood, menu_items[itemIndex1]);

  uint8_t byteWidth = (menuIcons[itemIndex1].width + 7) / 8;
  u8g.drawBitmapP(4, iconYcood, byteWidth, menuIcons[itemIndex1].height, 
  menuIcons[itemIndex1].bitmap);
}
//=========================================//




void loop() {
  if (chosen_game==-1) {
  //=============================//
  //=============MENU============//
  //=============================//
    prev_selected_item = selected_item - 1;
    if (prev_selected_item<0) {prev_selected_item=number_items-1;}
    next_selected_item = selected_item + 1;
    if (next_selected_item>=number_items) {next_selected_item=0;}

    if ((yVal > 850) && (down_clicked==1)) {  // Down
      selected_item-=1;
      if (selected_item<0) {selected_item = number_items-1;}
      down_clicked=0;
    } else if ((yVal < 150) && (up_clicked==1)) { // UP
      selected_item+=1;
      if (selected_item>=number_items) {selected_item=0;}
      up_clicked=0;
    } 
    
    if ((yVal < 850) && (down_clicked==0)) {down_clicked=1;} //reset Down
    if ((yVal > 150) && (up_clicked==0)) {up_clicked=1;} //reset UP 
    
    if (!bVal) {
      chosen_game=selected_item;
      Serial.print(bVal);
      Serial.println(menu_items[chosen_game]);
      delay(200);
    }

    u8g.firstPage();
    do {
      // previous item
      display.setFont(&FreeMono9pt7b);
      draw(prev_selected_item, 4, 16);

      //selected item
      display.setFont(&FreeMonoBold9pt7b);
      draw(selected_item, 26, 37);

      //next item    
      display.setFont(&FreeMono9pt7b);
      draw(next_selected_item, 47, 58);

      u8g.drawBitmapP(0, 22, 120/8, 21, scrolloutline);
      u8g.drawBitmapP(125, 0, 8/8, 64, scrollbar);
    
      display.display();
      delay(50);
  //=============================//
  
  
  } else if (chosen_game==1) {
  //=============================//
  //========GAME1:SnakeGame======//
  //=============================//
    delay(80);

    if (xVal > 850 && snake_dir != 2) {  // Right
      snake_dir = 0;
    } else if (xVal < 150 && snake_dir != 0) {  // Left
      snake_dir = 2;
    } else if (yVal > 850 && snake_dir != 1) {  // Down
      snake_dir = 3;
    } else if (yVal < 150 && snake_dir != 3) {  // UP
      snake_dir = 1;
    }

    for (int i = snake_length - 1; i > 0; i--) {
      snake_x[i] = snake_x[i - 1];
      snake_y[i] = snake_y[i - 1];
    }

    switch (snake_dir) {
      case 0:
        snake_x[0]++;
        break;
      case 1:
        snake_y[0]--;
        break;
      case 2:
        snake_x[0]--;
        break;
      case 3:
        snake_y[0]++;
        break;
    }

    if (snake_x[0] < 0) {
      snake_x[0] = display.width() / 8 - 1;
    } else if (snake_x[0] >= display.width() / 8) {
      snake_x[0] = 0;
    }

    if (snake_y[0] < 0) {
      snake_y[0] = display.height() / 8 - 1;
    } else if (snake_y[0] >= display.height() / 8) {
      snake_y[0] = 0;
    }

    //Eat Apple
    if (snake_x[0] == food_x && snake_y[0] == food_y) {
      if (snake_length < SNAKE_MAX_LENGTH) {
        snake_length++;
      }
      score = score + 1;

      food_x = random(0, display.width() / 8);
      food_y = random(0, display.height() / 8);
    }

    for (int i = 1; i < snake_length; i++) {
      if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) {
        gameOver();
        Reset();
      }
    }

    display.clearDisplay();
    drawSnake();
    drawFood();
    display.display();
  //=============================//


  } else if (chosen_game==2) {
  //=============================//
  //========GAME2:FlappyBird=====//
  //=============================//

  //=============================//


  } else if (chosen_game==3) {
  //=============================//
  //========GAME3:RacingGame=====//
  //=============================//

  //=============================//
  }
}




//=========================================//
//============GAME1:SnakeGame==============//
//=========================================//
void drawSnake() {
  for (int i = 0; i < snake_length; i++) {
    display.fillRect(snake_x[i] * 8, snake_y[i] * 8, 8, 8, WHITE);
  }
}

void drawFood() {
  display.fillRect(food_x * 8, food_y * 8, 8, 8, WHITE);
}

void gameOver() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 20);
  display.println("Game Over!");
  display.setCursor(15, 40);
  display.print("Score: ");
  display.println(score);
  display.display();
  delay(3000);
}

void Reset() {
  score = 0;
  food_x = random(0, display.width() / 8);
  food_y = random(0, display.height() / 8);
  snake_length = 2;
  snake_dir = 0;  // 0 = right, 1 = up, 2 = left, 3 = down
  snake_x[0] = 0;
  snake_y[0] = 0;
  snake_x[1] = 1;
  snake_y[1] = 0;
}
//=========================================//




//=========================================//
//============GAME2:FlappyBird=============//
//=========================================//
/*
 Flappy Bird for Arduino with OLED Display 128x64 – Comments in English
 https://www.makerblog.at

 - Arduino UNO R3 (or comparable)
 - OLED I2C Display 128x64 with SSD1306
 - Push Button

 Wiring:
 
 OLED -> Arduino UNO R3
 SDA -> A4
 SCL -> A5
 GND -> GND
 VIN -> 5V

 Push Button: GND - Push Button - D2

 Required libraries (install via Library Manager if needed):

 - Adafruit SSD1306 including Adafruit GFX

  Optimization:
 -------------
 To avoid floating point numbers (float), all relevant positions 
 and velocities are multiplied by SCALE_FACTOR = 10 and stored as int. 
 This improves performance and avoids rounding errors.

 Scaled values:
 - Coordinates are stored *10.
 - For display output, these values are divided by SCALE_FACTOR = 10.

 Example:
 - A pipe x = 350 is actually at x = 35 pixels on the display.
 
 This scaling is used in all calculations, but 
 when drawing (drawBitmap, drawFastHLine etc.) the true pixel value 
 is restored (x / SCALE_FACTOR).

#include <avr/pgmspace.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display settings
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int OLED_RESET = -1;
const int SCREEN_ADDRESS = 0x3C; // I2C address of the display, see datasheet
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const bool DEBUG_PRINT = false;  // set to true for debug output in Serial Monitor

// Button & physics
const int BUTTON_PIN = 2; // push button on digital pin 2
const int SCALE_FACTOR = 10;  // scale factor 10 for more precise movement
const int GRAVITY = 5; // gravity applied to the character each frame
const int JUMP_STRENGTH = -25; // jump acceleration when push button is pressed
const int PIPE_WIDTH = 15 * SCALE_FACTOR; // pipes are 15px wide on display
const int PIPE_GAP = 30 * SCALE_FACTOR; // gap in pipes 30px high
const int PIPE_SPEED = 10; // side scroll speed
const int PIPE_SPACING = 65 * SCALE_FACTOR;  // default spacing between 2 pipes

// Size of player sprite
const int BIRD_WIDTH_PX = 10;
const int BIRD_HEIGHT_PX = 8;

// Game variables
int birdY; // vertical position of the player
float velocity = 0; // combined vertical velocity of the player
bool gameOver = false;
int score = 0;
unsigned long frameCounter = 0;
unsigned long gameOverTime = 0;

// Game state
enum GameState { STARTSCREEN,
                 HIGHSCORE,
                 PLAYING };
GameState gameState = STARTSCREEN;

// Data type for a pipe (x-position, height of the upper part)
struct Pipe {
  int x;
  int height;
};

// Maximum 3 pipes simultaneously
const int MAX_PIPES = 3;
Pipe pipes[MAX_PIPES];

// Highscore
long highscores[5] = {};
int newHighscoreIndex = -1;
unsigned long highscoreTime = 0;
bool showBlink = true;
unsigned long lastBlinkTime = 0;

// Framerate control
const int FRAME_TIME = 40;  // 40ms per frame = 25 FPS
unsigned long lastFrameTime = 0;

// Graphics for player sprite
// converted from PNG with https://javl.github.io/image2cpp/

// 'bird1', 10x8px
const unsigned char epd_bitmap_bird1[] PROGMEM = {
  0x0e, 0x00, 0x15, 0x00, 0x60, 0x80, 0x81, 0x80, 0xfc, 0xc0, 0xfb, 0x80, 0x71, 0x00, 0x1e, 0x00
};
// 'bird2', 10x8px
const unsigned char epd_bitmap_bird2[] PROGMEM = {
  0x0e, 0x00, 0x15, 0x00, 0x70, 0x80, 0xf9, 0x80, 0xfc, 0xc0, 0x83, 0x80, 0x71, 0x00, 0x1e, 0x00
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 64)
const int epd_bitmap_allArray_LEN = 2;
const unsigned char* epd_bitmap_allArray[2] = {
  epd_bitmap_bird1,
  epd_bitmap_bird2
};

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  randomSeed(analogRead(0));

  // initialize display or pause on error
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1) {
      delay(100);
    }
  }

  gameState = STARTSCREEN;  
}

void loop() {
  unsigned long currentTime = millis();

  // ensure that the next frame is only calculated after 40ms
  if (currentTime - lastFrameTime < FRAME_TIME) return;
  lastFrameTime = currentTime;
  frameCounter++;

  switch (gameState) {
    case STARTSCREEN:
      renderStartScreen();
      if (anyKeyPressed()) {
        resetGame(); // reset variables
        gameState = PLAYING; // start game
      }
      break;

    case PLAYING:
      if (gameOver) {
        // after game over, automatically switch to highscore screen after 3 seconds
        if (millis() - gameOverTime > 3000) {  
          gameState = HIGHSCORE;
          highscoreTime = millis();
        }
        // or player presses button (1 sec pause), then start new game immediately
        if ((millis() - gameOverTime > 1000) && anyKeyPressed()) {
          resetGame();
          gameState = PLAYING;
        }
      } else {
        // if button pressed, add negative acceleration (upwards)
        if (digitalRead(BUTTON_PIN) == LOW) velocity = JUMP_STRENGTH;
        velocity += GRAVITY; // apply gravity
        velocity = constrain(velocity, -50, 50); // limit velocity to prevent too fast falling or rising

        // calculate player position
        birdY += velocity;
        // if player at top (y=0), do not go off screen
        if (birdY < 0) birdY = 0;

        if (DEBUG_PRINT) {
          Serial.print("Vel: ");
          Serial.println(velocity);
        }

        movePipes();
        checkCollision();
      }
      drawGame();
      break;

    case HIGHSCORE:
      renderHighscoreScreen();
      // after 8 seconds without button press, switch to start screen
      if (millis() - gameOverTime > 8000) {
        gameState = STARTSCREEN;
      }
      // or start new game if button pressed
      if (anyKeyPressed()) {
        resetGame();
        gameState = PLAYING;
      }
      break;
  }
}

// PULLUP logic: LOW means pressed
bool anyKeyPressed() {
  return digitalRead(BUTTON_PIN) == LOW;
}

// ---- Reset game ---- //
void resetGame() {
  // set start position of the player to 1/5 of the screen height
  birdY = SCREEN_HEIGHT * SCALE_FACTOR / 5;
  velocity = 0;
  score = 0;
  gameOver = false;

  // create first pipe just outside the screen edge
  pipes[0].x = SCREEN_WIDTH * SCALE_FACTOR;

  pipes[0].height = generatePipeHeight();
  // create pipes 2 and 3 with random variation in spacing
  for (int i = 1; i < MAX_PIPES; i++) {
    pipes[i].x = SCREEN_WIDTH * SCALE_FACTOR + i * PIPE_SPACING + random(-20, 20) * SCALE_FACTOR;
    pipes[i].height = generatePipeHeight();
  }
}

// generate random height of the upper pipe section,
// leaving at least space for gap and 10px for lower part
int generatePipeHeight() {
  return random(10 * SCALE_FACTOR, SCREEN_HEIGHT * SCALE_FACTOR - PIPE_GAP - 10 * SCALE_FACTOR);
}

// move pipes toward the player
void movePipes() {

  int currentSpeed = PIPE_SPEED + score; // pipes move faster and faster

  for (int i = 0; i < MAX_PIPES; i++) {
    // move pipe position left by currentSpeed
    pipes[i].x -= currentSpeed;

    // if pipe leaves the left edge, immediately create a new pipe at the right edge
    if (pipes[i].x < -PIPE_WIDTH) {
      pipes[i].x = findFurthestPipe() + PIPE_SPACING + random(-20, 20) * SCALE_FACTOR;  // place new pipe far right
      pipes[i].height = random(10 * SCALE_FACTOR, SCREEN_HEIGHT * SCALE_FACTOR - PIPE_GAP - 10 * SCALE_FACTOR);
      score++;
    }
  }
}

// helper function to find the pipe farthest to the right
int findFurthestPipe() {
  int maxX = 0;
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i].x > maxX) {
      maxX = pipes[i].x;
    }
  }
  return maxX;
}

// ---- Collision detection ---- //
void checkCollision() {

  // collision check with a 2px smaller hitbox (Collision Forgiveness),
  // because the player is more round → avoids unfair edge hits
  for (int i = 0; i < MAX_PIPES; i++) {
    // check if the player overlaps horizontally with a pipe 
    if (pipes[i].x < (15 + 2) * SCALE_FACTOR && pipes[i].x + PIPE_WIDTH > (15 - 2) * SCALE_FACTOR) {
      // check if the player hits the pipe vertically (top or bottom)
      if ((birdY - 2 * SCALE_FACTOR) < pipes[i].height || (birdY + 2 * SCALE_FACTOR) > pipes[i].height + PIPE_GAP) {
        gameOver = true;
        gameOverTime = millis();
        saveHighscores(score);
      }
    }
  }

  // if player below the screen bottom
  if (birdY >= SCREEN_HEIGHT * SCALE_FACTOR) {
    gameOver = true;
    gameOverTime = millis();
    saveHighscores(score);
  }
}

// ---- Draw game ---- //
void drawGame() {
  display.clearDisplay();

  drawBird();
  drawPipes();
  drawScore();

  if (gameOver) displayGameOver();  // if game over, display overlay

  display.display();
}

void drawBird() {
  int birdFrame = (frameCounter / 5) % 2;
  display.drawBitmap(15 - BIRD_WIDTH_PX / 2, birdY / SCALE_FACTOR - BIRD_HEIGHT_PX / 2,
                     epd_bitmap_allArray[birdFrame], BIRD_WIDTH_PX, BIRD_HEIGHT_PX, SSD1306_WHITE);
}

void drawPipes() {
  for (int pipeIndex = 0; pipeIndex < MAX_PIPES; pipeIndex++) {
    int pipeX = pipes[pipeIndex].x / SCALE_FACTOR;
    int upperPipeHeight = pipes[pipeIndex].height / SCALE_FACTOR;
    int gapHeight = PIPE_GAP / SCALE_FACTOR;
    int lowerPipeY = upperPipeHeight + gapHeight;
    int lowerPipeHeight = SCREEN_HEIGHT - lowerPipeY;    
    int pipeWidth = PIPE_WIDTH / SCALE_FACTOR;

    // upper pipe segment (vertical)
    display.fillRect(pipeX, 0, pipeWidth, upperPipeHeight - 6, SSD1306_WHITE);
    // pipe head top (horizontal cap)
    display.fillRect(pipeX - 2, upperPipeHeight - 6, pipeWidth + 4, 6, SSD1306_WHITE);
    // pipe head bottom (horizontal cap below gap)
    display.fillRect(pipeX - 2, lowerPipeY, pipeWidth + 4, 6, SSD1306_WHITE);
    // lower pipe segment (vertical)
    display.fillRect(pipeX, lowerPipeY + 6, pipeWidth, lowerPipeHeight - 6, SSD1306_WHITE);
  }
}

// show current score at top right of display
void drawScore() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(110, 2);
  display.print(score);
}

// ---- Show start screen ---- //
void renderStartScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("Ardu Bird");
  display.setTextSize(1);
  display.setCursor(20, 40);
  display.println("Press to start");
  display.display();
}

// ---- Show Game Over ---- //
void displayGameOver() {
  display.fillRect(20, 5, SCREEN_WIDTH - 40, SCREEN_HEIGHT - 10, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setTextSize(1);
  display.setCursor(38, 10);
  display.println("GAME OVER");
  display.setCursor(35, 20);
  display.println("Your score");

  int16_t x1, y1;
  uint16_t textWidth, textHeight;
  char scoreBuffer[10];               // buffer for number as string
  sprintf(scoreBuffer, "%d", score);  // convert number to string
  display.setTextSize(2);
  display.getTextBounds(scoreBuffer, 0, 0, &x1, &y1, &textWidth, &textHeight);  // calculate text width for centering
  display.setCursor(64 - textWidth / 2, 36);
  display.println(scoreBuffer);
}

// ---- Show Highscore list ---- //
void renderHighscoreScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(25, 2);
  display.println("HIGHSCORES");
  display.setTextSize(1);

  // current achieved score should blink if in Top10, blink status changes every 100ms
  if (millis() - lastBlinkTime > 100) {
    showBlink = !showBlink;
    lastBlinkTime = millis();
  }

  for (int i = 0; i < 5; i++) {
    if (i == newHighscoreIndex && !showBlink) {
      continue;
    }
    display.setCursor(30, 14 + (i * 9));
    display.print(i + 1);
    display.print(".    ");
    display.print(highscores[i]);
  }
  display.display();
}

// ---- Save highscores ---- //
void saveHighscores(int newScore) {
  newHighscoreIndex = -1;
  for (int i = 0; i < 5; i++) {
    if (newScore > highscores[i]) {
      for (int j = 4; j > i; j--) {
        highscores[j] = highscores[j - 1];
      }
      highscores[i] = newScore;
      newHighscoreIndex = i;
      break;
    }
  }
}
*/ 
//=========================================//


//=========================================//
//============GAME3:RacingGame=============//
//=========================================//
/*
 Retro Racer with OLED Display 128x64 - Comments in English
 https://www.makerblog.at

 - Arduino UNO R3 (or compatible)
 - OLED I2C Display 128x64 with SSD1306

 Wiring:
 
 OLED -> Arduino UNO R3
 SDA -> A4
 SCL -> A5
 GND -> GND
 VIN -> 5V

 Push Button LEFT   GND - Push Button - D2
 Push Button RIGHT  GND - Push Button - D3

 Buzzer             GND - Buzzer - D6

 Required Libraries:
 The following libraries must be installed. If an error occurs, check the Library Manager:
	
 - Adafruit SSD1306 (including Adafruit GFX)

  Optimization::
 -------------
 To avoid floating point numbers (float), all relevant4 positions and speeds are scaled by a factor of 10 and stored as integers.
 This improves performance and prevents rounding errors.

 Scaled Values:
 - All x and y coordinates are stored multiplied by 10.
 - All speeds are stored multiplied by 10.
 - For display rendering, these values are divided by 10.

 Example:
 - A car with x = 150 is actually at 15.0 pixels on the display.
 - A speed of playerSpeed = 25 means a movement of 2.5 px per frame.

 This scaling is used in all calculations but is converted back to the actual pixel value when displayed
 (using functions like drawBitmap, drawFastHLine, etc.) by dividing x / 10.

#include <avr/pgmspace.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Settings for the OLED display with SSD1306 controller
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int OLED_RESET = -1;
const int SCREEN_ADDRESS = 0x3C;  // I2C address of the display, see datasheet
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const bool DEBUG_PRINT = false;  // aset to true for debug output in the serial monitor

// Player and lanes
const int PLAYER_X = 10;      // X position of the player's car
const int PLAYER_WIDTH = 10;  // Width of the player sprite in pixels
const int PLAYER_HEIGHT = 8;  // Height of the cars in pixels

const int LANES = 4;                              // Number of lanes
const int LANE_HEIGHT = (SCREEN_HEIGHT / LANES);  // Height of a lane
const int OBSTACLE_WIDTH = 10;                    // Width of obstacles in pixels
const int OBSTACLE_HEIGHT = 8;                    // Height of obstacles in pixels
const int MAX_OBSTACLES = 10;                     // Maximum number of possible opponent cars

const int BUTTON_LEFT = 2;   // Push button LEFT connected to digital pin 2
const int BUTTON_RIGHT = 3;  // Push button RIGHT connected to digital pin 3
const int PIN_BUZZER = 6;    // Buzzer connected to digital pin 6

const int FRAME_TIME = 40;  // Target frame rate: 40 ms per frame = 25 FPS
long frameCounter = 0;
unsigned long lastFrameTime = 0;

const int GAME_OVER_DELAY = 1000;  // 1 second delay after game over
unsigned long gameOverTime = 0;    // Stores the timestamp of game over

// Enumeration for possible game states
enum GameState { STARTSCREEN,
                 HIGHSCORE,
                 PLAYING };
GameState gameState = STARTSCREEN;

// Note: X coordinates, speeds, and traveled distances are scaled by a factor of 10.
// Calculations are done using integers to avoid floating point numbers.
// Before displaying on the screen, values are divided by 10.

// Data type for opponent properties
struct Obstacle {
  int x;
  int lane;
  int cartype;
  bool active;
};

// High score list
long highscores[10] = {};
int newHighscoreIndex = -1;
unsigned long highscoreTime = 0;
unsigned long lastBlinkTime = 0;
bool showBlink = true;

// Intro Screen Animation
const int BITMAP_SPEED = 5;  // Pixels per frame
int introBitmapX1 = 12;      // Image 1 starts on-screen on left side
int introBitmapX2 = -52;     // Image 2 starts off-screen (left)
unsigned long lastBitmapMove = 0;

// Properties for player, opponents, and lanes
Obstacle obstacles[MAX_OBSTACLES];  // Array for opponents
int playerLane = 2;                 // Player starts in lane 2
bool gameOver = false;
int spawnProbability = 5;                  // Initial spawn probability for new opponents (1-100)
int maxActiveObstacles = 5;                // Start with 5 opponents
int dashedLineOffset = 0;                  // Offset for moving dashed lines
long distanceCounter = 0;                  // Total distance traveled in pixels
int playerSpeed = 15;                      // Player speed (px/frame) * factor 10
int laneSpeeds[LANES] = { 13, 10, 8, 5 };  // Opponent speeds (px/frame) * factor 10
int laneCooldown[LANES] = { 0 };           // Counter for cooldown per lane
const int MIN_FRAMES_BETWEEN_SPAWNS = 20;  // Minimum waiting time per lane before spawning a new opponent

const int DEBOUNCE_FRAMES = 3;  // Number of frames in which no new button input is accepted
int debounceCounter = 0;

int lastBuzzerFreq = 0;  // Last used frequency for the buzzer


// Graphics for intro, player, and opponents
// Converted from PNG using https://javl.github.io/image2cpp/

const unsigned char intro_bitmap_[] PROGMEM = {
  // 'player_car, 40x24px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x00,
  0x30, 0x03, 0x00, 0x00, 0x01, 0xe0, 0x01, 0x80, 0x00, 0x02, 0x20, 0x01, 0xff, 0x00, 0x04, 0x20,
  0x01, 0xc0, 0x80, 0x08, 0x20, 0x01, 0xc0, 0xf0, 0x33, 0x30, 0x03, 0xc0, 0x08, 0x23, 0x1f, 0xfe,
  0xc1, 0xcc, 0x23, 0x0c, 0xfc, 0xc1, 0xe4, 0x23, 0x1c, 0xfe, 0x40, 0x04, 0x23, 0x3c, 0xff, 0x03,
  0x84, 0x22, 0x3c, 0xff, 0x03, 0xc4, 0x20, 0x00, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x00, 0x0c,
  0x21, 0xe0, 0x00, 0x1e, 0x08, 0x33, 0x30, 0x00, 0x33, 0x10, 0x3e, 0x10, 0x00, 0x21, 0x30, 0x1e,
  0x1f, 0xff, 0xe1, 0xe0, 0x03, 0x30, 0x00, 0x33, 0x00, 0x01, 0xe0, 0x00, 0x1e, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char player_car_[] PROGMEM = {
  // 'player_car, 12x19px
  0x33, 0x00, 0x7f, 0x80, 0xc4, 0xc0, 0x56, 0x40, 0x56, 0x40, 0xc4, 0xc0, 0x7f, 0x80, 0x33, 0x00
};

// 'car_8x10_1', 10x8px
const unsigned char car_car_8x10_1[] PROGMEM = {
  0x63, 0x00, 0xff, 0xc0, 0x82, 0x40, 0x83, 0x40, 0x83, 0x40, 0x82, 0x40, 0xff, 0xc0, 0x63, 0x00
};
// 'car_8x10_2', 10x8px
const unsigned char car_car_8x10_2[] PROGMEM = {
  0x63, 0x00, 0xf7, 0x80, 0x9e, 0x40, 0x83, 0x40, 0x83, 0x40, 0x9e, 0x40, 0xf7, 0x80, 0x63, 0x00
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 64)
const int car_allArray_LEN = 2;
const unsigned char* car_allArray[2] = {
  car_car_8x10_1,
  car_car_8x10_2
};

// Setup and Game Loop

void setup() {
  Serial.begin(9600);

  // Initialize display or halt on error
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1) {
      delay(100);
    }
  }

  // Enable internal PULLUP resistors for push buttons
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);

  pinMode(PIN_BUZZER, OUTPUT);

  randomSeed(analogRead(0));

  // Reset game data and set status to STARTSCREEN
  resetGame();
  gameState = STARTSCREEN;
}

void loop() {
  unsigned long currentTime = millis();

  // If FRAME_TIME has passed, process the next frame
  if (currentTime - lastFrameTime >= FRAME_TIME) {

    // Output frame time if needed:
    if (DEBUG_PRINT) {
      Serial.print("FR: ");
      Serial.println(currentTime - lastFrameTime);
    }

    lastFrameTime = currentTime;

    switch (gameState) {
      case STARTSCREEN:
        renderStartScreen();
        if (anyKeyPressed()) {
          resetGame();
          gameState = PLAYING;
        }
        break;

      case PLAYING:
        // If gameOver, change state based on elapsed time
        if (gameOver) {
          if (currentTime - gameOverTime > 3000) {  // 3 seconds after Game Over, switch to Highscore screen
            gameState = HIGHSCORE;
            highscoreTime = millis();
          }
          // Ignore button presses for 1 second, then allow restarting the game
          if ((currentTime - gameOverTime > 1000) && anyKeyPressed()) {
            resetGame();
            gameState = PLAYING;
          }
        } else {
          handleInput();
          updateGameLogic();
        }
        renderGame();
        break;

      case HIGHSCORE:
        renderHighscoreScreen();
        if (currentTime - gameOverTime > 8000) {  // After 3s Game Over + 5s on Highscore screen, return to Startscreen
          gameState = STARTSCREEN;
        }
        if (anyKeyPressed()) {
          resetGame();
          gameState = PLAYING;
        }
        break;
    }
  }
}

// ---- Game logic ------ //

// PULLUP logic: Button pressed is LOW
bool anyKeyPressed() {
  return digitalRead(BUTTON_LEFT) == LOW || digitalRead(BUTTON_RIGHT) == LOW;
}

void handleInput() {
  if (debounceCounter > 0) {
    debounceCounter--;
    return;  // no input during debounce frames
  }

  // Change lane to left if possible
  if (digitalRead(BUTTON_LEFT) == LOW && playerLane > 0) {
    playerLane--;
    debounceCounter = DEBOUNCE_FRAMES;
  }

  if (digitalRead(BUTTON_RIGHT) == LOW && playerLane < LANES - 1) {
    playerLane++;
    debounceCounter = DEBOUNCE_FRAMES;
  }
}

// ---- Game logic in currewnt frame ----- //
void updateGameLogic() {
  updateObstacles();
  updateBuzzer();
  spawnNewObstacle();
  checkCollision();
  updateDashedLines();

  // More speed every 20 frames
  frameCounter++;
  if (frameCounter % 20 == 0) {  // Alle 20 Frames
    playerSpeed++;
  }

  // Cooldown per lane, should prevent 2 cars being generated too close
  for (int i = 0; i < LANES; i++) {
    if (laneCooldown[i] > 0) {
      laneCooldown[i]--;
    }
  }

  // 1 enemy when starting, raising to 4 quite fast, then raising up to 10 every 500 pixel of player movement
  if (distanceCounter < 500) {
    maxActiveObstacles = 1;
  } else if (distanceCounter < 1000) {
    maxActiveObstacles = 2;
  } else if (distanceCounter < 2000) {
    maxActiveObstacles = 3;
  } else if (distanceCounter < 3000) {
    maxActiveObstacles = 4;
  } else  
    if (distanceCounter / 5000 + 5 <= 10) {
      maxActiveObstacles = distanceCounter / 5000 + 5;
    }
}

// ---- Collision check ----- //
void checkCollision() {
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    // collisionMargin makes collision area more graceful
    int collisionMargin = 2 * 10;  // Scaling *10!
    if ((obstacles[i].active && obstacles[i].x <= (PLAYER_X * 10 + PLAYER_WIDTH * 10 - collisionMargin) && (obstacles[i].x + OBSTACLE_WIDTH * 10) >= (PLAYER_X * 10 + collisionMargin) && obstacles[i].lane == playerLane)) {
      gameOver = true;
      gameOverTime = millis();
      saveHighscores(distanceCounter);
      noTone(PIN_BUZZER);
    }
  }

  // When no collision in this frame, add to score
  if (!gameOver) {
    distanceCounter += playerSpeed;
  }
}

// ---- Reset game parameters for next round ----- //
void resetGame() {
  gameOver = false;
  playerLane = 2;        
  playerSpeed = 15;      
  spawnProbability = 5;  
  frameCounter = 0;
  dashedLineOffset = 0;
  distanceCounter = 0;
  maxActiveObstacles = 5;  
  for (int i = 0; i < LANES; i++) {
    laneCooldown[i] = 0; 
  }
  resetObstacles();
}


// ---- Move obstacles and reset if necessary ----- //
// Obstacles move in the same direction as the player but at a lower speed
// This means the player approaches the obstacles based on the speed difference
// Each lane has different speeds, stored in laneSpeeds[]
void updateObstacles() {
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (obstacles[i].active) {
      // Calculate the new position of the obstacle relative to the player based on the speed difference
      obstacles[i].x = obstacles[i].x - playerSpeed + laneSpeeds[obstacles[i].lane];
      // If the obstacle moves out of the display area, deactivate it (scale factor 10!)
      if (obstacles[i].x < -OBSTACLE_WIDTH * 10) {
        obstacles[i].active = false;
      }
    }
  }
}

// ---- Random generation of new obstacles ----- //
void spawnNewObstacle() {
  int activeObstacles = 0;
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (obstacles[i].active) {
      activeObstacles++;
    }
  }

  if (activeObstacles >= maxActiveObstacles) {
    return;  // Do not spawn new obstacles if the limit is reached
  }

  int selectedLane = random(0, LANES);

  if (laneCooldown[selectedLane] > 0) {
    return;  // Do not spawn on this lane if cooldown is still active
  }

  // Find a free slot in the array for a new obstacle
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (!obstacles[i].active) {                     
      if (random(100) < spawnProbability) {         
        obstacles[i].x = (SCREEN_WIDTH + 10) * 10;  
        obstacles[i].lane = selectedLane;           
        obstacles[i].cartype = random(2);           
        obstacles[i].active = true;

        laneCooldown[selectedLane] = MIN_FRAMES_BETWEEN_SPAWNS;  // Cooldown
        return;
      }
    }
  }
}

// ---- Initialize all obstacles ------ //
void resetObstacles() {
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    obstacles[i].x = 0; // X-Position = Position entlang der Fahrspur
    obstacles[i].lane = 0; // zugewiesene Fahrspur
    obstacles[i].cartype = 0; // 2 verschiedene Sprites
    obstacles[i].active = false; // gerade aktiv oder nicht?
  }
}

// ---- Render the game field in the current frame ------ //
void renderGame() {
  display.clearDisplay(); 
  drawDashedLines(); 
  drawObstacles(); 
  drawPlayer(); 
  if (gameOver) drawGameOver(); 
  drawDistanceCounter(); 
  display.display(); 
}

// ---- Move dashed lines backward based on player speed ------ //
void updateDashedLines() {
  dashedLineOffset -= playerSpeed;
  if (dashedLineOffset < -100) {
    dashedLineOffset = 0;
  }
}

// ---- Startscreen animation ------ //
void renderStartScreen() {
  display.clearDisplay();
  display.setRotation(1);

  // Calculate positions for two bitmaps to create an endless driving effect 
  if (millis() - lastBitmapMove > 50) {        
    introBitmapX1 += BITMAP_SPEED;             
    if (introBitmapX1 > SCREEN_HEIGHT + 12) {  
      introBitmapX1 = -52;
    }
    introBitmapX2 += BITMAP_SPEED;             
    if (introBitmapX2 > SCREEN_HEIGHT + 12) {  
      introBitmapX2 = -52;
    }
    lastBitmapMove = millis();
  }

  display.drawBitmap(introBitmapX1, 12, intro_bitmap_, 40, 24, SSD1306_WHITE);
  display.drawBitmap(introBitmapX2, 12, intro_bitmap_, 40, 24, SSD1306_WHITE);

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(4, 46);
  display.println("Pixel");
  display.setCursor(3, 66);
  display.println("Racer");
  display.setTextSize(1);

  display.setCursor(8, 96);
  display.println("Click to");
  display.setCursor(3, 106);
  display.println("start game");
  display.setRotation(0);
  display.display();
}

// ---- Display the top 10 high scores ------ //
void renderHighscoreScreen() {
  display.clearDisplay();
  display.setRotation(1); // Rotate text output by 90 degrees due to breadboard mounting
  display.setCursor(2, 6);
  display.drawFastHLine(2, 16, 60, SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println("HIGHSCORES");

  // The latest score should blink if in the top 10, switching state every 100m
  if (millis() - lastBlinkTime > 100) {
    showBlink = !showBlink;
    lastBlinkTime = millis();
  }

  for (int i = 0; i < 10; i++) {
    display.setCursor(10, 20 + (i * 10));
    display.print(i + 1);
    display.print(". ");

    int16_t x1, y1;
    uint16_t textWidth, textHeight;
    char scoreBuffer[10];  // Buffer for the number as a string
    sprintf(scoreBuffer, "%ld", highscores[i] / 10); // Convert number to string
    display.getTextBounds(scoreBuffer, 0, 0, &x1, &y1, &textWidth, &textHeight); // Calculate score width
    display.setCursor(55 - textWidth, 20 + (i * 10)); // Set cursor for left-aligned display

    // If this is the current score, make it blink (not displayed every 100ms)
    // The continue statement skips the next iteration so that the score is not displayed, creating a blinking effect
   if (i == newHighscoreIndex && !showBlink) {
      continue;   
    }
    display.print(highscores[i] / 10); 
  }

  display.setRotation(0);
  display.display();
}


// ---- Draw dashed lines between lanes ------ //
void drawDashedLines() {

  for (int i = 1; i < LANES; i++) {
    for (int j = 0; j < SCREEN_WIDTH; j += 10) {
      display.drawFastHLine((j + dashedLineOffset / 10) % SCREEN_WIDTH, i * LANE_HEIGHT, 6, SSD1306_WHITE);
    }
  }
}


// ---- Draw obstacle sprites ------ //
void drawObstacles() {
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (obstacles[i].active) {
      // x-coordinate is divided by scale factor 10 to draw in actual pixels
      display.drawBitmap(obstacles[i].x / 10, lanePosition(obstacles[i].lane), car_allArray[obstacles[i].cartype], 10, 8, SSD1306_WHITE);
    }
  }
}

// ---- Draw player sprite at the current position ------ //
void drawPlayer() {
  display.drawBitmap(PLAYER_X, lanePosition(playerLane), player_car_, 10, 8, SSD1306_WHITE);
}

// ---- Display the current distance/score at the top of the screen ------ //
void drawDistanceCounter() {
  display.setRotation(1); // Display is mounted 90 degrees rotated on the breadboard, adjust rotation for the following output
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int16_t x1, y1;
  uint16_t textWidth, textHeight;
  char scoreBuffer[10];  // Buffer for the number as a string
  sprintf(scoreBuffer, "%ld", distanceCounter / 10); // Convert number to string, divide by scaling factor 10
  display.getTextBounds(scoreBuffer, 0, 0, &x1, &y1, &textWidth, &textHeight); // Calculate score width for centering
  display.fillRect(SCREEN_HEIGHT / 2 - textWidth / 2 - 2, 0, textWidth + 4, 8, SSD1306_BLACK); // Clear rectangle for score display
  display.setCursor(SCREEN_HEIGHT / 2 - textWidth / 2, 0); // Set cursor to display score text centered
  display.print(distanceCounter / 10); // Reduce by scaling factor 10
  display.setRotation(0);
}

// ---- Display game over overlay on the game field ------ //
void drawGameOver() {
  display.setRotation(1); // Display is mounted 90 degrees rotated on the breadboard, adjust rotation for the following output
  display.fillRect(1, 30, 62, 60, SSD1306_WHITE);
  display.setCursor(4, 34);
  display.setTextSize(1);
  display.setTextColor(SSD1306_BLACK);
  display.println("GAME OVER");
  display.setCursor(3, 50);
  display.println("Your score");
  display.setCursor(16, 66);

  int16_t x1, y1;
  uint16_t textWidth, textHeight;
  char scoreBuffer[10];  // Buffer for the number as a string
  sprintf(scoreBuffer, "%ld", distanceCounter / 10); // Convert number to string, divide by scaling factor 10
  display.setTextSize(2);
  display.getTextBounds(scoreBuffer, 0, 0, &x1, &y1, &textWidth, &textHeight); // Calculate score text width for centering
  display.setCursor(32 - textWidth / 2, 66);
  display.println(distanceCounter / 10); // Divide by scaling factor 10
  display.setRotation(0);
}


// ---- Insert high score into the top 10 list ------ //
void saveHighscores(long newScore) {
  newHighscoreIndex = -1;  // Store position if the result is in the top 10
  for (int i = 0; i < 10; i++) {     // Insert new score into the ranking
    if (newScore > highscores[i]) {  // Is the new score better?
      for (int j = 9; j > i; j--) {  // Shift old values down
        highscores[j] = highscores[j - 1];
      }
      highscores[i] = newScore;  // Insert new high score
      newHighscoreIndex = i;     // Store index of new score for blink animation
      break;
    }
  }
}

// ---- Gradually adjust buzzer frequency with playerSpeed ------ //
void updateBuzzer() {
  int buzzerFreq = map(playerSpeed, 10, 100, 50, 70);  // Map playerSpeed to frequency
  if (buzzerFreq != lastBuzzerFreq) {                  // Update only if frequency has changed
    tone(PIN_BUZZER, buzzerFreq);
    lastBuzzerFreq = buzzerFreq;
  }
}

// ---- Calculate Y-positions for cars on lanes ------ //
int lanePosition(int lane) {
  return lane * LANE_HEIGHT + (LANE_HEIGHT - PLAYER_HEIGHT) / 2;
}
*/
//=========================================//



//=========================================//
//============BUGS=========================//
//=========================================//
/*
1.Fix scroll bar  
2.Personalize the snake y pos to be 1 pixel up 
*/
//=========================================//
