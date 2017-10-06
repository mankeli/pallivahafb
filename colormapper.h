// adopted RGB->YUV from some stackoverflow example

#define CLIP16(X) ( (X) > 65535 ? 65535 : (X) < 0 ? 0 : X)
#define CLIP(X) ( (X) > 127 ? 127 : (X) < -128 ? -128 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP16(( (  66 * (R) + 129 * (G) +  25 * (B) + 128)) + 0x1000)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8))
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8))

uint32_t getcolor(int red, int green, int blue)
{
	int y,u,v;
	uint32_t final;

	red >>= 8;
	green >>= 8;
	blue >>= 8;
	y = RGB2Y(red, green, blue);
	u = RGB2U(red, green, blue);
	v = RGB2V(red, green, blue);

	final = (y << 16) | (((v+256)&0xFF) << 8) | ((u+256)&0xFF);

	#ifdef COLORMAPPER_PRINT
	printf("%i,%i:%i = %i,%i,%i  : %08X\n",red,green,blue,y,u,v, final);
	#endif

	return final;
}

