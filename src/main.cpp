#include <Arduino.h>
// #include <ESP32-HUB75-32x16-MatrixPanel-I2S-DMA.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "glcdfont.c"

#define PANEL_RES_X 64 // Number of pixels wide of each INDIVIDUAL panel module.
#define PANEL_RES_Y 16 // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1  // Total number of panels chained one to another

// MatrixPanel_I2S_DMA dma_display;
MatrixPanel_I2S_DMA *dma_display = nullptr;


uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

uint8_t wheelval = 0;

uint16_t colorWheel(uint8_t pos)
{
	if (pos < 85)
	{
		return dma_display->color565(pos * 3, 255 - pos * 3, 0);
	}
	else if (pos < 170)
	{
		pos -= 85;
		return dma_display->color565(255 - pos * 3, 0, pos * 3);
	}
	else
	{
		pos -= 170;
		return dma_display->color565(0, pos * 3, 255 - pos * 3);
	}
}

struct point
{
	int oX;
	int oY;
	int x;
	int y;
	point(int x, int y)
	{
		this->oX = x;
		this->oY = y;

		// now we calculate the offset
		// System Analysis:
		// The Matrix indexing is set like this
		// one block bottom left is 0,0

		// to get a vertical alignment we need to calculate the offset
		// The points for the right most line are
		/*
		0, 0  ->  0,0
		0, 1  ->  0,1
		0, 2  ->  0,2
		0, 3  ->  0,3
		0, 4  ->  8,0
		0, 5  ->  8,1
		0, 6  ->  8,2
		0, 7  ->  8,3
		0, 8  ->  0,16
		0, 9  ->  0,17
		0, 11 ->  0,18
		0, 12 ->  0,19
		0, 13 ->  8,16
		0, 14 ->  8,17
		0, 15 ->  8,18
		0, 16 ->  8,19


		8,0 -> 8,0
		8,1 -> 8,1
		8,2 -> 8,2
		8,3 -> 8,3
		8,4 -> 16,0
		8,5 -> 16,1
		8,6 -> 16,2
		8,7 -> 16,3
		8,8 -> 8,16
		8,9 -> 8,17
		8,11 -> 8,18
		8,12 -> 8,19
		8,13 -> 16,16
		8,14 -> 16,17
		8,15 -> 16,18
		8,16 -> 16,19




		*/
		// using these points lets just draw and check
		// xmap
		/*
		0 - 0
		1 - 1
		2 - 2
		3 - 3
		4 - 4
		5 - 5
		6 - 6
		7 - 7
		8 - 16
		9 - 17
		10 - 18
		11 - 19
		12 - 20
		13 - 21
		14 - 22
		15 - 23
		16 - 32
		*/

		// calculing x offset
		if (this->oX >= 56 && this->oX <= 64)
		{
			this->oX = this->oX + 56;
		}
		else if (this->oX >= 48 && this->oX <= 55)
		{
			this->oX = this->oX + 48;
		}
		else if (this->oX >= 40 && this->oX <= 47)
		{
			this->oX = this->oX + 40;
		}
		else if (this->oX >= 32 && this->oX <= 39)
		{
			this->oX = this->oX + 32;
		}
		else if (this->oX >= 24 && this->oX <= 31)
		{
			this->oX = this->oX + 24;
		}
		else if (this->oX >= 16 && this->oX <= 23)
		{
			this->oX = this->oX + 16;
		}
		else if (this->oX >= 8 && this->oX <= 15)
		{
			this->oX = this->oX + 8;
		}

		// we can see that the offset is 8
		// so we can just add 8 to the x value
		// and we will get the correct point
		// therefore
		if (oY > 3 && oY < 8 || oY > 11 && oY < 16)
		{
			this->x = oX + 8;
		}
		else
		{
			this->x = oX;
		}

		if (oY > 7 && oY < 13 || oY > 12 && oY < 17)
		{
			this->y = oY + 8;
		}
		else
		{
			this->y = oY;
		}
	}

