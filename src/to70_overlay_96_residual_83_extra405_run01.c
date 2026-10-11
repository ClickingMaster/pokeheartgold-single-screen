#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad[8];void*bg;u8 gap[20];void*screen;u8 tail[48];void*sprites[3];}W;extern void CopyRectToBgTilemapRect(void*,int,u8,int,int,int,void*,u8,u8,int,int),ScheduleBgTilemapBufferTransfer(void*,int);void ov96_02203BD0(W*,int);
extern void PokeathlonCourseSpritePoolSprite_TryChangeAnim(void*,int);
void ov96_02203CA4(W*,u32,int);
void ov96_02203CA4(W*w,u32 index,int value){if(index>=3)GF_AssertFail();PokeathlonCourseSpritePoolSprite_TryChangeAnim(w->sprites[index],value+index);}
