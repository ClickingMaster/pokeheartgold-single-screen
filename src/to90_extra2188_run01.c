#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u16 x,y;u32 rest[12];} Template;
typedef struct {u8 pad[0x66c];void *manager;void *sprites[24];} Work;
extern Template ov18_021FA484,ov18_021FAB24;void ov18_021F2424(Work *,int,Template *);int SpriteManager_FindPlttResourceOffset(void *,int,int);void ManagedSprite_SetPaletteOverride(void *,int);void ov18_021F2468(Work *);
void ov18_021F2468(Work *w){Template t=ov18_021FA484;u32 i;int x;for(i=18,x=0x1b0;i<=23;i++){t.x=x-0x134;ov18_021F2424(w,i,&t);ManagedSprite_SetPaletteOverride(w->sprites[i],SpriteManager_FindPlttResourceOffset(w->manager,50522,2));x+=24;}ov18_021F2424(w,8,&ov18_021FAB24);}
