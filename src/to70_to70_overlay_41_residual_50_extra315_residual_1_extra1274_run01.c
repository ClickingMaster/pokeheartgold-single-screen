#pragma force_active on
#include "global.h"
void ov41_022498E8(s8*,int,int,u8*),ov41_02249978(u8*,int,int,int,int);
void ov41_022498E8(s8 *pixels,int stride,int unused,u8 *bounds){int x,y,row;bounds[0]=40;bounds[1]=40;bounds[2]=40;bounds[3]=40;for(x=0;x<80;x++){int right;y=0;right=80-x;row=y;for(;y<80;y++){int index=x+row;u8 mask=15<<((index%2)*4);if(pixels[index/2]&mask){if(bounds[0]>x)bounds[0]=x;if(bounds[1]>right)bounds[1]=right;if(bounds[2]>y)bounds[2]=y;if(bounds[3]>80-y)bounds[3]=80-y;}row+=stride;}}}
