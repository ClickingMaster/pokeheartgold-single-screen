#pragma force_active on
#include "global.h"
#pragma force_active on
extern void Main_SetVBlankIntrCB(void (*)(void *),void *);
extern void Main_SetHBlankIntrCB(void *,void *);
extern void GfGfx_DisableEngineAPlanes(void);
extern void GfGfx_DisableEngineBPlanes(void);
extern void sub_0200FBF4(int,int);
extern void ResetVisibleHardwareWindows(int);
extern void SetKeyRepeatTimers(int,int);
extern void Heap_Create(int,int,u32);
extern void *OverlayManager_CreateAndGetData(void *,u32,u32);
extern void *OverlayManager_GetArgs(void *);
extern int MenuInputStateMgr_GetState(void *);
extern void *Save_PlayerData_GetOptionsAddr(void *);
extern int Options_GetTextFrameDelay(void *);
extern int Options_GetFrame(void *);
extern int sub_02037474(void);
extern void *BgConfig_Alloc(u32);
extern void ov51_021E5F64(void *);
extern void ov51_021E60D4(void);
extern void ov51_021E6238(void *);
extern void ov51_021E6354(void *);
extern void sub_020210BC(void);
extern void sub_02021148(int);
extern void TextFlags_SetCanTouchSpeedUpPrint(int);
extern void TextFlags_SetCanABSpeedUpPrint(int);
extern void Sound_SetSceneAndPlayBGM(int,int,int);
extern void ov51_021E7DA4(void *);
extern void PlaySE(u16);
extern void ov51_021E78F8(void *);
extern void ov51_021E7AF4(void *,const u8 *,int);
extern void ov51_021E7BD0(void *);
extern void ov51_021E6E60(void *);
extern void ov51_021E7664(void *,int,void *);
extern void ov51_021E6C00(int);
extern void ov51_021E6734(void *);
extern void ov51_021E76A4(void *,int);
extern void ov51_021E6B88(void *);
extern void sub_0203A964(void);
extern void GF_SndHandleSetPlayerVolume(int,int);
extern void BeginNormalPaletteFade(int,int,int,u16,int,int,u32);
extern void GfGfx_EngineATogglePlanes(int,int);
typedef struct Args {u8 kind;u8 p0[5];u16 flags;u8 p1[0x2b];u8 selected;u8 p2[0x638];void *input,*save;u32 result,enable;} Args;
typedef struct Work {u8 data[0x3444];} Work;
#define PTR(w,o,T) (*(T **)&(w)->data[o])
#define WORD(w,o) (*(u32 *)&(w)->data[o])
typedef struct Bits {u8 selected:1,wireless:1,rest:6;} Bits;
int TrainerCardMainApp_Init(void *,int *);
int TrainerCardMainApp_Init(void *manager,int *state){
 Work *w;void *options;int i;u16 mask;
 Main_SetVBlankIntrCB(NULL,NULL);Main_SetHBlankIntrCB(NULL,NULL);GfGfx_DisableEngineAPlanes();GfGfx_DisableEngineBPlanes();
 *(vu32 *)0x04000000&=0xffffe0ff;*(vu32 *)0x04001000&=0xffffe0ff;
 sub_0200FBF4(0,0);sub_0200FBF4(1,0);ResetVisibleHardwareWindows(0);ResetVisibleHardwareWindows(1);SetKeyRepeatTimers(4,8);
 Heap_Create(3,25,0x50000);w=OverlayManager_CreateAndGetData(manager,0x3444,25);memset(w,0,0x3444);
 PTR(w,0xe4,Args)=OverlayManager_GetArgs(manager);PTR(w,0xe8,Args)=PTR(w,0xe4,Args);
 WORD(w,0x310c)=MenuInputStateMgr_GetState(PTR(w,0xe4,Args)->input);
 options=Save_PlayerData_GetOptionsAddr(PTR(w,0xe4,Args)->save);w->data[0x343e]=Options_GetTextFrameDelay(options);w->data[0x343d]=Options_GetFrame(options);
 if(sub_02037474()==1)w->data[0x343a]|=2;
 if(PTR(w,0xe8,Args)->selected!=0)((Bits *)&w->data[0x343a])->selected=1;else ((Bits *)&w->data[0x343a])->selected=0;
 if(((Bits *)&w->data[0x343a])->wireless==1&&PTR(w,0xe4,Args)->enable==0)((Bits *)&w->data[0x343a])->selected=0;
 if((u8)(PTR(w,0xe8,Args)->kind+0xf9)<=1){mask=1;for(i=0;i<16;i++){if(PTR(w,0xe8,Args)->flags&mask)w->data[0x3424+i]=1;else w->data[0x3424+i]=0;mask<<=1;}}
 else{for(i=0;i<16;i++)w->data[0x3424+i]=0;}
 PTR(w,0,void)=BgConfig_Alloc(25);ov51_021E5F64(w);ov51_021E60D4();ov51_021E6238(PTR(w,0,void));ov51_021E6354(w);
 sub_020210BC();sub_02021148(4);TextFlags_SetCanTouchSpeedUpPrint(1);TextFlags_SetCanABSpeedUpPrint(1);Sound_SetSceneAndPlayBGM(0x38,0,0);
 ov51_021E7DA4(&w->data[0x33a0]);PlaySE(1684);ov51_021E78F8(&w->data[0x311c]);ov51_021E7AF4(&w->data[0x311c],&w->data[0x3424],((Bits *)&w->data[0x343a])->selected);ov51_021E7BD0(&w->data[0x311c]);ov51_021E6E60(w);ov51_021E7664(&w->data[0x54],1,PTR(w,0x33cc,void));
 if(PTR(w,0xe4,Args)->result)WORD(w,0x30f4)=1;else WORD(w,0x30f4)=0;PTR(w,0xe4,Args)->result=0;
 w->data[0x3434]=0;WORD(w,0x3108)=-1;WORD(w,0x339c)=0;w->data[0x3438]=0;ov51_021E6C00(0);ov51_021E6734(w);ov51_021E76A4(w,0);Main_SetVBlankIntrCB(ov51_021E6B88,w);sub_0203A964();GF_SndHandleSetPlayerVolume(1,0x2a);BeginNormalPaletteFade(2,3,3,0,6,1,25);
 if(PTR(w,0xe8,Args)->kind!=7&&PTR(w,0xe8,Args)->kind!=8)GfGfx_EngineATogglePlanes(4,0);return 1;
}
