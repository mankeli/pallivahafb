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

	uint32_t* palette = (uint32_t *)mmap(0, 256*4, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, 0x42000000);

	for (int i = 0; i < 16; i++)
	{
		int r,g,b,w;

		r = i & 4 ? 0xAA : 0;
		g = i & 2 ? 0xAA : 0;
		b = i & 1 ? 0xAA : 0;

		w = i & 8 ? 0x55 : 0;

		r += w;
		g += w;
		b += w;


		r*=256;
		g*=256;
		b*=256;

		palette[i] = getcolor(r,g,b);
	}
}