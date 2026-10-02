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

int ov38_022249A0(char *s) {int n=0,c=*s++;while(c) {if((c>=48&&c<=57)||(c>=65&&c<=90)||(c>=97&&c<=122)||c==32)n++;else n+=3;c=*s++;}return n;}
int ov38_02224A08(char *p,int c) {int hi,lo; if(c==32) {*p=43;return 1;} if((c>=48&&c<=57)||(c>=65&&c<=90)||(c>=97&&c<=122)) {*p=c;return 1;} hi=(c>>4)&15; *p=37;if(hi<10)p[1]=hi+48;else p[1]=hi+55;lo=c&15;if(lo<10)p[2]=lo+48;else p[2]=lo+55;return 3;}
