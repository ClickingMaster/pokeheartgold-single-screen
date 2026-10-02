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

void ov38_02222B44(Node *p) {while(p) {Node *prev=p->prev; if(p!=prev) {Node *next=prev->prev; ov38_0222621C(prev); p->prev=next;} else {ov38_0222621C(p); p=0;}}}
int ov38_02222B98(void *p) {ov38_02222B44(P(p,0x34)); ov38_02222B44(P(p,0x38)); if(P(p,0x20)) {if(P(P(p,0x20),0x800))ov38_0222621C(P(P(p,0x20),0x800)); ov38_0222621C(P(p,0x20));} ov38_0222621C(P(p,0x24)); ov38_0222621C(p); return 1;}
int ov38_02222C10(void *p) {int n; if(I(p,0)) {ov38_02226214=11; return -1;} if(I(p,0x40)==1 && !P(p,0x38)) {ov38_02226214=11; return -1;} ov38_02222038(); n=ov38_02221E2C(p); if(n>=0) {I(p,0)=1; ov38_0222211C();} else ov38_02226214=1; ov38_0222204C(); return n;}
int ov38_02222C9C(int id) {int ok=0; ov38_02222038(); if(ov38_02226220 && ov38_02226220->id==id && I(ov38_02226220->obj,4)==0) {P(ov38_02226220->obj,4)=(void *)1; ov38_022232BC(ov38_02226220->obj,ov38_02226220->size); ok=1;} if(!ok)ok=ov38_02221F38(id); ov38_0222204C(); return ok;}
void ov38_02222D18(void) {ov38_02222038(); if(ov38_02226220 && !I(ov38_02226220->obj,4)) {P(ov38_02226220->obj,4)=(void *)1; ov38_022232BC(ov38_02226220->obj,ov38_02226220->size);} ov38_02221FD8(); ov38_0222204C();}
u32 ov38_02222D6C(void *p) {char old=B(P(p,0x24),I(p,0xc)); char *s; int skip; struct {u32 ip;} addr; u32 ip; B(P(p,0x24),I(p,0xc))=0; s=P(p,0x24); skip=I(p,8)?8:7; if(SOC_InetAtoN(s+skip,&addr.ip))ip=addr.ip; else {ip=SOCL_Resolve(s+skip); if(!ip)return 0; ip=(ip>>24&255)|(ip>>8&0xff00)|(ip<<8&0xff0000)|(ip<<24&0xff000000);} B(P(p,0x24),I(p,0xc))=old; return ip;}
