#pragma force_active on
#include "global.h"
typedef struct {int a,b,c;} Info;typedef struct {u8 pad[0x43c];int n;} Object;typedef struct {Info*info;u8 pad[0x21];u8 x;u8 pad2[6];int state;int unused;Object*obj;} Work;
extern void PCBox_CloseMessageWindow(Object*,int);extern void PCBox_SetSpriteVisible(Object*,int,int);extern void PCBox_SetIconsTranslucent(Work*,int,int);

int ov14_021F2778(Work*w);

int ov14_021F2778(Work*w){PCBox_CloseMessageWindow(w->obj,39);PCBox_SetSpriteVisible(w->obj,9,0);if(w->info->c==3)PCBox_SetIconsTranslucent(w,129,0);else PCBox_SetIconsTranslucent(w,1,0);w->obj->n=w->x%6;w->state=9;return 67;}
