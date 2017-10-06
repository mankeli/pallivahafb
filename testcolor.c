#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define COLORMAPPER_PRINT
#include "colormapper.h"



int main(int argc, char* argv[])
{
	uint32_t out = getcolor(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));

	printf("out: %08X\n", out);
}