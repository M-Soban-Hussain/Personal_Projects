#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

MCUFRIEND_kbv tft;

// Touch pins
#define YP A1
#define XM A2
#define YM 7
#define XP 6
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// ===== Calibration constants =====
int TS_MINX = 244;
int TS_MAXX = 868;
int TS_MINY = 266;
int TS_MAXY = 905;

// ===== Adjustable mapping variables =====
bool invertX = true;      // true = flip X axis
bool invertY = true;       // true = flip Y axis
int xOffset = 0;           // horizontal adjustment
int yOffset = 0;           // vertical adjustment
float xScale = 1;        // stretch/shrink X mapping
float yScale = 1;        // stretch/shrink Y mapping

// ===== Grid settings =====
const int BOX_ROWS = 12;
const int BOX_COLS = 16;
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;
int boxWidth = SCREEN_WIDTH / BOX_COLS;
int boxHeight = SCREEN_HEIGHT / BOX_ROWS;

// ===== Draw grid with numbers =====
void drawGrid() {
  tft.fillScreen(0xFFFF); // white
  tft.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0x0000);
  int count = 1;
  tft.setTextSize(1);
  tft.setTextColor(0x0000);
  for (int r = 0; r < BOX_ROWS; r++) {
    for (int c = 0; c < BOX_COLS; c++) {
      int x = c * boxWidth;
      int y = r * boxHeight;
      tft.drawRect(x, y, boxWidth, boxHeight, 0x0000);
      tft.setCursor(x + 2, y + 2);
      tft.print(count);
      count++;
    }
  }
}

void setup() {
  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(1);
  drawGrid();
}

void loop() {
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > 50 && p.z < 1000) {
    float nx = float(p.x - TS_MINX) / (TS_MAXX - TS_MINX); // normalized 0-1
    float ny = float(p.y - TS_MINY) / (TS_MAXY - TS_MINY);

    if (invertX) nx = 1.0 - nx;
    if (invertY) ny = 1.0 - ny;

    int x = ny * SCREEN_WIDTH * xScale + xOffset; // swap
    int y = nx * SCREEN_HEIGHT * yScale + yOffset; // swap

    x = constrain(x, 0, SCREEN_WIDTH - 1);
    y = constrain(y, 0, SCREEN_HEIGHT - 1);

    tft.fillCircle(x, y, 3, 0xF800); // red pen
  }
}
