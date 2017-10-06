#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <stdio.h>
#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <math.h>


#define MAP_SIZE 4096UL

#define COLORMAPPER_PRINT

#include "colormapper.h"

int main(int argc, char* argv[])
{
	int memfd = open("/dev/mem", O_RDWR | O_SYNC);

 	uint8_t* screen = (uint8_t *)mmap(0, 65536, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, 0x40000000);
 	uint32_t* palette = (uint32_t *)mmap(0, 256*4, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, 0x42000000);

#if 1
 	for (int i = 0; i < 64; i++)
 	{
 		palette[i+0] = getcolor(i*1024, i * 1024, i*1024);
 	}

 	for (int i = 0; i < 64; i++)
 	{
 		palette[i+64] = getcolor(i * 1024, 0, 0);
 	}

 	for (int i = 0; i < 64; i++)
 	{
 		palette[i+128] = getcolor(0, i * 1024, 0);
 	}

 	for (int i = 0; i < 64; i++)
 	{
 		palette[i+192] = getcolor(0, 0,i * 1024);
 	}
#endif

#if 0
 	for (int i = 0; i < 256; i++)
 	{
 		float a = (i / 128.f) * 3.14159f;
 		int y = 0x2000;
 		int u = CLIP((int)(sin(a) * 128));
 		int v = CLIP((int)(cos(a) * 128));

		  uint32_t final = (y << 16) | (((-u+256)&0xFF) << 8) | ((-v+256)&0xFF);

		  palette[i] = final;
 	}
#endif


 	for (int y = 0; y < 200; y++)
 	{
 		for (int x = 0; x < 320; x++)
 		{
 			screen[y*320+x] = x & 0xFF;
 		}
 	}
}