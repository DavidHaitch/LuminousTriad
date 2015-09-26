#include <FastLED.h>
#define LED_PIN 3

CRGB leds[3];
int brightness = 0;
int targetBrightness = 32;
int brightnessDir = 1;
CRGBPalette16 palette;
void setup()
{
	FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 3);
	pinMode(0, INPUT);

	//initialize seed by converting each character in the __TIME__ macro to int, and taking the sum.
	int seed = 0;
	for (int i = 0; i < 9; i++)
	{
		seed += (int)__TIME__[i];
	}

	srand(seed);

	palette = CRGBPalette16(
		0xFF0000, 0xD52A00, 0xAB5500, 0xAB7F00,
		0xABAB00, 0x56D500, 0x00FF00, 0x00D52A,
		0x00AB55, 0x0056AA, 0x0000FF, 0x2A00D5,
		0x5500AB, 0x7F0081, 0xAB0055, 0xD5002B);
}

long counter = rand() % 1000000;
long offset = rand() % 128;
void loop()
{
	bool touchState = digitalRead(0);

	if (touchState)
	{
		if (targetBrightness > 255)
		{
			brightnessDir = -1;
		}

		if (targetBrightness < -20)
		{
			brightnessDir = 1;
		}

		targetBrightness += brightnessDir;
	}

	if (targetBrightness < 0)
	{
		leds[0] = leds[1] = leds[2] = CRGB::Black;
		FastLED.show();
		return;
	}

	if (brightness < targetBrightness)
	{
		brightness++;
	}

	if (brightness > targetBrightness && brightness > 0)
	{
		brightness--;
	}

	if (!touchState)
	{
		for (int i = 0; i < 3; i++)
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
	}

	for (int i = 0; i < 3; i++)
	{
		leds[i].nscale8(brightness / 2);
	}

	if (rand() % 2 == 0 || counter % (50) == 0)
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






