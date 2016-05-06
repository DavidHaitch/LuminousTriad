#include <FastLED.h>
#define LED_PIN 3

CRGB leds[4]; //Four LEDs, since some models have four. 
CRGBPalette16 palette;

int brightness = 32;
int brightnessDir = 1;

void setup()
{
	FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 4);
	pinMode(0, INPUT);

	//initialize seed by converting each character in the __TIME__ macro to int, and taking the sum.
	int seed = 0;
	for (int i = 0; i < 9; i++)
	{
		seed += (int)__TIME__[i];
	}

	srand(seed);

  palette = RainbowColors_p;
  //If you want to define a custom color palette, follow the example below.
	//palette = CRGBPalette16(
	//	0xFF0000, 0xD52A00, 0xAB5500, 0xAB7F00,
	//	0xABAB00, 0x56D500, 0x00FF00, 0x00D52A,
	//	0x00AB55, 0x0056AA, 0x0000FF, 0x2A00D5,
	//	0x5500AB, 0x7F0081, 0xAB0055, 0xD5002B);
}

//Randomize starting positions
long counter = rand() % 1000000;
long offset = rand() % 128;
void loop()
{
	bool touchState = digitalRead(0);

	if (touchState)
	{
		if (brightness >= 255)
		{
			brightnessDir = -1;
		}

		if (brightness < 4)
		{
			brightnessDir = 1;
		}

		brightness += brightnessDir;
    delay(10);
	}

	if (!touchState)
	{
		for (int i = 0; i < 4; i++)
		{

			int color = inoise8((i + 1) * 1500 * brightness, 0, counter);
			color = qsub8(color, 16);
			color = qadd8(color, scale8(color, 39));
			leds[i] = ColorFromPalette(palette, (color + offset) % 256, 255, LINEARBLEND);
		}
	}
	else
	{
		leds[0] = CRGB(255, 0, 0);
		leds[1] = CRGB(0, 255, 0);
		leds[2] = CRGB(0, 0, 255);
    		leds[3] = CRGB(128, 128, 0);
	}

	for (int i = 0; i < 4; i++)
	{
		leds[i].nscale8(brightness);
	}

	if (rand() % 2 == 0)
	{
		offset += 1;
	}
	else
	{
		offset -= 1;
	}

	FastLED.show();
	counter++;
	delay(5);
}






