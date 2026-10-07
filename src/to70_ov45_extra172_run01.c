#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define W(p,o) (*(int *)((char *)(p)+(o)))
#define P(p,o) (*(void **)((char *)(p)+(o)))
#define A(p,o) ((void *)((char *)(p)+(o)))
typedef struct Node {void *left,*right;u32 parent;int key;} Node;
typedef struct {int count;void *root,*last;} Tree;
extern void ov00_021EC2EC(int,void*,int),ov00_021EC3D8(void*);
extern void ov45_02237A24(void*,void*),ov45_02237BC8(void*),ov45_02242C74(void**,void*,void*);
extern void __msl_assertion_failed(char*,char*,char*,int);
extern void *ov00_021FE5B4(int,int,void(*)(void*));
extern char ov45_02255458[],ov45_02255468[],ov45_02255438[];
extern int ov45_02237F7C(void*,void*);
extern void ov45_02243780(void *p,int a,void *b);
extern int ov45_02243794(void *p,void *key);
extern int ov45_022437CC(void *p,void *key);
extern int ov45_02243808(void *p,void *key);
extern void *ov45_02243F1C(void *p);
extern void *ov45_02244130(void *p);
extern void *ov45_02244150(void *p);
extern void *ov45_02244170(void *p,void *key,void **prev,u8 *left,u8 *first);
extern Node *ov45_022441FC(Tree *p,int *key,Node **prev,u8 *left,u8 *first);
extern Node *ov45_02244268(Tree *p,int *key,Node **prev,u8 *left,u8 *first);
extern void ov45_022442D4(void *p,void *node);
extern void *ov45_02244330(void *p);
extern void ov45_02244350(Tree *p,Tree *q);
extern void ov45_022443DC(void *p,void *node);
extern void ov45_02244428(void *p,void *node);
extern void ov45_0224447C(void *p,void *node);
extern void ov45_022444C8(void *p);
extern int ov45_02244B9C(void *p);
extern int ArrayLength(void*);
extern void *ov00_021FE6EC(void*,int);
extern void ov00_021FE678(void*),ov45_02244500(void*);
extern char ov45_022554F0[],ov45_0225540C[],ov45_02255478[],ov45_02255488[],ov45_022554A4[],ov45_022554BC[],ov45_022554DC[];
extern void *ov45_02243A98(void *p);
extern void ov45_02244BCC(void *p);
typedef struct {Node *node;u8 ok;} InsertResult;
extern Node *ov45_02243E44(Tree*,Node*,int,int,int*),*ov45_0224404C(Tree*,Node*,int,int,int*);
extern u8 ov45_02255358[];
extern void ov45_02243AB8(InsertResult*,Tree*,int*),ov45_02243F3C(InsertResult*,Tree*,int*);
void *ov45_02244130(void *p){if(P(p,4))ov45_02244428(p,P(p,4));return p;}

void *ov45_02244150(void *p){if(P(p,4))ov45_0224447C(p,P(p,4));return p;}
