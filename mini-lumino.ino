#include <LEDMatrixDriver.hpp>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include "secrets.h"
#include "font.h"

// Pin for the software CS
const uint8_t LEDMATRIX_CS_PIN = 15;

// Define LED Matrix dimensions
const int MODULE_HEIGHT = 8; // height of a single LED matrix module
const int MODULE_WIDTH = 8; // width of a single LED matrix module
const int LEDMATRIX_WIDTH = 64; // physical width of the display
const int LEDMATRIX_HEIGHT = 16; // physical height of the display

// Total number of segments (automatic calculation)
const int LEDMATRIX_SEGMENTS = (LEDMATRIX_WIDTH / MODULE_WIDTH) * (LEDMATRIX_HEIGHT / MODULE_HEIGHT);

// Mini-lumino settings
const int PROGRESS_HEIGHT = 2;

// The LEDMatrixDriver class instance
LEDMatrixDriver lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);
Ticker scroller;
WiFiClient mpd_client;

int x=0, y=0;
char* display_text = "";

char states[4][6] = {
	{ 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x0 },
	{ 0x8, 0xc, 0xe, 0xc, 0x8, 0x0 },
	{ 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x0 },
	{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 },
};

#define STOP 0,
#define PLAY 1,
#define PAUSE 2,
#define IDLE 3,

void setup() {
	// init the display
	lmd.setEnabled(true);
	lmd.setIntensity(2); // 0 = low, 10 = high
	Serial.begin(9600); // init serial for debugging
	display_text = "...";
	title(display_text);

	WiFi.mode(WIFI_STA);
	//WiFi.begin(ssid, password);
	//while (WiFi.status() != WL_CONNECTED) {
	//	delay(500);
	//}
	display_text = "";
	title(display_text);
}

void loop() {
	display_text = "A";
	//scroller.attach(0.5, rotateText);
	while (true) {
		//if (!mpd_client.connect("10.42.0.3", 6600)){
		//	return;
		//}
		//String resp = mpd_client.readStringUntil('\n');
		//String title = "";
		//String name = "";
		//String state = "";
		//if (resp.indexOf("OK") == 0) {
		//	mpd_client.print("currentsong\n");
		//	resp = mpd_client.readStringUntil('\n');
		//	while (!(resp.lastIndexOf("Id") == 0)) {
		//		if (resp.lastIndexOf("Title: ") == 0) {
		//			title = resp.substring(7, resp.length());
		//			Serial.println(title);
		//		}
		//		if (resp.lastIndexOf("Name: ") == 0) {
		//			name = resp.substring(6, resp.length());
		//			Serial.println(name);
		//		}
		//		resp = mpd_client.readStringUntil('\n');
		//	}

		//	mpd_client.print("status\n");
		//	resp = mpd_client.readStringUntil('\n');
		//	while (!(resp.lastIndexOf("audio") == 0) && !(resp.lastIndexOf("songid") == 0)) {
		//		if (resp.lastIndexOf("state: ") == 0) {
		//			state = resp.substring(7, resp.length());
		//			Serial.println(state);
		//		}
		//		resp = mpd_client.readStringUntil('\n');
		//	}
		//}

		//if (state == "play") {
		//	status(play);
		//} else if (state == "stop") {
		//	status(stop);
		//} else if (state == "pause") {
		//	status(pause);
		//}

		String title = "";
		String name = "";
		String song_text = name + ' - ' + title;
		status(states[0]);
		//display_text = (char*) song_text.c_str();
		display_text = const_cast<char*>(title.c_str());
		//text(display_text, 0);

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
		mpd_client.stop();
	}
}

void status(char new_status[]) {
	for (int icon_line = 5; icon_line >= 0; icon_line--) {
		char line = new_status[icon_line];
		for (int x = 0; x <= 5; x++) {
			bool pixel_state = line & (0x20 >> x); // we want the sixth (0x20) pixel of that number)
			setPixel(x, icon_line + 5, pixel_state);
		}
	}
	lmd.display();
}

void progress(float prog) {
	int progress_shown = (int)(prog * (LEDMATRIX_WIDTH - 1));
	for (int progress_row = LEDMATRIX_HEIGHT; progress_row >= (LEDMATRIX_HEIGHT - PROGRESS_HEIGHT); progress_row--) {
		for (int x = 0; x <= progress_shown; x++) {
			setPixel(x, progress_row, 1);
		}
		for (int x = progress_shown + 1; x < LEDMATRIX_WIDTH; x++) {
			setPixel(x, progress_row, 0);
		}
		if (progress_shown <= 1) {
			setPixel(0, progress_row, 0);
		}
	}

	lmd.display();
}

void title(char new_title[]) {
	int l_offset = 0;
	drawString(new_title, strlen(new_title), 8, 0, l_offset);
	lmd.display();
}

void drawString(char* text, int len, int left, int top, int l_offset ) {
	const char CHAR_SPACING = 1;
	const char FONT_WIDTH = 6;
	const char FONT_HEIGHT = 7;

	for( int idx = 0; idx < len; idx ++ ) {
		int char_offset = idx * (FONT_WIDTH + CHAR_SPACING); // current offset in px from the left for the next char
		int c = text[idx] - 32; // ASCII char 32 ("SPACE") is the first element in the font array

		int next_x = (left + char_offset) - l_offset;
		int next_y = top;

		// only draw if char is in range after l_offset shift
		if ((next_x >= left || next_x < LEDMATRIX_WIDTH) && (next_y >= top || next_y < LEDMATRIX_HEIGHT)) {
				drawSprite(font[c], next_x, next_y, FONT_WIDTH, FONT_HEIGHT);
		}

	}
}

void drawSprite( char* sprite, int x, int y, int width, int height ) {
	// The mask is used to get the column bit from the sprite row
	byte mask = B10000000;

	for( int iy = 0; iy <= height; iy++ ) {
		for( int ix = 0; ix <= width; ix++ ) {
			setPixel(x + ix - 1, y + iy - 1, (bool)(sprite[iy] & mask ));
			// shift the mask by one pixel to the right
			mask = mask >> 1;
		}

		// reset column mask
		mask = B10000000;
	}
}
void setPixel(int x, int y, bool value) {
	// LEDMatrixDriver is only designed to address one-matrix-heigh displays.
	// To archive multiple stacked displays on top of each other we just fake
	// a very long display and map y-coords >=8 onto the next segment by adding
	// 64 to the x value
	int real_x = (((int)(y / MODULE_HEIGHT)) * LEDMATRIX_WIDTH) + x;
	int real_y = y - (((int)(y / MODULE_HEIGHT)) * MODULE_HEIGHT);

	lmd.setPixel(real_x, real_y, value);
}

//int current_offset = -4;
//void rotateText() {
//	int max_length = strlen(display_text) * 4;
//	int px_visible = LEDMATRIX_WIDTH - 7;
//	int overlap = max_length - px_visible;
//	overlap += 0;
//	current_offset++;
//
//	if (current_offset >= overlap) {
//		text(display_text, overlap);
//		if (current_offset >= overlap + 2) {
//			current_offset = -6;
//		}
//	} else if (current_offset < 0) {
//		text(display_text, 0);
//	} else {
//		text(display_text, current_offset);
//	}
//}
