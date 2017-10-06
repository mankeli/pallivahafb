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

 	for (int i = 0; i < 256; i++)
 	{
		  palette[i] &= 0xFFFF0000;
 	}
}