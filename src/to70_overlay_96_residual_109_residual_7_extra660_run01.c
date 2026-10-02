#pragma force_active on
#include "global.h"
typedef struct {u16 id,value;} Entry;
extern int ov96_0221454C(const void*,const void*,void*);extern void MATH_QSort(void*,u32,u32,int(*)(const void*,const void*,void*),void*);extern void ov96_021EABA8(void*,int);void ov96_0221457C(void*,u8*,int);
void ov96_0221457C(void*w,u8*data,int omitted){Entry entries[9];u8 i,n=0;i=n;for(;i<12;i++){if(i/3!=omitted){entries[n].id=i;entries[n].value=data[i+12];n++;}}MATH_QSort(entries,9,4,ov96_0221454C,*(void**)((u8*)w+0x81c));{u8 j;for(j=0;j<9;j++){u8 id=entries[j].id;int bank=id/3;int slot=id%3;ov96_021EABA8(*(void**)((u8*)w+bank*0x174+slot*0x7c+0x5c),j+16);}}}
