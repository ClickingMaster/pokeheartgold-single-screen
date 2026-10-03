#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef union {u32 words[3];struct {u32 heap:1;u32 unused:31;} bits;u8 bytes[12];} String;
typedef struct{int state,error;u8 p[0xb8-8];u8 names[1];} Global;extern Global *_022577C4;extern char ov45_022550B4[];
extern int ov45_0223D7D4(void*,void*,void*,void*,void*,String*),ov45_022414C0(int,char*),ov45_022413D4(char*,char*),ov45_02237FD8(String*,char*),ov45_022415CC(char*,int,int,void*);extern void ov45_02237BC8(String*),ov45_02232434(String*,void*,int),ov45_022377AC(String*,u32),ov45_022379D4(String*);extern void *ov45_02232B9C(String*);extern int ov00_021EFF30(void*,u32,void*,u32);
static inline void Zero(u32 *p){u32 i,zero=0;for(i=0;i<3;i++)p[i]=zero;}
static inline char *Text(String*s){if(s->bits.heap==0)return (char*)&s->bytes[1];else return (char*)s->words[2];}
static inline int Empty(String*s){return ov45_02237FD8(s,ov45_022550B4)==0;}
static inline void Status(int result){volatile u8 z=0;Global*g=_022577C4;int e=0;(void)&z;switch(result){case 1:e=2;break;case 2:e=3;break;}if(e){g->error=e;g->state=10;}}
int ov45_0223325C(int,int,void*,void*,void*,void*,void*);int ov45_02233434(int,int,int,void*,u32);
int ov45_02233434(int id,int key,int mode,void*data,u32 length){String name,encoded;int result;char *text;ov45_02232434(&name,_022577C4->names,key);if(Empty(&name)){ov45_02237BC8(&name);return 5;}if(length>300){ov45_02237BC8(&name);return 3;}encoded.words[0]=0;encoded.words[1]=0;encoded.words[2]=0;ov45_022377AC(&encoded,(length+2)/3*4+1);{void *out=ov45_02232B9C(&encoded);int end=ov00_021EFF30(data,length,out,encoded.words[1]);((char*)encoded.words[0])[end]=0;}text=Text(&name);result=ov45_022415CC(text,id,mode,ov45_02232B9C(&encoded));Status(result);ov45_022379D4(&encoded);ov45_02237BC8(&name);return result;}
