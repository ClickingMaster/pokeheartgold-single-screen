#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{u32 allocated:1;u32 rest:31;u32 size;void*data;}String;
typedef struct{String*data;u32 size,cap;}Vec;
typedef struct{u32 state;u8 pad[0x54];String message;int failed;}Work;
extern Work*_022577C4;extern char ov45_022550B8[];
extern void ov45_02237B58(String*,char*),ov45_02237BC8(String*),ov45_02237664(Vec*),ov45_02237D48(String*,String*),ov45_02233B74(String*);extern void ov45_0223649C(Vec*,String*,String*,int);
void ov45_0223633C(int,int,char**,int,int*);
void ov45_0223633C(int valid,int count,char**names,int unused,int*types){Vec parts;String item;String delimiter;String text;String empty;int i;if(!valid)return;for(i=0;i<count;i++){if(types[i]>=20)continue;ov45_02237B58(&item,names[i]);ov45_02237B58(&delimiter,ov45_022550B8);ov45_0223649C(&parts,&item,&delimiter,0);ov45_02237BC8(&delimiter);ov45_02237BC8(&item);if(parts.size<2){ov45_02237664(&parts);continue;} {String*s=parts.data+1;char*p=s->allocated==0?(char*)s+1:s->data;ov45_02237B58(&text,p);}ov45_02237D48(&_022577C4->message,&text);ov45_02237BC8(&text);_022577C4->failed=0;_022577C4->state=6;ov45_02237664(&parts);return;}ov45_02233B74(&empty);ov45_02237D48(&_022577C4->message,&empty);ov45_02237BC8(&empty);_022577C4->failed=1;_022577C4->state=6;}
