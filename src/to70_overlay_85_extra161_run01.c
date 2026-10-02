#pragma force_active on
#include "global.h"
#pragma force_active on
u8 *OverlayManager_GetData(void *);
void OverlayManager_FreeData(void *);
int sub_02021238(void);
void Main_SetVBlankIntrCB(void (*)(void *),void *);
void GF_DestroyVramTransferManager(void);
void NARC_Delete(void *);
void Heap_Destroy(int);
void sub_0201A738(int);
void ov85_021E7D40(void *),ov85_021E7E78(void *),ov85_021E7FB0(void *),ov85_021E8118(void *),ov85_021E833C(void *),ov85_021E8418(void *),ov85_021E75B8(void *),ov85_021E7658(void *),ov85_021E67CC(void *);
void ov85_021E74AC(void *),ov85_021E75C8(void *),ov85_021E7798(void *),ov85_021E67F4(void *),ov85_021E5AF0(void *);
extern int (*const ov85_021EA800[])(u8 *);
int ov85_021E5A34(void *),ov85_021E5AAC(void *);
u8 *OverlayManager_GetArgs(void *);
void sub_020398D4(int,int),HBlankInterruptDisable(void),sub_0201A728(int),Heap_Create(int,int,u32);
u8 *OverlayManager_CreateAndGetData(void *,u32,int);
void *SaveArray_Party_Get(void *),*NARC_New(int,int);
void GF_CreateVramTransferManager(int,int),sub_020210BC(void),sub_02021148(int);
void ov85_021E678C(void *),ov85_021E6764(void *),ov85_021E752C(void *),ov85_021E7650(void *);
int sub_0203769C(void),GF_GetCurrentPlayingBGM(void);
void ov85_021E7D08(void *),ov85_021E7E3C(void *),ov85_021E7F74(void *),ov85_021E80E0(void *),ov85_021E82F8(void *),ov85_021E83E0(void *);
void BeginNormalPaletteFade(int,int,int,int,int,int,int);
extern const u16 ov85_021EA788[][5];
int ov85_021E5900(void *);
#define W(p,o) (*(int *)((p)+(o)))
#define P(p,o) (*(void **)((p)+(o)))
int ov85_021E5900(void *man){
 u8 *args=OverlayManager_GetArgs(man);
 u8 *w;
 int i,count,me;
 u8 *pos;
 sub_020398D4(1,1);Main_SetVBlankIntrCB(NULL,NULL);HBlankInterruptDisable();sub_0201A728(2);
 Heap_Create(3,102,0x80000);
 w=OverlayManager_CreateAndGetData(man,0xdcc,102);memset(w,0,0xdcc);
 P(args,0x34)=w;P(w,0xcc)=args;P(w,0xd0)=P(args,0x30);
 P(w,0x24)=SaveArray_Party_Get(P((u8 *)P(w,0xcc),0x1c));
 P(w,0xd80)=NARC_New(187,102);GF_CreateVramTransferManager(8,102);sub_020210BC();sub_02021148(4);
 ov85_021E678C(w);Main_SetVBlankIntrCB(ov85_021E6764,w);ov85_021E752C(w);ov85_021E7650(w);
 i=0;count=0;me=sub_0203769C();
 pos=w+0xd4;
 args=P(w,0xcc);
 for(;i<5;i++)if(W(args,12)&(1<<i)){if(i==me)break;count++;}
 W(pos,0x40)=ov85_021EA788[W(args,8)][count]<<12;
 W(w,28)=GF_GetCurrentPlayingBGM();
 ov85_021E7D08(w);ov85_021E7E3C(w);ov85_021E7F74(w);ov85_021E80E0(w);ov85_021E82F8(w);ov85_021E83E0(w);
 BeginNormalPaletteFade(0,1,1,0,8,1,102);return 1;
}
int ov85_021E5A34(void *man){
 u8 *w=OverlayManager_GetData(man);
 GF_ASSERT(sub_02021238()==1);
 ov85_021E7D40(w);ov85_021E7E78(w);ov85_021E7FB0(w);ov85_021E8118(w);ov85_021E833C(w);ov85_021E8418(w);
 ov85_021E75B8(w);ov85_021E7658(w);ov85_021E67CC(w);
 Main_SetVBlankIntrCB(NULL,NULL);GF_DestroyVramTransferManager();
 NARC_Delete(*(void **)(w+0xd80));OverlayManager_FreeData(man);Heap_Destroy(102);sub_0201A738(2);return 1;
}
int ov85_021E5AAC(void *man){
 u8 *w=OverlayManager_GetData(man);
 int ret;
 ov85_021E74AC(w);
 do{ret=ov85_021EA800[*(int *)w](w);}while(ret==1);
 if(ret==2)return 1;
 ov85_021E75C8(w);ov85_021E7798(w);ov85_021E67F4(w);ov85_021E5AF0(w);return 0;
}
