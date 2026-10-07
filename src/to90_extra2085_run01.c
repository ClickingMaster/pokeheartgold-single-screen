#pragma force_active on
#include "global.h"
#pragma force_active on
extern u32 PaletteData_GetSelectedBuffersBitmask(void*);
extern int ov12_0226BD50(void*),ov08_02222B8C(void*),ov08_02223CD4(void*,u8);
extern void ov08_02224938(void*,int,int),ov08_02222D84(void*),ov08_02222D90(void*),PlaySE(int);
int ov08_02222EC4(u8*);
#define PTR(p,o) (*(void**)((u8*)(p)+(o)))
int ov08_02222EC4(u8 *s){if(PaletteData_GetSelectedBuffersBitmask(PTR(s,8)))return 12;switch(s[0x1159]){case 0:if(ov12_0226BD50(PTR(s,0x38))==1){PlaySE(1501);s[0x114d]=2;s[0x114b]=12;ov08_02224938(s,2,0);s[0x115a]=0;s[0x1159]++;return 11;}s[0x115a]++;break;case 1:ov08_02222D84(s);s[0x1159]++;break;case 2:if(ov12_0226BD50(PTR(s,0x38))==1){PlaySE(1501);*((u8*)PTR(s,0)+s[0x114d]+0x27)=0;s[0x114b]=12;ov08_02224938(s,6,0);s[0x115a]=0;s[0x1159]++;return 11;}s[0x115a]++;break;case 3:ov08_02222D90(s);s[0x1159]++;break;case 4:if(ov12_0226BD50(PTR(s,0x38))==1){PlaySE(1501);*(u16*)((u8*)PTR(s,0)+0x1c)=ov08_02223CD4(s,*((u8*)PTR(s,0)+s[0x114d]+0x27));((u8*)PTR(s,0))[0x1e]=s[0x114d];ov08_02224938(s,15,0);return ov08_02222B8C(s);}s[0x115a]++;break;}return 12;}
