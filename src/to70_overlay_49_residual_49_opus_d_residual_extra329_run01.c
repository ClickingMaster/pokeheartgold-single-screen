#pragma force_active on
#include "global.h"
#pragma force_active on
int ov49_0225D450(void *,int);void ov49_0225D214(void *,void *,int,int),ov49_0225D3F8(void *,void *,int,int),ov49_0225D394(void *,void *),ov49_0225D4A0(void *,void *,int),ov49_0225D328(void *,void *,int),ov49_0225E8C4(u8 *,int,u32,int,int,int);
#define P(o) (*(void **)(w+(o)))
#define U(o) (*(u32 *)(w+(o)))
#define FIX(v) ((s32)(((v)<<2)?(float)((v)<<14)+0.5f:(float)((v)<<14)-0.5f))
void ov49_0225E8C4(u8 *w,int index,u32 value,int flag,int other,int remove){void *b,*a;a=*(void **)(w+556+index*4);b=*(void **)(w+568+index*4);if(other==1||flag==1){if(!ov49_0225D450(a,1))ov49_0225D214(P(4),a,1,0);if(other==1){ov49_0225D3F8(P(4),b,0,FIX(value+4));return;}U(1548)=(U(1548)+1)%28;if(U(1548)<14)ov49_0225D3F8(P(4),b,0,FIX(value));else ov49_0225D3F8(P(4),b,0,0);}else if(remove){ov49_0225D394(P(4),a);ov49_0225D394(P(4),b);ov49_0225D4A0(P(4),a,1);}else{ov49_0225D328(P(4),a,1);ov49_0225D3F8(P(4),b,0,0);}}
