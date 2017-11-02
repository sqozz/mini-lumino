#include <LEDMatrixDriver.hpp>

// This sketch will 'flood fill' your LED matrix using the hardware SPI driver Library by Bartosz Bielawski.
// Example written 16.06.2017 by Marko Oette, www.oette.info 

// Define the ChipSelect pin for the led matrix (Dont use the SS or MISO pin of your Arduino!)
// Other pins are arduino specific SPI pins (MOSI=DIN of the LEDMatrix and CLK) see https://www.arduino.cc/en/Reference/SPI
const uint8_t LEDMATRIX_CS_PIN = 15;

// Define LED Matrix dimensions (0-n) - eg: 32x8 = 31x7
const int LEDMATRIX_WIDTH = 31;
const int LEDMATRIX_HEIGHT = 7;
const int LEDMATRIX_SEGMENTS = 4;

// The LEDMatrixDriver class instance
LEDMatrixDriver lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);

void setup() {
	// init the display
	lmd.setEnabled(true);
	lmd.setIntensity(2);   // 0 = low, 10 = high
	Serial.begin(9600);
}

int x=0, y=0;   // start top left
bool s = true;  // start with led on
int progress_height = 2;

void loop() {
	lmd.setPixel(0, 0, 1);
	lmd.setPixel(1, 1, 1);
	lmd.display();
	lmd.setPixel(LEDMATRIX_WIDTH, LEDMATRIX_HEIGHT, 1);
	lmd.display();
	delay(1000);
	progress(1.0);
	delay(1000);
	progress(0.0);
	delay(1000);
	while (true) {
		for (float prog = 0.0; prog <= 1.0; prog += 0.01) {
			progress(prog);
			delay(10);
		}
		progress(1.0);
		delay(10);
		for (float prog = 1.0; prog >= 0.0; prog -= 0.01) {
			progress(prog);
			delay(10);
		}
	}
}

void progress(float prog) {
	int progress_shown = (int)(prog * LEDMATRIX_WIDTH);
	for (int progress_row = LEDMATRIX_HEIGHT - progress_height + 1; progress_row <= LEDMATRIX_HEIGHT; progress_row++) {
		for (int x = 0; x <= progress_shown; x++) {
			lmd.setPixel(x, progress_row, 1);
		}
		for (int x = progress_shown + 1; x <= LEDMATRIX_WIDTH; x++) {
			lmd.setPixel(x, progress_row, 0);
		}
		if (progress_shown <= 1) {
			lmd.setPixel(0, progress_row, 0);
		}
	}

	lmd.display();
}
