#pragma force_active on
#include "global.h"
#include "system.h"
#include "sound.h"
#pragma force_active on
void PlaySE(int);
typedef struct{u8 a[0x10];u32 state;u8 b[0x42e - 0x14];u8 count,pad,selection,index;}Work;
int ov108_021E9528(Work*,int);void ov108_021EA584(Work*,u8,void*,int);void ov108_021EA2EC(Work*,int);void ov108_021EA47C(Work*);int ov108_021E95AC(Work*);
int ov108_021E95AC(Work*w){int refresh=0;u32 keys;u8 row,column;if(gSystem.newKeys&0xcf3)w->state=0;keys=gSystem.newKeys;if(keys&2)return ov108_021E9528(w,6);if(keys&1)return ov108_021E9528(w,w->index);if(!(keys&0xf0))return 0;column=(int)w->index%2;row=w->index/2;if(keys&0x10){if(row==3)return 0;if(column==1){if(w->selection<w->count-1&&row<3){w->selection++;refresh=2;}else return 0;}else column++;}else if(keys&0x20){if(row==3)return 0;if(column==0){if(w->selection&&row<3){w->selection--;refresh=1;}else return 0;}else column--;}else if(keys&0x40)row=(int)(row+3)%4;else if(keys&0x80)row=(int)(row+1)%4;w->index=column+row*2;if(refresh){ov108_021EA584(w,refresh-1,(u8*)w+0x14,0);return 0;}PlaySE(1509);ov108_021EA2EC(w,w->index);ov108_021EA47C(w);return 0;}
