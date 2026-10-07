#pragma force_active on
#include "global.h"
typedef struct {u8 p[12];void *data;} Palette;typedef struct {u8 p[0x14];u8 count;u8 p2[23];u8 disabled[8];u8 p3[0x624];Palette *pal;u16 frame,enabled;} Work;
extern u8 ov90_0225C1EC[];extern int GF_CreateNewVramTransferTask(int,u32,void *,u32);extern void GF_AssertFail(void);void ov90_0225B2A8(Work *);
#pragma force_active on
static inline u8 *Source(Work*w,u8*base,u32 n){if(w->frame==0)return base+(n+3)*32;else return base+(n+7)*32;}
void ov90_0225B2A8(Work *w){if(w->enabled!=0){if(w->frame==0||w->frame==8){int i;const u8 *p;i=0;if(i<w->count){p=ov90_0225C1EC;do{if(w->disabled[i]==0){u32 n=*p;u32 destination=n*32;u8*source=w->pal->data;if(!GF_CreateNewVramTransferTask(15,destination,Source(w,source,n),32))GF_AssertFail();}p++;i++;}while(i<w->count);}}w->frame=((int)w->frame+1)%16;}}
