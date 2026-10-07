#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
void*PokeathlonCourse_GetHeapAllocPtr4(void*);int IsPaletteFadeFinished(void),ov96_021E5F24(void*),PokeathlonCourse_GetParticipantCount(void*);void PokeathlonCourse_SetField5E0_AtIndex(void*,u8,u16);void*ov96_021E60D8(void*,int,int);u16 LCRandom(void);void ov96_021E8228(void*,u8,u8,int,int);
static inline int Sum(int a,int b){return a+b;}
int ov96_021F5730(void*,u8*);
int ov96_021F5730(void*p,u8*state){u8*record=PokeathlonCourse_GetHeapAllocPtr4(p);int owner,i,score,participant,j,total;u8 part,random;u8*info;if(*state==0){if(IsPaletteFadeFinished())*state=1;return 0;}owner=ov96_021E5F24(p);score=0;for(i=0;i<3;i++){score+=H(record,0xb0);record+=0x38;}if(score>999)score=999;PokeathlonCourse_SetField5E0_AtIndex(p,(u8)owner,(u16)score);if(owner==0){for(participant=PokeathlonCourse_GetParticipantCount(p);participant<4;participant++){j=0;total=j;part=(u8)participant;for(;j<3;j++){info=ov96_021E60D8(p,participant,j);if(B(info,2)){int r=LCRandom()%21;total+=Sum(B(info,3)*2+(B(info,2)*6+80+B(info,4)*2),r);LCRandom();}ov96_021E8228(p,part,(u8)j,2,0);random=(u8)(LCRandom()%21);ov96_021E8228(p,part,(u8)j,4,random);ov96_021E8228(p,part,(u8)j,1,random);}PokeathlonCourse_SetField5E0_AtIndex(p,(u8)participant,(u16)total);}}return 1;}
