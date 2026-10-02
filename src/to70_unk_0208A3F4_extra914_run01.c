#pragma force_active on
#include "global.h"
typedef struct {u8 pad[18];u8 mode;u8 pad2[5];u16 value;} Info;
typedef struct {u8 pad[0x22c];Info *info;u8 pad2[0x34];u16 entries[4];u8 pad3[0x551];u8 selected:4;u8 flags:4;} W;
int sub_0208A71C(W *,int);
int sub_0208A71C(W *w,int direction){u8 old=w->selected;s8 index=old;s8 max;if(w->info->mode==2){if(w->info->value)max=4;else max=3;}else max=3;do{index+=direction;if(index<0)index=max;else if(index>max)index=0;if(index==4)break;if(w->entries[index]!=0)break;}while(index!=old);if(index!=old){w->selected=index;return 1;}return 0;}
