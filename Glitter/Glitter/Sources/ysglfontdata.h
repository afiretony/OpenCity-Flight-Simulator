#ifndef YSGLFONTDATA_IS_INCLUDED
#define YSGLFONTDATA_IS_INCLUDED
/* { */

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char *YsFont6x7[256];
extern unsigned char *YsFont6x8[256];
extern unsigned char *YsFont6x10[256];
extern unsigned char *YsFont7x10[256];
extern unsigned char *YsFont8x8[256];
extern unsigned char *YsFont8x12[256];
extern unsigned char *YsFont12x16[256];
extern unsigned char *YsFont16x20[256];
extern unsigned char *YsFont16x24[256];
extern unsigned char *YsFont20x28[256];
extern unsigned char *YsFont20x32[256];

void YsGlUseFontBitmap6x7(int listBase);
void YsGlDrawFontBitmap6x7(const char str[]);
void YsGlUseFontBitmap6x8(int listBase);
void YsGlDrawFontBitmap6x8(const char str[]);
void YsGlUseFontBitmap6x10(int listBase);
void YsGlDrawFontBitmap6x10(const char str[]);
void YsGlUseFontBitmap7x10(int listBase);
void YsGlDrawFontBitmap7x10(const char str[]);
void YsGlUseFontBitmap8x8(int listBase);
void YsGlDrawFontBitmap8x8(const char str[]);
void YsGlUseFontBitmap8x12(int listBase);
void YsGlDrawFontBitmap8x12(const char str[]);
void YsGlUseFontBitmap12x16(int listBase);
void YsGlDrawFontBitmap12x16(const char str[]);
void YsGlUseFontBitmap16x20(int listBase);
void YsGlDrawFontBitmap16x20(const char str[]);
void YsGlUseFontBitmap16x24(int listBase);
void YsGlDrawFontBitmap16x24(const char str[]);
void YsGlUseFontBitmap20x28(int listBase);
void YsGlDrawFontBitmap20x28(const char str[]);
void YsGlUseFontBitmap20x32(int listBase);
void YsGlDrawFontBitmap20x32(const char str[]);

void YsGlSelectAndUseFontBitmapByHeight(int *selectedWidth,int *selectedHeight,int listBase,int fontHeight);

int YsGlWriteStringToSingleBitBitmap(
	char str[],
    unsigned char bmpPtr[],unsigned int bmpWid,unsigned int bmpHei,
    int bottomLeftX,int bottomLeftY,
    unsigned char *fontPtr[],int fontWid,int fontHei);

int YsGlWriteStringToRGBA8Bitmap(
    char str[],unsigned int c0,unsigned int c1,unsigned int c2,unsigned int c3,
    unsigned char bmpPtr[],unsigned int bmpWid,unsigned int bmpHei,
    int bottomLeftX,int bottomLeftY,
    unsigned char *fontPtr[],int fontWid,int fontHei);
/* In OpenGL:   c0=r  c1=g  c2=b  c3=a
   In Direct3D: c0=a  c1=r  c2=g  c3=b */


#ifdef __cplusplus
}
#endif


/* } */
#endif
