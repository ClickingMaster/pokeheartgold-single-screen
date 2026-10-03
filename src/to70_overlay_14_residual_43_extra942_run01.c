#pragma force_active on
#include "global.h"
typedef struct {int a,b,c;} Info;typedef struct {u8 pad[0x43c];int n;} Object;typedef struct {Info*info;u8 pad[0x21];u8 x;u8 pad2[6];int state;int unused;Object*obj;} Work;
extern void ov14_021F6654(Object*,int);extern void ov14_021F2A18(Object*,int,int);extern void ov14_021F3488(Work*,int,int);

int ov14_021F2778(Work*w);

int ov14_021F2778(Work*w){ov14_021F6654(w->obj,39);ov14_021F2A18(w->obj,9,0);if(w->info->c==3)ov14_021F3488(w,129,0);else ov14_021F3488(w,1,0);w->obj->n=w->x%6;w->state=9;return 67;}
