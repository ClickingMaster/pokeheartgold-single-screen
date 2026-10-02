#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern int SOC_InetAtoN(const char *,void *);
extern int SOC_Socket(int,int,int);
extern int SOC_Close(int);
extern int SOC_Connect(int,void *);
extern int SOC_Recv(int,void *,int,int);
extern int SOC_Send(int,const void *,int,int);
extern int SOC_Shutdown(int,int);
#define I(p,o) (*(int *)((char *)(p)+(o)))
#define P(p,o) (*(void **)((char *)(p)+(o)))
#define B(p,o) (*(char *)((char *)(p)+(o)))
typedef struct Node {struct Node *prev,*next; int id; void * volatile obj; int size;} Node;
typedef struct Chain {struct Chain *next;} Chain;
extern void (*ov38_0222621C)(void *);
extern void *(*ov38_02226218)(int,int);
extern Node *ov38_02226220,*ov38_02226230;
extern int ov38_02226214;
extern char ov38_02225EE4[],ov38_02226BF0[],ov38_02226C58[];
extern int ov38_02226C10[2];
typedef struct Pair {int v,pad;} Pair;
extern Pair ov38_02226C18[];
typedef struct PtrPair {void *v;int pad;} PtrPair;
extern PtrPair ov38_02226C1C[];
extern int STD_CompareString(const char *,const char *);
extern u32 STD_GetStringLength(const char *);
extern u32 SOCL_Resolve(char *);
extern void CPS_SslAddRandomSeed(void *,int);
extern int SOCL_EnableSsl(int,void *);
extern void ov38_02222038(void);
extern void ov38_0222204C(void);
extern int ov38_02221E2C(void *);
extern int ov38_0222211C(void);
extern int ov38_02221F38(int);
extern void ov38_02221FD8(void);
extern int ov38_02222210(void *,int,int,int *);
extern int ov38_02222350(void *,int,int,char *,int);
extern int ov38_022222E0(void *,int,int);
extern int ov38_02222420(void *,void *,int,int);
void ov38_02222B44(Node *p);
int ov38_02222B98(void *p);
int ov38_02222C10(void *p);
int ov38_02222C9C(int id);
void ov38_02222D18(void);
u32 ov38_02222D6C(void *p);
void ov38_02222E04(void *p);
int ov38_02222E7C(void *p,char *key,int *out);
int ov38_02222F88(void *p,char *key,char **out);
int ov38_0222308C(void *p,void **out);
int ov38_022230B0(int v);
void ov38_022230BC(void);
int ov38_022230DC(void *p);
int ov38_0222315C(void *p,int fd);
int ov38_0222316C(void *p,int fd);
int ov38_022231A8(void *p,int fd,u32 ip,int port);
int ov38_02223224(void *p,int fd,void *b,int n,int flag);
int ov38_02223270(void *p,int fd,void *b,int n,int flag);
void ov38_022232BC(void *p,int fd);
int ov38_022232D8(const char *a,const char *b);
int ov38_022232E4(char *a,char *b,int n);
u32 ov38_0222334C(const char *s);
void ov38_02223358(void *p,int n);
void ov38_0222336C(void *p,const void *s,int n);
int ov38_02223384(int *a,int *b);
int ov38_02223424(void *p,int fd,int *used,char *src,int n);
int ov38_022234D8(void *p,int fd);
void ov38_02223564(void *p);
void ov38_022235A0(int fd,void **p);
void ov38_02223640(void);
int ov38_022249A0(char *s);
int ov38_02224A08(char *p,int c);
int ov38_02224A98(char *p,int n);
int ov38_02224B80(char *p,int n);
int ov38_02224CD4(char *a,char *b);
int ov38_02224D58(char *p,int n);
int ov38_02224DC8(char *p,int n,char *key,int size);

typedef struct Digits {u32 v[9];} Digits;
extern Digits ov38_02225000;
void ov38_02222B44(Node *p);
int ov38_02222B98(void *p);
int ov38_02222C10(void *p);
int ov38_02222C9C(int id);
void ov38_02222D18(void);
u32 ov38_02222D6C(void *p);
void ov38_02222E04(void *p);
int ov38_02222E7C(void *p,char *key,int *out);
int ov38_02222F88(void *p,char *key,char **out);
int ov38_0222308C(void *p,void **out);
int ov38_022230B0(int v);
void ov38_022230BC(void);
int ov38_022230DC(void *p);
int ov38_0222315C(void *p,int fd);
int ov38_0222316C(void *p,int fd);
int ov38_022231A8(void *p,int fd,u32 ip,int port);
int ov38_02223224(void *p,int fd,void *b,int n,int flag);
int ov38_02223270(void *p,int fd,void *b,int n,int flag);
void ov38_022232BC(void *p,int fd);
int ov38_022232D8(const char *a,const char *b);
int ov38_022232E4(char *a,char *b,int n);
u32 ov38_0222334C(const char *s);
void ov38_02223358(void *p,int n);
void ov38_0222336C(void *p,const void *s,int n);
int ov38_02223384(int *a,int *b);
int ov38_02223424(void *p,int fd,int *used,char *src,int n);
int ov38_022234D8(void *p,int fd);
void ov38_02223564(void *p);
void ov38_022235A0(int fd,void **p);
void ov38_02223640(void);
int ov38_022249A0(char *s);
int ov38_02224A08(char *p,int c);
int ov38_02224A98(char *p,int n);
int ov38_02224B80(char *p,int n);
int ov38_02224CD4(char *a,char *b);
int ov38_02224D58(char *p,int n);
int ov38_02224DC8(char *p,int n,char *key,int size);
int ov38_02224C18(char *p,u32 v);

