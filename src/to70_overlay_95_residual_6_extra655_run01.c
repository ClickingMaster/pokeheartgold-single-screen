#pragma force_active on
#include "global.h"
typedef struct{u32 a[8];}T32;typedef struct{u32 a[5];}T20;typedef struct{u32 a[6];}T24;extern T32 ov95_021E7574;extern T20 ov95_021E7548;extern T24 ov95_021E755C;extern void*SpriteSystem_Alloc(int),*SpriteManager_New(void*);extern void SpriteSystem_Init(void*,T32*,T20*,int);extern int SpriteSystem_InitSprites(void*,void*,int),SpriteSystem_InitManagerWithCapacities(void*,void*,T24*);typedef struct{u8 pad[0x4c];void*m,*s;}Work;
void ov95_021E619C(Work*w);
void ov95_021E619C(Work*w){T32 a;T20 b;T24 c;w->s=SpriteSystem_Alloc(70);a=ov95_021E7574;b=ov95_021E7548;SpriteSystem_Init(w->s,&a,&b,32);c=ov95_021E755C;w->m=SpriteManager_New(w->s);GF_ASSERT(SpriteSystem_InitSprites(w->s,w->m,255));GF_ASSERT(SpriteSystem_InitManagerWithCapacities(w->s,w->m,&c));}
