#include <stdio.h>
#include <string.h>

#define RGB_COLOR_WHITE 0XFFFFFF	
#define RGB_COLOR_BLACK 0X000000
#define RGB_COLOR_RED 0XFF0000
#define RGB_COLOR_BLUE 0X0000FF
#define RGB_COLOR_GREEN 0X00FF00
#define RGB_COLOR_YELLOW 0XFFFF00
#define RGB_COLOR_ORANGE 0XFFA500
#define RGB_COLOR_PURPLE 0XA020F0


typedef struct _PIXEL_FORMAT_INFO
{
	unsigned char alpha_len;
	unsigned char red_len;
	unsigned char green_len;
	unsigned char blue_len;
	
}PIXEL_FORMAT_INFO;

int rgb_convert_argb(unsigned char red, unsigned char green, unsigned char blue, PIXEL_FORMAT_INFO pixel_format_info)
{
	unsigned char argb_alpha = pixel_format_info.alpha_len;
	unsigned char argb_red = red >> (8 - pixel_format_info.red_len);
	unsigned char argb_green = green >> (8 - pixel_format_info.green_len);
	unsigned char argb_blue = blue >> (8 - pixel_format_info.blue_len);
	unsigned short pixel = 0;
	unsigned int tmp = 15;

	while(argb_alpha)
	{
		pixel |= (1 << tmp);
		argb_alpha--;
		tmp--;
	}

	pixel |= argb_red;

	pixel |= argb_green << pixel_format_info.green_len;

	pixel |= argb_blue << (pixel_format_info.green_len + pixel_format_info.blue_len);

	printf("pixel:%0X\n", pixel);

	return 0;
}


int main(int argc, char* argv[])
{
	PIXEL_FORMAT_INFO pixel_format_info[8] = {{0, 0, 0, 0},		/*RESERVED*/
											  {0, 4, 4, 4},		/*RGB444*/
											  {0, 5, 5, 5},		/*RGB555*/
											  {0, 5, 6, 5},		/*RGB565*/
											  {0, 8, 8, 8},		/*RGB888*/
											  {1, 5, 5, 5},		/*ARGB1555*/
											  {4, 4, 4, 4},		/*ARGB4444*/
											  {8, 8, 8, 8}};	/*ARGB8888*/

	unsigned char r = RGB_COLOR_PURPLE & 0XFF;
	unsigned char g = (RGB_COLOR_PURPLE & 0XFF00) >> 8;
	unsigned char b = (RGB_COLOR_PURPLE & 0XFF0000) >> 16;
	
	rgb_convert_argb(r, g, b, pixel_format_info[5]);

    return 0;
}


