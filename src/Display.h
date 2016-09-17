#ifndef _Display_h_
#define _Display_h_

#include "Buffer.h"

class Display {
	char display[4][21];

	int menu;
	int row;

	void showLine(int line, char *buf);

public:

	void init();

	uint16_t mem(bool alloced);

	void showDisplay(uint32_t now);

	void menuInput(Buffer &send, uint8_t i);
};

EXTERN Display display;

#endif
