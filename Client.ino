#include <LCD5110_Graph.h>

LCD5110 myGLCD(8,9,10,11,12);

#define BUF_SIZE 504

uint8_t buf[BUF_SIZE];

int pos = 0;

void setup()
{
	Serial.begin(115200);

	myGLCD.InitLCD();
	randomSeed(analogRead(7));

	clear();

	while (Serial.available())
		Serial.read();

	Serial.write("1"); // Tell server that we are ready to receive
}

void clear()
{
	for (int i = 0; i < BUF_SIZE; i++)
		buf[i] = 0; 
}

// Analog of LCD5110.drawBitmap, but works with RAM arrays instead of ROM ones
void drawBitmap(LCD5110* lcd, int x, int y, uint8_t* bitmap, int sx, int sy)
{
	int bit;
	byte data;

	for (int cy=0; cy<sy; cy++)
	{
		bit= cy % 8;
		for(int cx=0; cx<sx; cx++)
		{
			data=bitmap[cx+((cy/8)*sx)];
			if ((data & (1<<bit))>0)
				lcd->setPixel(x+cx, y+cy);
			else
				lcd->clrPixel(x+cx, y+cy);
		}
	}
}

void loop()
{
	if (pos < BUF_SIZE)
		return;
	
	drawBitmap(&myGLCD, 0, 0, buf, 84, 48);

	myGLCD.update();
	pos = 0;
	Serial.write("2"); // Tell server that we are ready for next frame
}

void serialEvent()
{
	while (Serial.available()) {
	byte b = Serial.read();
	if (pos < BUF_SIZE)
		buf[pos++] = b;
	}
}
