#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W31687Parent {u8 pad[0x1c];void *profile;} W31687Parent;
typedef struct W31687 {W31687Parent *parent;u8 a[0x10];u32 index;u8 b[0x38];u32 field50;u8 c[0xc0];u32 reset;u8 d[0x10];u32 field128;u8 e[0xac8];void *sprites;} W31687;
typedef u32 (*W31687Callback)(W31687 *,u32);
typedef struct W31687Callbacks {W31687Callback first,second,third;} W31687Callbacks;
extern const W31687Callbacks ov70_022463EC[];
extern u32 ov70_02246944;
extern W31687 *OverlayManager_GetData(void *);
extern void ov00_021ECB40(void),ov70_022378DC(void),ov00_021EC294(void (*)(void),void (*)(void)),ov70_02238DF8(void),ov70_02238E20(void),ov70_02238880(void),ov70_02238E98(W31687 *),ov70_02238E70(W31687 *),ov70_02240D74(W31687 *,u32),ov70_02241184(W31687 *,u32,int),ov70_02239C6C(W31687 *),ov70_02239CF8(W31687 *),ov70_02238F04(W31687 *),ov70_02238F24(W31687 *),SpriteList_RenderAndAnimateSprites(void *);
extern int sub_02034DB8(void),IsPaletteFadeFinished(void);
extern u32 PlayerProfile_GetTrainerGender(void *);
BOOL ov70_022385C0(void *,u32 *);
BOOL ov70_022385C0(void *manager,u32 *state){W31687 *p=OverlayManager_GetData(manager);ov00_021ECB40();ov70_022378DC();switch(*state){case 0:if(sub_02034DB8()){ov70_02246944=p->field50;ov00_021EC294(ov70_02238DF8,ov70_02238E20);*state=1;}break;case 1:*state=ov70_022463EC[p->index].first(p,*state);ov70_02238880();if(p->reset)ov70_02238E98(p);break;case 2:if(IsPaletteFadeFinished())*state=3;break;case 3:*state=ov70_022463EC[p->index].second(p,*state);break;case 4:if(IsPaletteFadeFinished()){if(p->reset){ov70_02238E70(p);ov70_02240D74(p,PlayerProfile_GetTrainerGender(p->parent->profile));ov70_02241184(p,p->field128,0);ov70_02239C6C(p);ov70_02239CF8(p);p->reset=0;}*state=ov70_022463EC[p->index].third(p,*state);}break;case 5:return TRUE;}ov70_02238F04(p);ov70_02238F24(p);if(p->sprites)SpriteList_RenderAndAnimateSprites(p->sprites);return FALSE;}
