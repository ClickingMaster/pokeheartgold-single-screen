#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
#define W(p,o) (*(int *)((char *)(p)+(o)))
#define U(p,o) (*(u32 *)((char *)(p)+(o)))
#define P(p,o) (*(void **)((char *)(p)+(o)))
#define B(p,o) (*(char *)((char *)(p)+(o)))
#define A(p,o) ((void *)((char *)(p)+(o)))
extern void __msl_assertion_failed(char*,char*,char*,int);
extern void *ov00_021EC3BC(void*,int),*ov00_021EC3A8(int);
extern void ov00_021EC3D8(void*),ov00_021EC2EC(int,void*,int),ov00_021FA0D8(void),ov00_021EC24C(int,int);
extern int ov00_021EC230(void);
extern int ov45_0225478C(int,int,int,int,int,int);
extern int ov45_02253528(void*,char*);
extern void ov45_02251DB4(void*,int);
extern char ov45_02257148[],ov45_02257154[],ov45_02257164[],ov45_02257184[],ov45_022571A4[],ov45_022571B4[],ov45_022571C4[],ov45_02257220[],ov45_02257248[],ov45_02257258[],ov45_02257334[],ov45_02257344[],ov45_022573C4[],ov45_022573E4[],ov45_02257408[],ov45_02257438[],ov45_0225745C[],ov45_0225749C[],ov45_022574BC[],ov45_022574E0[],ov45_02257514[],ov45_02257538[],ov45_022575D8[],ov45_022575FC[],ov45_02257600[],ov45_02257610[],ov45_02257620[],ov45_02257624[],ov45_02257628[],ov45_0225762C[],ov45_0225763C[],ov45_0225765C[],ov45_022576A0[],ov45_022576CC[],ov45_022576D4[],ov45_022576DC[],ov45_02257704[],ov45_02257728[],ov45_02257750[],ov45_02257758[],ov45_0225775C[],ov45_02257764[],ov45_02257780[],ov45_022577AC[],ov45_022577F8[],ov45_022587F8[];
extern int ov45_02253968(void*);
extern int ov45_02253374(char *s,void *p);
extern int ov45_022533E4(char *s,void *p);
extern u32 ov45_02253D50(u32 n);
extern void ov45_02253D88(u32 n);
extern int ov45_02253D9C(void);
extern int ov45_02253DDC(char *a,char *b,u32 c,u32 d,u32 e,u32 f);
extern void ov45_022545BC(void);
extern void ov45_022546F8(void);
extern int ov45_02254774(int *p);
extern int ov45_022549E0(int a,int b,int c,int d,int e);
extern int ov45_02254A04(int a,int b,int c,int d,int e);
extern void *ov45_02253B5C(void *p);
extern void ov45_02254088(void);
extern void *ov00_021EC2A8(int,int);
extern void ov00_021FFC18(void*,void*,int,int),ov00_021FA0B4(int);
extern unsigned long strtoul(const char*,char**,int);
extern int ov45_022542F4(u8 *out,u8 *a,int na,u8 *b,int nb);
extern void ov45_0225443C(int mode,char *s);
extern int ov45_02253968(void *p);
extern void ov45_0225408C(void);
extern int ov00_021FA488(void*,int,int,void(*)(void),void(*)(void),int),ov00_021FA118(void);
extern int ov45_022545F0(void);
int ov45_02253968(void *p){char *s=P(p,0x108),*end,*last;char old;int i;if(!*s)goto fail;while(*s==13||*s==10)s++;if(!*s)goto fail;end=s;last=s;while(*end!=10&&*end!=13&&*end){if(*end!=32)last=end;end++;}if(!*end)goto fail;old=last[1];last[1]=0;if(P(p,808)){ov00_021EC3D8(P(p,808));P(p,808)=0;}if(P(p,812)){ov00_021EC3D8(P(p,812));P(p,812)=0;}if(P(p,816)){ov00_021EC3D8(P(p,816));P(p,816)=0;}if(P(p,820)){ov00_021EC3D8(P(p,820));P(p,820)=0;}if(P(p,824)){ov00_021EC3D8(P(p,824));P(p,824)=0;}if(P(p,828)){ov00_021EC3D8(P(p,828));P(p,828)=0;}if(P(p,832)){ov00_021EC3D8(P(p,832));P(p,832)=0;}if(P(p,836)){ov00_021EC3D8(P(p,836));P(p,836)=0;}for(i=0;i<W(p,0x34c);i++){if(((void**)P(p,0x348))[i]){ov00_021EC3D8(((void**)P(p,0x348))[i]);((void**)P(p,0x348))[i]=0;}}if(P(p,0x348)){ov00_021EC3D8(P(p,0x348));P(p,0x348)=0;}W(p,0x34c)=0;memset(A(p,0x328),0,40);if(!ov45_02253528(p,s)){memset(A(p,0x328),0,40);return 0;}last[1]=old;ov45_02251DB4(A(p,0x108),end-(char*)P(p,0x108));return 1;fail:return 0;}