int ov38_02222F88(void *p,char *key,char **out) {int at[1],n; if(!I(p,8))return -1; if(P(p,0x14)) {ov38_0222621C(P(p,0x14)); P(p,0x14)=0;} n=ov38_02222E7C(p,key,at); if(n>=0) {P(p,0x14)=ov38_02226218(n+1,4); B(P(p,0x14),n)=0; ov38_02222420(p,P(p,0x14),at[0],n); *out=P(p,0x14); return n;} if(ov38_022232D8(ov38_02225EE4,key)!=0)return -1; P(p,0x14)=ov38_02226218(4,4); B(P(p,0x14),3)=0; ov38_02222420(p,P(p,0x14),9,3); *out=P(p,0x14); return 3;}
int ov38_0222308C(void *p,void **out) {if(!I(p,8)||!I(p,4))return -1; *out=P(p,0x18); return I(p,4);}
int ov38_022230B0(int v) {if(v&0x8000)v&=~0x8000; return v;}
void ov38_022230BC(void) {OS_GetLowEntropyData((u32 *)ov38_02226BF0); CPS_SslAddRandomSeed(ov38_02226BF0,32);}
int ov38_022230DC(void *p) {int fd=SOC_Socket(2,1,0); if(fd>=0 && I(p,8)) {ov38_022230BC(); I(P(p,0x20),0x814)=I(p,0x1c); I(P(p,0x20),0x818)=I(p,0x18); P(P(p,0x20),0x810)=ov38_022230B0; if(SOCL_EnableSsl(fd,P(p,0x20))<0) {SOC_Close(fd); fd=-1;}} return fd;}
int ov38_0222315C(void *p,int fd) {return SOC_Close(fd);}
int ov38_0222316C(void *p,int fd) {int n=SOC_Close(fd); if(n!=-26)return n; do {OS_Sleep(500); n=SOC_Close(fd);} while(n==-26); return n;}
int ov38_022231A8(void *p,int fd,u32 ip,int port) {struct {u8 size,family;u16 port;u32 ip;} addr; addr.size=8;addr.family=2;addr.port=((u16)port>>8&255)|((u16)port<<8&0xff00);addr.ip=ip; if(SOC_Connect(fd,&addr)<0) {if(I(p,4))return -1002; return -1001;} return 0;}
int ov38_02223224(void *p,int fd,void *b,int n,int flag) {int r=SOC_Recv(fd,b,n,flag); if(r>=0)return r; if(I(p,4))return -1002; if(r==-56)return 0; return -1001;}
int ov38_02223270(void *p,int fd,void *b,int n,int flag) {int r=SOC_Send(fd,b,n,flag); if(r>=0)return r; if(I(p,4))return -1002; if(r==-56)return 0; return -1001;}
void ov38_022232BC(void *p,int fd) {if(fd<0)return; SOC_Shutdown(fd,2);}
int ov38_022232D8(const char *a,const char *b) {return STD_CompareString(a,b);}
int ov38_022232E4(char *a,char *b,int n) {while(n>0) {int y=*b++,x=*a++; if(!x||!y) {if(!x&&!y)n=0;break;} if(y>=65&&y<=90)y+=32; if(x>=65&&x<=90)x+=32; if(x!=y)break; n--;} return n;}
u32 ov38_0222334C(const char *s) {return STD_GetStringLength(s);}
void ov38_02223358(void *p,int n) {MI_CpuFill8(p,0,n);}
void ov38_0222336C(void *p,const void *s,int n) {MI_CpuCopy8(s,p,n);}
int ov38_02223384(int *a,int *b) {int ok; *a=0;*b=0;ov38_02222038();if(ov38_02226220) {if(I(ov38_02226220->obj,0x14)) {*a=I(P(ov38_02226220->obj,0x30),4); *b=I(ov38_02226220->obj,0x14)==-1 ? 0:I(ov38_02226220->obj,0x14);} ok=1;} else ok=ov38_02226230!=0; ov38_0222204C();return ok;}
int ov38_02223424(void *p,int fd,int *used,char *src,int n) {int left=n; while(left>0) {int count; if(I(p,4))return -1; count=1024-*used;if(left<=count)count=left;ov38_0222336C(ov38_02226C58+*used,src,count);*used+=count;src+=count;left-=count;if(*used==1024) {int r=ov38_02223270(p,fd,ov38_02226C58,1024,0);if(r<=0)return r;*used-=r;}} return n;}
