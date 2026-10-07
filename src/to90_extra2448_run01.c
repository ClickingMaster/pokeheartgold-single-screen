#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern long atol(const char *);
typedef union {u32 words[3];struct {u8 flags;char small[11];} local;struct {u32 long_:1;u32 rest:31;u32 length;char *text;} heap;} Text;
typedef struct {Text *items;volatile u32 count;u32 capacity;} Vector;
typedef struct {void (*first)(int,void *,void *);void (*second)(int,void *);u32 third;} Callbacks;
typedef struct {u32 first;int state;u8 pad[0x64-8];Callbacks callbacks;} Work;
extern Work *ov45_022577DC;extern char ov45_022552C0[],ov45_022552C8[],ov45_022552D8[];
extern void ov45_02237B58(Text *,char *);extern int ov45_0223D644(Text *,Text *,int);extern void ov45_02237BC8(Text *);extern void ov45_02237664(Vector *);extern void ov45_0223F000(Vector *,Text *,Text *,int);extern int ov45_022435F8(Work *,Text *);
#pragma force_active off
static inline char *TextChars(Text*t){if(t->heap.long_==0)return t->local.small;else return t->heap.text;}
static inline int Compare(register char*b,char*a,Text*ta,Text*tb,int n){int result;ov45_02237B58(ta,a);ov45_02237B58(tb,b);result=ov45_0223D644(ta,tb,n);ov45_02237BC8(tb);ov45_02237BC8(ta);return result;}
#pragma force_active on
#pragma opt_propagation off
void ov45_0223EDD8(void *,char *,char *,int,void *);
void ov45_0223EDD8(void *unused,char *name,char *data,int kind,void *arg){struct {Text c,d,a,b;volatile Callbacks second;Text g;volatile Callbacks first;Text f,e;Vector vector;} frame;char *firstKey,*secondKey;int firstResult,secondResult,id;if(ov45_022577DC&&ov45_022577DC->state==5)return;firstKey=ov45_022552C0;firstResult=Compare(firstKey,name,&frame.a,&frame.b,6);if(firstResult==0){secondKey=ov45_022552C8;secondResult=Compare(secondKey,data,&frame.c,&frame.d,12);if(secondResult!=0)return;ov45_02237B58(&frame.e,data);ov45_02237B58(&frame.f,ov45_022552D8);ov45_0223F000(&frame.vector,&frame.e,&frame.f,0);ov45_02237BC8(&frame.f);ov45_02237BC8(&frame.e);if(frame.vector.count==0){ov45_02237664(&frame.vector);return;}{Text *last=&frame.vector.items[frame.vector.count-1];id=atol(TextChars(last));}if(id==0){ov45_02237664(&frame.vector);return;}frame.first=ov45_022577DC->callbacks;frame.first.second(id,arg);ov45_02237664(&frame.vector);return;}if(kind!=3)return;ov45_02237B58(&frame.g,name);id=ov45_022435F8(ov45_022577DC,&frame.g);ov45_02237BC8(&frame.g);if(id==-1)return;frame.second=ov45_022577DC->callbacks;frame.second.first(id,data,arg);}
