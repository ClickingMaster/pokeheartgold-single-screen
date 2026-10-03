#pragma force_active on
#include "global.h"
typedef struct{u32 unused;u16 channel;} Work;
extern Work *ov74_022310C4(void *);extern u32 WM_GetAllowedChannel(void);
void ov74_0222FFAC(void *);
void ov74_0222FFAC(void *arg){int i,start,n;u32 mask;Work *w=ov74_022310C4(arg);mask=WM_GetAllowedChannel();if(mask){start=w->channel;n=start;for(i=0;i<16;i++,n++){if(mask&(1<<(n%16))){w->channel=(start+i)%16+1;return;}}}}
