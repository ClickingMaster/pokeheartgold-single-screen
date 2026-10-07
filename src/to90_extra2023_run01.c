#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
typedef u64 Pair;
typedef struct {Pair value;u8 pad[16];} Packet;
typedef struct {void*a,*b;u32 c;u8 padc[0xc8];u32 d4;Pair pos,prev;} Work;
void ov45_022320C4(Packet*),ov45_0222D500(void*,Packet*),ov45_0222D8BC(void*,void*),ov45_0222CA7C(void*,int,int),ov45_0222C370(void*,int);
int ov45_0222ECDC(int);
void ov45_0222B5A0(u32,void*);
void ov45_0222B5A0(u32 id,void*w){Packet p;int flag=0,which;ov45_022320C4(&p);ov45_0222D500((u8*)w+0xd4,&p);((Work*)w)->pos=p.value;
 switch(id){
 case 0:ov45_0222D8BC(P(w,4),(u8*)w+0xd4);((Work*)w)->prev=((Work*)w)->pos;B(w,0x1fc)=(B(w,0x1fc)&~1)|1;W(w,0x100)|=1;break;
 case 2:case 3:case 4:case 5:case 6:if(!W(w,8)){W(w,8)=1;flag=1;which=0;H(w,0x202)=300;}
 case 1:B(w,0x1fd)=id;W(w,0x100)|=2;break;
 case 8:case 9:case 10:if(!W(w,8)){W(w,8)=1;flag=1;which=0;H(w,0x202)=300;}
 case 7:B(w,0x1fe)=id;W(w,0x100)|=4;break;
 case 12:case 13:case 14:if(!W(w,8)){W(w,8)=1;flag=1;which=0;H(w,0x202)=300;}
 case 11:B(w,0x1ff)=id;W(w,0x100)|=8;break;
 case 16:{int a,b;flag=1;which=1;a=ov45_0222ECDC(6);b=ov45_0222ECDC(7);ov45_0222CA7C((u8*)w+0x49c,a,b);H(w,0x204)=300;}break;
 case 17:B(w,0x1fc)=(B(w,0x1fc)&~12)|8;W(w,0x100)|=32;break;
 case 18:flag=1;which=2;H(w,0x206)=300;break;
 case 19:flag=1;which=4;B(w,0x1fc)|=128;H(w,0x200)=900;break;
 case 15:B(w,0x1fc)=(B(w,0x1fc)&~96)|32;W(w,0x100)|=16;break;
 }if(flag)ov45_0222C370(w,which);
}
