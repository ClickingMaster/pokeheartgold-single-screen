#pragma force_active on
#include "global.h"
extern int ov74_0223D454[];extern void BeginNormalPaletteFade(int,int,int,int,int,int,int);extern int IsPaletteFadeFinished(void);

void ov74_0223539C(int mode,int next,int*out,int value);
void ov74_022353FC(int*out);

void ov74_022353FC(int*out){int*g=ov74_0223D454;if(IsPaletteFadeFinished())*out=g[3];}