	void printPointDetails()
	{
		Serial.print("Original X: ");
		Serial.print(oX);
		Serial.print(" Original Y: ");
		Serial.print(oY);
		Serial.print(" X: ");
		Serial.print(x);
		Serial.print(" Y: ");
		Serial.println(y);
	}
};

void printPixel(point p, uint16_t colorVal)
{
	dma_display->drawPixel(p.x, p.y, colorVal);
}

class mirageGFX : public Adafruit_GFX
{

public:
	mirageGFX(int16_t w, int16_t h) : Adafruit_GFX(w, h) {}

	void drawPixel(int16_t x, int16_t y, uint16_t color)
	{
		point p(x, y);
		printPixel(p, color);
	}

	void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y)
	{
		if (!gfxFont)
		{ // 'Classic' built-in font

			if ((x >= _width) ||			  // Clip right
				(y >= _height) ||			  // Clip bottom
				((x + 6 * size_x - 1) < 0) || // Clip left
				((y + 8 * size_y - 1) < 0))	  // Clip top
				return;

			if (!_cp437 && (c >= 176))
				c++; // Handle 'classic' charset behavior

			startWrite();
			for (int8_t i = 0; i < 5; i++)
			{ // Char bitmap = 5 columns
				uint8_t line = pgm_read_byte(&font[c * 5 + i]);
				for (int8_t j = 0; j < 8; j++, line >>= 1)
				{
					if (line & 1)
					{
						if (size_x == 1 && size_y == 1)
							printPixel(point(x + i, y + j), color);
						// writePixel(x + i, y + j, color);
						// else
						//   writeFillRect(x + i * size_x, y + j * size_y, size_x, size_y, color);
					}
					else if (bg != color)
					{
						if (size_x == 1 && size_y == 1)
							printPixel(point(x + i, y + j), bg);
						// writePixel(x + i, y + j, bg);
						// else
						//   writeFillRect(x + i * size_x, y + j * size_y, size_x, size_y, bg);
					}
				}
			}
			endWrite();
		}
	}

	void drawText(int16_t x, int16_t y, const char *c, int len, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y)
	{

		int cursorX = x;
		int fontXOffset = 6;

		for (int currentCharIndex = 0; currentCharIndex < len; currentCharIndex++)
		{
			if (cursorX + fontXOffset < _width)
			{
				drawChar(cursorX, y, c[currentCharIndex], color, bg, size_x, size_y);
				cursorX += fontXOffset * size_x;
			}
			else
			{
				printPixel(point(cursorX, y), colorWheel(70));
				break;
			}
		}
	}
};
mirageGFX gfx(PANEL_RES_X, PANEL_RES_Y);
void drawText(int colorWheelOffset)
{

	dma_display->clearScreen();
	dma_display->flush();

	char text[] = "Faseeh";
	char text2[] = "LUMS";
	gfx.drawText(0, 0, text, 6, colorWheel(colorWheelOffset), 0, 1, 1);

	gfx.drawText(0, 8, text2, 4, colorWheel(colorWheelOffset), 0, 1, 1);


	delay(7000);
	dma_display->clearScreen(); 
	gfx.drawText(0, 1, "DDL", 3, colorWheel(wheelval), myBLACK, 1,1);
	gfx.drawText(0, 9, "LAB", 3, colorWheel(wheelval), myBLACK, 1,1);
}

