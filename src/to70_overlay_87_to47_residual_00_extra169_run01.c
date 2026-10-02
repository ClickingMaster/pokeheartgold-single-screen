#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{void*manager;u8 p4[5];u8 mode;u8 pA[4];u8 count;u8 pF[0x58-15];void*bg;u8 p5C[0x164-0x5c];void*options,*save,*sprites;u8 p170[0x374-0x170];void*flags;u16*results,*other;u8 p380[0x39d-0x380];u8 token;};struct Args{void*save;u8 mode;u8 pad[3];u16 results[3],other[3];u32 flags;};void*OverlayManager_GetData(void*);void SpriteList_RenderAndAnimateSprites(void*);int ScratchOffCards_Main(void*,int*);void ov87_021E725C(struct S*,int*,int);void ov87_021E6760(struct S*);void ov87_021E6668(struct S*);void ov87_021E66B8(struct S*);
int ov87_021E5AFC(struct S*);
int ov87_021E5B48(struct S*);
int ov87_021E5C38(struct S*);
int ov87_021E5CEC(struct S*);
int ov87_021E5E00(struct S*);
int ov87_021E6080(struct S*);
int ov87_021E64F8(struct S*);
int ov87_021E65FC(struct S*);
void ov87_021E68A4(void);void Heap_Create(int,int,u32);void*OverlayManager_CreateAndGetData(void*,u32,int);void*OverlayManager_GetArgs(void*);void*memset(void*,int,u32);void*BgConfig_Alloc(int);void*Save_PlayerData_GetOptionsAddr(void*);void ov87_021E7334(struct S*);void ov87_021E68DC(struct S*);void Sound_SetSceneAndPlayBGM(int,int,int);int ScratchOffCards_Init(void*,int*);
int ScratchOffCards_Init(void*m,int*state){struct S*s;struct Args*args;int i;ov87_021E68A4();Heap_Create(3,122,0x48000);s=OverlayManager_CreateAndGetData(m,0x99fc,122);memset(s,0,0x99fc);s->bg=BgConfig_Alloc(122);s->manager=m;args=OverlayManager_GetArgs(m);s->save=args->save;s->mode=args->mode;s->flags=&args->flags;s->options=Save_PlayerData_GetOptionsAddr(s->save);s->results=args->results;s->other=args->other;ov87_021E7334(s);for(i=0;i<3;i++){s->results[i]=0;s->other[i]=0;}s->token=0;ov87_021E68DC(s);*state=0;Sound_SetSceneAndPlayBGM(66,0,0);return 1;}
int ScratchOffCards_Main(void*manager,int*state){struct S*s=OverlayManager_GetData(manager);switch(*state){case 0:if(ov87_021E5AFC(s)==1)ov87_021E725C(s,state,1);break;case 1:if(ov87_021E5B48(s)==1)ov87_021E725C(s,state,2);break;case 2:if(ov87_021E5C38(s)==1)ov87_021E725C(s,state,3);break;case 3:if(ov87_021E5CEC(s)==1){if(s->count>=3){s->count=0;ov87_021E6760(s);ov87_021E6668(s);ov87_021E66B8(s);ov87_021E725C(s,state,4);}else ov87_021E725C(s,state,1);}break;case 4:if(ov87_021E5E00(s)==1)ov87_021E725C(s,state,5);break;case 5:if(ov87_021E6080(s)==1){if(s->count>=3)ov87_021E725C(s,state,7);else ov87_021E725C(s,state,6);}break;case 6:if(ov87_021E64F8(s)==1)ov87_021E725C(s,state,4);break;case 7:if(ov87_021E65FC(s)==1)return 1;break;}SpriteList_RenderAndAnimateSprites(s->sprites);return 0;}
