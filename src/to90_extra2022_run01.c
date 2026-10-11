#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct {u32 a,b;} Pair;
typedef struct {u32 word;} Result;
int WiFiPlazaProfile_IsChecksumValid(void*,void*),ov45_0222EC68(int),ov45_0222AA84(void*),ov45_0222AAA8(void*),ov45_0222AA28(void*),ov45_0222EA78(int),ov45_0222A9CC(void*),ov45_0222CBD0(void*,int,int),ov45_0222F484(int),ov45_0222F4AC(int);
Result ov45_02230F94(u16,u8,u32,u32);
void ov45_0222BCE4(void*,void*,u32),ov45_0222D8C8(void*,int,int,int),ov45_0222EA4C(int,Pair*),ov45_0222D500(void*,Pair*),WiFiPlazaProfile_UpdateChecksum(void*,void*),ov45_0222BA3C(void*),ov45_0222D8F0(void*,int),ov45_0222C944(void*,int,int),ov45_0222DC08(void*,int*,void*),ov45_0222C9A0(void*,int,int,int);
void ov45_0222B2B4(int,void*,void*,int);
void ov45_0222B2B4(int id,void*arg,void*w,int mode){Pair p;int a,b,index,copy,state;
 if(mode){ov45_0222BCE4((u8*)w+0x508,(u8*)w+0x108,W(w,0x528));if(!WiFiPlazaProfile_IsChecksumValid((u8*)w+0x108,P(w,0)))W(w,0x52c)=1;
 ov45_0222D8C8(P(w,4),ov45_0222EC68(id),B(w,0x160),3);W(w,0x128)=id;ov45_0222EA4C(id,&p);ov45_0222D500((u8*)w+0x140,&p);
 a=ov45_0222AA84((u8*)w+0x128);b=ov45_0222AAA8((u8*)w+0x128);state=ov45_0222AA28((u8*)w+0x128);if(a && state==1){*(Result*)((u8*)w+0x144)=ov45_02230F94((u16)a,(u8)b,W(w,0x140),W(w,0x528));}else{B(w,0x144)=12;B(w,0x145)=0;B(w,0x146)=0;}
 WiFiPlazaProfile_UpdateChecksum((u8*)w+0x108,P(w,0));ov45_0222BA3C(w);
 }else {a=ov45_0222EA78(id);b=ov45_0222EC68(id);ov45_0222D8C8(P(w,4),b,ov45_0222A9CC(arg),0);if(a==id)ov45_0222D8F0(P(w,4),b);if(ov45_0222CBD0((u8*)w+0x4bc,id,-1))ov45_0222BA3C(w);}
 index=ov45_0222EC68(id);W(w,0xf8)|=1<<index;
 if(ov45_0222F484(id)==1){ov45_0222C944((u8*)w+0x3cc,index,1);copy=index;ov45_0222DC08(P(w,4),&copy,(u8*)w+0x3cc);a=ov45_0222F4AC(id);if(a)ov45_0222C9A0((u8*)w+0x3e4,index,1,a);else ov45_0222C9A0((u8*)w+0x3e4,index,0,a);
 }else {ov45_0222C944((u8*)w+0x3cc,index,0);ov45_0222C9A0((u8*)w+0x3e4,index,0,0);}
}
