#pragma force_active on
#include "global.h"
typedef struct{u32 unused;u8 mode;} Settings;
typedef struct{Settings *settings;u8 p4[0xd4];u8 resource[16],object[64];} Work;
extern void sub_02018030(void *,int,int,int);extern void sub_020181B0(void *,void *);extern void sub_020182A8(void *,int,int,int);extern void sub_020182C4(void *,int,int,int);extern void sub_020182A0(void *,int);
void ov89_02259BAC(Work *,int);
void ov89_02259BAC(Work *w,int arg){int id=w->settings->mode==0?28:27;sub_02018030(w->resource,arg,id,125);sub_020181B0(w->object,w->resource);sub_020182A8(w->object,0,0,0);sub_020182C4(w->object,4096,4096,4096);sub_020182A0(w->object,1);}
