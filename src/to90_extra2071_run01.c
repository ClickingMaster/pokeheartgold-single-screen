#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,n) (*(void**)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define SH(p,n) (*(s16*)((u8*)(p)+(n)))
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
void PlaySE(u16);
#include "system.h"
void PokewalkerIr_GetChunkProgress(void*);int PokewalkerIr_GetTotalProgress(void);u32 PokewalkerApp_UpdateLinkProgress(void*,u8);void PokewalkerApp_SpawnRisingParticle(void*);void ov112_021EA60C(void*);void PokewalkerApp_PlayTransferSE(u32);void ov112_021EDAF4(void*,u32);void ov112_021EAA98(void*,void*,int);void ov112_021EAA10(void*);u32 PokewalkerIr_GetAndClearError(void);void ManagedSprite_SetDrawFlag(void*,int);void sub_02032674(void*,void*,void*);void ov112_021ED590(void*);void sub_02032644(void*);void PokewalkerApp_HideSprite(void*,int);void PokewalkerIr_Stop(void);int ov112_021E7668(void);void PokewalkerApp_ShowSubBg1(void*,int);int PokewalkerApp_GetTouchedButton(int);void PokewalkerApp_SelectMenuItem(void*,int,int,int);void PokewalkerApp_ShowMessageBox(void*,int,int);int ov112_021EDB24(void*);
typedef struct{u8 pad[0x1f2c0];u8 window[20];u16 marker;} MarkerWork;
int ov112_021EDB24(void*s){u32 value,mode;PokewalkerIr_GetChunkProgress(s);value=PokewalkerApp_UpdateLinkProgress(s,(u8)PokewalkerIr_GetTotalProgress());if(value)PokewalkerApp_SpawnRisingParticle(s);ov112_021EA60C(s);PokewalkerApp_PlayTransferSE(value);ov112_021EDAF4(s,value);if(value>200){if(((MarkerWork*)s)->marker==0){ov112_021EAA98(s,((MarkerWork*)s)->window,2);((MarkerWork*)s)->marker=1;}}else{ov112_021EAA10(s);((MarkerWork*)s)->marker=0;}mode=PokewalkerIr_GetAndClearError();switch(mode){case 2:if(H(s,22)==1)H(s,20)=42;break;case 4:case 5:case 7:case 8:case 9:case 11:case 12:case 13:H(s,20)=mode;break;case 15:ManagedSprite_SetDrawFlag(P(s,0x1e538),0);sub_02032674(P(s,0x1e440),(u8*)s+0x1d758,(u8*)s+0x1d75c);ov112_021ED590(s);sub_02032644(P(s,0x1e440));PokewalkerApp_HideSprite(s,8);ov112_021EAA98(s,((MarkerWork*)s)->window,0);return 35;}if(H(s,20)){ManagedSprite_SetDrawFlag(P(s,0x1e538),0);W(s,12)=59;PokewalkerIr_Stop();return 32;}if(!ov112_021E7668()){PokewalkerApp_ShowSubBg1(s,1);if(!PokewalkerApp_GetTouchedButton(5)||(gSystem.newKeys&1)||(gSystem.newKeys&2)){H(s,20)=33;W(s,12)=66;PokewalkerIr_Stop();PokewalkerApp_SelectMenuItem(s,3,3,33);PlaySE(1501);return 65;}}else{if(H(s,22)==0){PokewalkerApp_ShowMessageBox(s,2,6);H(s,22)=1;}PokewalkerApp_ShowSubBg1(s,0);}return 31;}
