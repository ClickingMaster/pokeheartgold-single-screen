#pragma force_active on
#include "global.h"
#pragma force_active on
extern int ov45_0222C5B4(void*,int),ov45_0222C4E4(void*,int),ov45_0222C61C(void*,int,int),ov45_0222C4B4(void*,int);

int ov45_0222C408(void*w,void*p,int value);

int ov45_0222C408(void*w,void*p,int value){int column,base,row;if(ov45_0222C5B4(w,(s32)p)==-1){base=0;row=0;for(;row<9;row++,base+=3){if(ov45_0222C4E4(w,row)!=2){for(column=0;column<3;column++){if(ov45_0222C61C(w,column+base,value)==0 && ov45_0222C4B4(w,column+base)==-1){{u8 *slot=(u8*)w+12*row;slot+=4*column;*(void**)(slot+0x50)=p;}return column+base;}}}}}return -1;}
