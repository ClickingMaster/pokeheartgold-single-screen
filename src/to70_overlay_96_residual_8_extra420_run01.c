#pragma force_active on
#include "global.h"
#pragma force_active on
int ov96_021E5F24(void *);u32 PokeathlonCourse_GetParticipantCount(void *);void ov96_021E8340(int,u8,void *),ov96_021E8228(u8 *,u32,u32,int,int);
void ov96_021E8228(u8 *s,u32 index,u32 mon,int value,int flag){if(value==0){if(index!=ov96_021E5F24(s))GF_AssertFail();if(flag!=1)GF_AssertFail();if(*(u32 *)(s+2224)<9999)(*(u32 *)(s+2224))++;}else{if(mon>=3)GF_AssertFail();if(*(int *)(*(u8 **)(s+480)+16)==0){if(ov96_021E5F24(s)!=0)GF_AssertFail();ov96_021E8340(value,(u8)flag,s+1836+index*96+mon*32);}else if(ov96_021E5F24(s)==0){if(index>=PokeathlonCourse_GetParticipantCount(s))ov96_021E8340(value,(u8)flag,s+1836+index*96+mon*32);else{if(index!=ov96_021E5F24(s))GF_AssertFail();ov96_021E8340(value,(u8)flag,s+2884+mon*32);}}else{if(index!=ov96_021E5F24(s))GF_AssertFail();ov96_021E8340(value,(u8)flag,s+2884+mon*32);}}}
