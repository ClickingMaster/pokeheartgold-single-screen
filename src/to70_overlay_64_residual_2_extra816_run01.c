#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x134];void*sprites;}Work;
extern Work*OverlayManager_GetData(void*);extern void ov64_021E6288(Work*),ov64_021E605C(Work*),SpriteSystem_DrawSprites(void*);extern int IsPaletteFadeFinished(void),ov64_021E62C8(Work*);
int HallOfFameShowcase_Main(void*,u32*);
int HallOfFameShowcase_Main(void*man,u32*state){Work*w=OverlayManager_GetData(man);switch(*state){case 0:ov64_021E6288(w);(*state)++;break;case 1:if(IsPaletteFadeFinished()==1)(*state)++;break;case 2:if(ov64_021E62C8(w)==0)(*state)++;break;case 3:if(IsPaletteFadeFinished()==1)return 1;break;}ov64_021E605C(w);SpriteSystem_DrawSprites(w->sprites);return 0;}
