#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x10];int mode;u8 pad2[0x14];void *argument;u8 pad3[0xbcc];void *sprites;} Work;
typedef struct {int (*callback)(Work *,int);void *pad[2];} Entry;
extern Entry ov73_021EA83C[],ov73_021EA840[],ov73_021EA844[];extern u32 _021EA940[];
extern Work *OverlayManager_GetData(void *);extern void ov00_021ECB40(void);extern void NintendoWfcHttp_Update(void);extern int sub_02034DB8(void);extern void *ov73_021E83F4(int,u32,int);extern void ov73_021E841C(int,void *,u32);extern void ov00_021EC294(void *(*)(int,u32,int),void (*)(int,void *,u32));extern int IsPaletteFadeFinished(void);extern void SpriteList_RenderAndAnimateSprites(void *);
int ov73_021E7FB8(void *,int *);
int ov73_021E7FB8(void *man,int *state){Work *w=OverlayManager_GetData(man);ov00_021ECB40();NintendoWfcHttp_Update();ov00_021ECB40();switch(*state){case 0:if(sub_02034DB8()){_021EA940[1]=(u32)w->argument;ov00_021EC294(ov73_021E83F4,ov73_021E841C);*state=1;}break;case 1:*state=ov73_021EA83C[w->mode].callback(w,*state);break;case 2:if(IsPaletteFadeFinished())*state=3;break;case 3:*state=ov73_021EA840[w->mode].callback(w,*state);break;case 4:if(IsPaletteFadeFinished())*state=ov73_021EA844[w->mode].callback(w,*state);break;case 5:return 1;}if(w->sprites)SpriteList_RenderAndAnimateSprites(w->sprites);return 0;}
