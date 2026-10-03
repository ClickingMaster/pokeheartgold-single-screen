#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
extern void *OverlayManager_GetData(void *);extern void ov74_0222AA18(void *,void *,int);extern void ov74_02229F60(void *,int);extern void ov74_02229E28(void *,int);extern u32 ov74_0223D0A8[];
int ov74_0222A4CC(void *manager);
int ov74_0222A4CC(void *manager){void *data=OverlayManager_GetData(manager);ov74_0222AA18(manager,(u8 *)data+0x18,3);ov74_02229F60((u8 *)data+0x48,0);ov74_02229E28(data,1);ov74_0223D0A8[4]=37;return 32;}
