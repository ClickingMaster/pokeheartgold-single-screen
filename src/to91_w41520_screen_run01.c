#include "global.h"
#include "gf_gfx_loader.h"
#pragma force_active on
typedef struct {
 u8 preceding[0x58];void *narc;u8 untouched[0x200-0x5c];
 void *raw[2];NNSG2dScreenData *screen[2];
} ScreenOwner;
typedef char ScreenOwnerSize[(sizeof(ScreenOwner)==0x210)?1:-1];
typedef char ScreenOwnerOutput[(offsetof(ScreenOwner,screen)==0x208)?1:-1];
extern const u8 ov43_0222F0C0[];
void ov43_0222A998(void *,int);
void ov43_0222A998(void *owner,int heap){ScreenOwner*w=owner;int i;for(i=0;i<2;i++)w->raw[i]=GfGfxLoader_GetScrnDataFromOpenNarc(w->narc,ov43_0222F0C0[i],TRUE,&w->screen[i],(enum HeapID)heap);}
