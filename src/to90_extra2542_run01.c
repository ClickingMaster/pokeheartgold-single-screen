#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern void __msl_assertion_failed(const char *,const char *,const char *,int);
extern char ov45_022559B8[],ov45_02255918[],ov45_0225569C[],ov45_022559D4[],ov45_022559E4[],ov45_022559F8[],ov45_02255A18[],ov45_02255678[];
extern char ov45_02255A2C[];
extern void *ov00_021EC3A8(u32);
extern int ov45_02247818(const void*,const void*);
extern void ov45_02247934(void*);
extern int ov45_022477B8(const void*,int);
extern int ov45_02247AD4(const void*,const void*);
typedef struct DArray DArray;typedef struct HashTable HashTable;
typedef int (*HashFn)(const void*,int);typedef int (*CompareFn)(const void*,const void*);typedef void (*FreeFn)(void*);
extern HashTable *ov00_021FED4C(int,int,int,HashFn,CompareFn,FreeFn);
extern int ov00_021FEA40(DArray*,const void*,CompareFn,int,int);
extern void ov00_021FE8D8(DArray*,int);
extern void ov00_021FEF74(HashTable*,const void*);
typedef struct {u8 p[0x80c];HashTable *list;DArray *thread;} Work;
#include "to90_ov45_input_private.h"
typedef struct {char name[257];u8 pad[3];Ov45Input48 input;void *thread;u8 p[0x158 - 0x138];int a;void *allocated;int b;u8 c;u8 tail[0x1e4 - 0x165];} Event;
typedef char EventSize[(sizeof(Event)==484)?1:-1];

#pragma force_active on
void ov45_02247B94(void *parent,const char *name,const Ov45Input48 *input){Work*w=parent;char *allocated;if(!w)__msl_assertion_failed(ov45_022559B8,ov45_02255918,ov45_0225569C,0x160);if(!name)__msl_assertion_failed(ov45_022559D4,ov45_02255918,ov45_0225569C,0x162);if(!*name)__msl_assertion_failed(ov45_022559E4,ov45_02255918,ov45_0225569C,0x162);if(strlen(name)>=257)__msl_assertion_failed(ov45_022559F8,ov45_02255918,ov45_0225569C,0x162);if(!input)__msl_assertion_failed(ov45_02255A18,ov45_02255918,ov45_0225569C,0x163);allocated=ov00_021EC3A8(2);if(!allocated)return;*allocated=*(const u8*)ov45_02255A2C;{Event e={0};e.input=*input;strncpy(e.name,name,257);e.name[256]=0;e.thread=ov00_021FED4C(224,61,2,ov45_022477B8,ov45_02247818,ov45_02247934);if(!e.thread)return;e.a=0;e.allocated=allocated;e.b=0;e.c=0;{int index=ov00_021FEA40(w->thread,&e,ov45_02247AD4,0,0);if(index!=-1)ov00_021FE8D8(w->thread,index);}ov00_021FEF74(w->list,&e);}}

extern int ov00_021FF01C(HashTable*,const void*);
void ov45_02247D84(void *,char *);
void ov45_02247D84(void *parent,char *name){Work*w=parent;Event e;if(!w)__msl_assertion_failed(ov45_022559B8,ov45_02255918,ov45_02255678,0x189);if(!name)__msl_assertion_failed(ov45_022559D4,ov45_02255918,ov45_02255678,0x18b);if(!*name)__msl_assertion_failed(ov45_022559E4,ov45_02255918,ov45_02255678,0x18b);if(strlen(name)>=257)__msl_assertion_failed(ov45_022559F8,ov45_02255918,ov45_02255678,0x18b);strncpy(e.name,name,257);e.name[256]=0;{int index=ov00_021FEA40(w->thread,&e,ov45_02247AD4,0,0);if(index!=-1){ov00_021FE8D8(w->thread,index);return;}}ov00_021FF01C(w->list,&e);}
