#pragma force_active on
#include "global.h"
#pragma force_active on
int sub_02074490(void),sub_0207449C(void),sub_020744A8(void);
void SpriteSystem_LoadPaletteBuffer(void *,int,void *,void *,int,int,int,int,int,int);
void SpriteSystem_LoadCellResObj(void *,void *,int,int,int,int);
void SpriteSystem_LoadAnimResObj(void *,void *,int,int,int,int);
void ov57_02238DD0(u8 *);
void ov57_02238DD0(u8 *w){
 void *system=*(void **)(w+0xdc),*manager=*(void **)(w+0xe0),*palette=*(void **)(w+0xe8);
 SpriteSystem_LoadPaletteBuffer(palette,2,system,manager,20,sub_02074490(),0,3,1,16000);
 SpriteSystem_LoadCellResObj(system,manager,20,sub_0207449C(),0,17000);
 SpriteSystem_LoadAnimResObj(system,manager,20,sub_020744A8(),0,18000);
}
