#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u32 count;void*anchor;u32 pad;void*first;} Tree;
typedef struct{void*node;u8 flag;} Result;
extern u8 ov45_02255358[];
extern void*ov45_02244170(Tree*,void*,void**,u8*,u8*);
extern int ov45_02237F7C(void*,void*);
void*ov45_02243970(Tree*,void*,u8,u8,void*);
extern void ov45_02236E58(void**);
extern void*ov45_022370D0(void*,void*);
extern void ov45_02243F1C(void*),ov45_02237BC8(void*),ov00_021EC2EC(int,void*,int);
static inline int isLess(void*a,void*b){return ov45_02237F7C(a,b)<0;}
void ov45_02243840(Result*,Tree*,void*);
void ov45_022438F0(void**,Tree*,void*);
typedef struct{u32 prefix[3];void*owner;u32 start;void*head,*tail;u32 value;u32 data[6];} Payload;
typedef struct{void*left,*right,*parent;Payload payload;} Node;
typedef struct{u8 pad,flag;u16 pad2;void*a,*b,*anchor;void*volatile current;} Guard;
static inline void initializeGuard(volatile Guard*g,Node*n,void*anchor){g->a=n;g->b=n;g->anchor=anchor;g->current=n;}
extern const char ov45_022553AC[];
extern int printf(const char*,...);
extern void abort(void);
extern void*ov45_02242394(int);
extern void ov45_02237A24(Payload*,Payload*),ov45_02243BC0(void*,void*),ov45_02236EC4(void*,void*);
void ov45_02243840(Result*out,Tree*t,void*key){u8 left,first;void*previous;void*parent=ov45_02244170(t,key,&previous,&left,&first);if(previous==0 || isLess((u8*)previous+12,key)){out->node=ov45_02243970(t,parent,left,first,key);out->flag=ov45_02255358[4];}else{out->node=previous;out->flag=ov45_02255358[5];}}
void ov45_022438F0(void**out,Tree*t,void*node){void*saved=node;if(saved==t->first){ov45_02236E58(&node);t->first=node;}*out=ov45_022370D0(saved,t->anchor);ov45_02243F1C((u8*)saved+28);ov45_02237BC8((u8*)saved+12);if(saved)ov00_021EC2EC(0,saved,0);t->count--;}