void deprecated()
{
	// dma_display->clearScreen();
	// for (uint16_t x = 0; x < 64; x++)
	// {
	//   for (uint16_t y = 0; y < 32; y++)
	//   {
	//     Serial.println("x: " + String(x) + " y: " + String(y));
	//     dma_display->drawPixel(x, y, colorWheel(wheelval));
	//     delay(delayTime);
	//   }
	// }

	// lets check if our mapping is correct to get a vline on the right side
	// {dma_display->drawPixel(0, 0, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(0, 1, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(0, 2, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(0, 3, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 0, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 1, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 2, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 3, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(0, 16, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(0, 17, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(0, 18, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(0, 19, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 16, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 17, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 18, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 19, colorWheel(wheelval));
	// delay(delayTime);}

	// lets check if our mapping is correct to get a vline on the right side

	// print vertical line
	// for (uint16_t x = 0; x < 32; x++)
	// {
	//   for (uint16_t y = 0; y < 16; y++)
	//   {
	//     point p(x, y);
	//     p.printPointDetails();
	//     printPixel(p, colorWheel(wheelval));
	//   }
	//   // Serial.println("point x: " + String(x) + "     cal point x: " + String(p.x));
	//   delay(delayTime * 20);
	//   dma_display->clearScreen();
	// }

	// dma_display->fillScreen(colorWheel(wheelval));
	// delay(1000);
	// dma_display->clearScreen();

	uint16_t x = 0;

	// ask user which x to draw line at
	Serial.println("Enter x value to draw line at");
	while (Serial.available() == 0)
	{
		delay(1000);
	}
	x = Serial.parseInt();
	Serial.println("x: " + String(x));
	dma_display->clearScreen();
	for (uint16_t y = 0; y < 16; y++)
	{
		point p(x, y);
		p.printPointDetails();
		printPixel(p, colorWheel(wheelval));
	}

	// printing vertical line at x = 8

	// dma_display->drawPixel(8, 0, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 1, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 2, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 3, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8+8, 0, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8+8, 1, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8+8, 2, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8+8, 3, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 16, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 17, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 18, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8, 19, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8+8, 16, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8+8, 17, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8+8, 18, colorWheel(wheelval));
	// delay(delayTime);
	// dma_display->drawPixel(8+8, 19, colorWheel(wheelval));
	// delay(delayTime * 10);
	// dma_display->clearScreen();
}

void setup()
{

	// Module configuration
	HUB75_I2S_CFG mxconfig(
		PANEL_RES_X * 2, // module width
		PANEL_RES_Y * 2, // module height
		PANEL_CHAIN		 // Chain length
	);

	// begin serial
	Serial.begin(115200);
	// mxconfig.clkphase = false;
	// mxconfig.driver = HUB75_I2S_CFG::FM6126A;

	// Display Setup
	dma_display = new MatrixPanel_I2S_DMA(mxconfig);
	dma_display->begin();
	dma_display->clearScreen();
	/*

	for (int i = 0; i < PANEL_RES_X; i++)
	{
		point p(i, i / 2);
		printPixel(p, colorWheel(wheelval));
	}

	delay(1000);
	dma_display->clearScreen();

	// print inverted lin
	for (int i = 0; i < PANEL_RES_X; i++)
	{
		point p(i, 15 - (i / 2));
		printPixel(p, colorWheel(wheelval));
	}

	delay(1000);
	dma_display->clearScreen();

	for (int x = 0; x < PANEL_RES_X; x++)
	{
		for (int y = 0; y < PANEL_RES_Y; y++)
		{
			point p(x, y);
			printPixel(p, colorWheel(wheelval));
		}
		delay(100);
	}
	delay(1000);
	dma_display->clearScreen();

	drawText(colorWheel(0));

	delay(1000);
	dma_display->clearScreen();
	*/

	drawText(colorWheel(0));
}

int delayTime = 40;
float deltaTime = 0.05;
double t = 0.0;

void printSin(int xMin)
{
	if (xMin >= PANEL_RES_X)
	{
		xMin = 0;
	}
	// printing a moving sine wave
	// printing a sin wave
	for (int x = xMin; x < PANEL_RES_X; x++)
	{
		// print a sin wave, y(x,t);
		float y = (4.5 * sin((0.05 * PI * x) + t)) + 7.5;

		point p(x, y);
		printPixel(p, colorWheel(wheelval));
	}
	delay(delayTime);
	// dma_display->clearScreen();
	// printing a black sin wave
	for (int x = xMin; x < PANEL_RES_X; x++)
	{
		// print a sin wave, y(x,t);
		float y = (4.5 * sin((0.05 * PI * x) + t)) + 7.5;

		point p(x, y);
		printPixel(p, dma_display->color565(0, 0, 0));
	}
	wheelval++;
	t += deltaTime;
}

void loop()
{	
	printSin(18);
}
