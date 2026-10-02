#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define W(p,n) (*(int *)((u8 *)(p)+(n)))
#define P(p,n) (*(void **)((u8 *)(p)+(n)))
extern char ov38_022257B8[],ov38_02225644[],ov38_02225600[],ov38_02225878[],ov38_02225C4C[],ov38_02225B74[],ov38_02225AE0[],ov38_02225CA8[],ov38_02225B00[],ov38_02225CAC[];
extern u8 ov38_02226050[];
extern char *ov38_02226028;
extern void *ov38_02226078[3];
extern int ov38_0222606C,ov38_02226074;
extern void (*ov38_02226070)(void *);
extern void *(*ov38_0222605C)(int,int);
void __msl_assertion_failed(char*,char*,char*,int);
int ov38_0221CD90(void*,void*,int);
int ghiPostDoPosting(void*);
int ghiParseURL(void*);
void ghiCallProgressCallback(void*,int,int);
void ghiCallPostCallback(void*);
void ov38_0221E0DC(void*);
void ov00_021FF3CC(void);
int ov00_021FFA44(char*);
void *SOC_GetHostByName(char*);
int ov00_021FF59C(int,int*,int,int);
int ov00_021FFA60(int);
int sscanf(const char *,const char *,...);
void ov38_02221734(void);void ov38_02221748(void);void ov38_02221730(void);void ov38_02221784(void);void ov38_022217D0(void);
int ov38_02221788(void);int ov38_02222C9C(int);void ov38_02221AC4(void(*)(void));
int ov38_02221870(char*);void ov38_0222189C(void*,void*,int);
int ov38_022201AC(int);int ov38_022201C8(char*,char*,int);int ov38_0221FFC0(int,char*);void ov38_0221FF7C(void);
extern char ov38_02225B68[],ov38_02225B48[],ov38_02225CB0[],ov38_02225CB8[];
int ghiAppendDataToBuffer(void*,void*,int);
extern char ov38_02225B9C[],ov38_02225BB0[],ov38_02225B14[],ov38_02225BE8[],ov38_02224E78[];
int ov38_0221C748(void*);void ghiResetBuffer(void*);int ov38_0221CB7C(void*,void*,int*);int ghiParseStatus(void*);
unsigned ov00_021FF368(void);void *ov00_021EC3A8(int);void ov00_021EC3D8(void*);int ghiProcessIncomingFileData(void*,void*,int);
int ov38_02220100(char*,char*,int);int ov38_02220148(unsigned);
int strncmp(const char*,const char*,size_t);char *strstr(const char*,const char*);
int ghiPostFileMemoryStateDoPosting(void*s,void*c);
void ov38_0221EBB0(void*c);
void ghiDoPosting(void*c);
void ov38_0221F330(void*c);
int ghiParseChunkSize(void*c);
void ov38_0221FF7C(void);
int ov38_0221FFC0(int i,char*s);
int ov38_02220090(char*a,char*b,char*c);
int ov38_02220100(char*d,char*s,int n);
int ov38_02220148(unsigned x);
int ov38_022201A0(int x);
int ov38_022201AC(int x);
char *ov38_022203E8(char*s);
void ov38_022205DC(void);
void ov38_02220620(void(*f)(void));
int ov38_02220648(void);
int ov38_0221F624(void*c,void*p,int n);
void ghiAppendToChunkHeaderBuffer(void*c,void*p,int n);
void ghiDoSecuringSession(void*c);
void ov38_0221F4FC(void*c);
void ov38_0221FEA8(void*c);
int ov38_022201C8(char*d,char*s,int n);
int ov38_022202C0(char*d,char*s,int n);
void ov38_0221F4FC(void*c){char buf[1024],*end;int n=1024,r=ov38_0221CB7C(c,buf,&n);if(r==3)return;if(r==1&&W(c,0x84)==W(c,0x80))return;if(r==0){if(!ghiAppendDataToBuffer((u8*)c+0x74,buf,n))return;}end=strstr(P(c,0x78),ov38_02225BE8);if(end){*end=0;r=end-(char*)P(c,0x78);W(c,0xf8)=r+1;if(!ghiParseStatus(c))return;W(c,0x84)=r+2;W(c,0x10)=7;ghiCallProgressCallback(c,0,0);return;}if(r!=2)return;W(c,0xfc)=1;W(c,0x38)=7;W(c,0x4c)=ov00_021FFA60(W(c,0x48));}

int ov38_0221F624(void*c,void*p,int n){void*data=0;int len=0;W(c,0x100)+=n;if(W(c,0x100)==W(c,0x104)||W(c,0x130))W(c,0xfc)=1;if(W(c,12)==0){if(!ghiAppendDataToBuffer((u8*)c+0xbc,p,n))return 0;data=P(c,0xc0);len=W(c,0xc8);}else if(W(c,12)==1){if(n!=0){W(c,0xfc)=1;W(c,0x38)=13;return 0;}data=p;len=n;}else if(W(c,12)==2){data=p;len=n;}ghiCallProgressCallback(c,(int)data,len);return 1;}

int ghiParseChunkSize(void*c){int x;if(!W(c,0x120))__msl_assertion_failed(ov38_02225CA8,ov38_02225B74,ov38_02225B00,0x321);if(sscanf((char*)c+0x114,ov38_02225CAC,&x)!=1)return -1;return x;}

void ghiAppendToChunkHeaderBuffer(void*c,void*p,int n){int len;if(!c)__msl_assertion_failed(ov38_02225B68,ov38_02225B74,ov38_02225B48,0x333);if(!p)__msl_assertion_failed(ov38_02225CB0,ov38_02225B74,ov38_02225B48,0x334);if(n<0)__msl_assertion_failed(ov38_02225CB8,ov38_02225B74,ov38_02225B48,0x335);if(!n)return;if(W(c,0x120)>=10)return;len=10-W(c,0x120);if(len>=n)len=n;memcpy((u8*)c+0x114+W(c,0x120),p,len);W(c,0x120)+=len;((u8*)c+W(c,0x120))[0x114]=0;}
