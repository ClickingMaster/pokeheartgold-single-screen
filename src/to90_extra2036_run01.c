#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,n) (*(void**)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define SW(p,n) (*(s32*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
void PlaySE(u16);
int ov72_02237B54(void*);int ov72_02237B74(void);void sub_0202D7C0(void*,void*,u8,u8);void ov72_02238680(void*,int,int);void ov00_021ECB94(void*,void*);void sub_0202D678(void*,u8,u8,void*);void ov72_0223A444(void*);void sub_020399EC(void);int ov72_022398BC(void*);
int ov72_022398BC(void*s){if(ov72_02237B54(s)){int result=ov72_02237B74();W(s,0xfd4)=0;switch(result){case 0:{u32 second[3],first[4];sub_0202D7C0(P(P(s,0),4),(u8*)s+0xa0,(u8)W(s,0x90),(u8)W(s,0x9c));ov72_02238680(s,39,19);ov00_021ECB94(first,second);sub_0202D678(P(P(s,0),4),(u8)W(s,0x94),(u8)W(s,0x9c),first);}break;case -1:W(s,0xf5c)=result;W(s,0x1c)=53;ov72_0223A444(s);break;case -2:W(s,0xf5c)=result;W(s,0x1c)=53;ov72_0223A444(s);break;case -3:case -4:W(s,0xf5c)=result;W(s,0x1c)=11;ov72_0223A444(s);break;case -6:W(s,0xf5c)=result;W(s,0x1c)=53;ov72_0223A444(s);break;case -7:W(s,0xf5c)=result;W(s,0x1c)=53;ov72_0223A444(s);break;case -5:W(s,0xf5c)=result;W(s,0x1c)=53;ov72_0223A444(s);sub_020399EC();break;}}else{W(s,0xfd4)++;if(W(s,0xfd4)==3600)sub_020399EC();}return 3;}