void *ov45_02253B5C(void *p){if(!(p))__msl_assertion_failed(ov45_02257334,ov45_02257258,ov45_02257164,1001);if(!(U(p,4)<=2))__msl_assertion_failed(ov45_02257344,ov45_02257258,ov45_02257164,1001);if(!(A(p,264)))__msl_assertion_failed(ov45_022573C4,ov45_02257258,ov45_02257164,1001);if(!(W(p,272)>=0))__msl_assertion_failed(ov45_022573E4,ov45_02257258,ov45_02257164,1001);if(!(W(p,272)%8192==0))__msl_assertion_failed(ov45_02257408,ov45_02257258,ov45_02257164,1001);if(!(W(p,268)>=0))__msl_assertion_failed(ov45_02257438,ov45_02257258,ov45_02257164,1001);if(!(W(p,268)<=W(p,272)))__msl_assertion_failed(ov45_0225745C,ov45_02257258,ov45_02257164,1001);if(!(A(p,276)))__msl_assertion_failed(ov45_0225749C,ov45_02257258,ov45_02257164,1001);if(!(W(p,284)>=0))__msl_assertion_failed(ov45_022574BC,ov45_02257258,ov45_02257164,1001);if(!(W(p,284)%8192==0))__msl_assertion_failed(ov45_022574E0,ov45_02257258,ov45_02257164,1001);if(!(W(p,280)>=0))__msl_assertion_failed(ov45_02257514,ov45_02257258,ov45_02257164,1001);if(!(W(p,280)<=W(p,284)))__msl_assertion_failed(ov45_02257538,ov45_02257258,ov45_02257164,1001);if(!W(p,0x10c))return 0;if(ov45_02253968(p))return A(p,0x328);return 0;}

u32 ov45_02253D50(u32 n){return 4*(n/3+(n%3!=0));}

void ov45_02253D88(u32 n){U(ov45_022587F8,0)=n|(n<<16);}

int ov45_02253D9C(void){U(ov45_022587F8,0)=(U(ov45_0225762C,68)*U(ov45_022587F8,0)+U(ov45_0225762C,72))%U(ov45_0225762C,76);return (W(ov45_022587F8,0)>>16)&255;}

int ov45_02253DDC(char *a,char *b,u32 c,u32 d,u32 e,u32 f){int i,n;u32 count;u8 *q;W(ov45_0225762C,12)=0;if(strlen(a)>=32)return 0;if(strlen(b)!=20)return 0;if((c-5)&7)return 0;if(!(d&1))return 0;n=0;for(i=0;i<32;i++)if(((e>>i)&1)==1)n++;if(n!=1)return 0;strncpy(ov45_0225763C,a,32);count=10;q=(u8*)ov45_0225765C;{do{u8 x=((u8*)b)[0],y=((u8*)b)[1];b+=2;q[0]=x;q[1]=y;q+=2;}while(--count!=0);}U(ov45_0225762C,68)=c;U(ov45_0225762C,72)=d;U(ov45_0225762C,76)=e;U(ov45_0225762C,80)=f;W(ov45_0225762C,12)=1;return 1;}
