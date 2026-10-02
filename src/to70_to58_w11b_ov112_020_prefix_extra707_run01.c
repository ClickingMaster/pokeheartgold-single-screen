#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Callback {int (*call)(u8 *);u32 padding[2];} Callback;extern Callback ov112_021FF54C[],ov112_021FF550[],ov112_021FF554[];
u8 *OverlayManager_GetData(void *);int IsPaletteFadeFinished(void),ov112_021E7830(void *,u32 *);void BgSetPosTextAndCommit(void *,int,int,int),BeginNormalPaletteFade(int,int,int,int,int,int,int),ov112_021E9C94(void *),SpriteSystem_DrawSprites(void *);
int ov112_021E7830(void *manager,u32 *step){u8 *s=OverlayManager_GetData(manager);switch(*step){case 0:if(IsPaletteFadeFinished()==1)*step=1;break;case 1:*step=ov112_021FF54C[*(u32 *)s].call(s);break;case 2:*step=ov112_021FF550[*(u32 *)s].call(s);BgSetPosTextAndCommit(*(void **)(s+24),5,0,*(u16 *)(s+0x1f2e0));break;case 3:*step=ov112_021FF554[*(u32 *)s].call(s);if(*(int *)(s+4)==11){BeginNormalPaletteFade(0,0,0,0x7fff,6,1,154);*step=4;}else{*(int *)s=*(int *)(s+4);ov112_021E9C94(s);}break;case 4:if(IsPaletteFadeFinished()==1)return 1;break;}if(*(void **)(s+0x1e52c))SpriteSystem_DrawSprites(*(void **)(s+0x1e52c));return 0;}
