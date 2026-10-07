#include "global.h"
#include "bg_window.h"
#pragma force_active on
extern const WindowTemplate*ov112_021FF608[],*ov112_021FF5D8[],*ov112_021FF5D0[],*ov112_021FF640[],*ov112_021FF660[],*ov112_021FF624[],*ov112_021FF684[],*ov112_021FF5F0[];
extern const u8 ov112_021FEC80[];
void ov112_021E9A78(void*,int);
#define BANK(window,table,count,field) do{Window*w;const WindowTemplate*const*t=table;i=0;w=window;do{AddWindow((BgConfig *)*(void **)((u8 *)v+24),w,*t);i++;t++;w++;}while(i<count);*((u8 *)v+field)=type;}while(0)
void ov112_021E9A78(void*v,int type){u32 i;if(type>=8)GF_AssertFail();switch(type){case 0:BANK((Window *)((u8 *)v+0x1eba8),ov112_021FF608,7,0x1ec4b);break;case 1:BANK((Window *)((u8 *)v+0x1ea68),ov112_021FF5D8,6,0x1ec4a);break;case 2:BANK((Window *)((u8 *)v+0x1eba8),ov112_021FF5D0,2,0x1ec4b);break;case 3:BANK((Window *)((u8 *)v+0x1ea68),ov112_021FF640,8,0x1ec4a);break;case 4:BANK((Window *)((u8 *)v+0x1ea68),ov112_021FF660,9,0x1ec4a);break;case 5:BANK((Window *)((u8 *)v+0x1ea68),ov112_021FF624,7,0x1ec4a);break;case 6:BANK((Window *)((u8 *)v+0x1ea68),ov112_021FF684,16,0x1ec4a);break;case 7:BANK((Window *)((u8 *)v+0x1ea68),ov112_021FF5F0,6,0x1ec4b);break;}*((u8 *)v+0x1ec48+ov112_021FEC80[type])=i;}
