#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern long atol(const char*);

extern void*memcpy(void*,const void*,u32);
extern int strncmp(const char*,const char*,u32);
typedef struct{u32 pad,count;char**names;long*values;char**texts;} List;
typedef struct{u8 p[0x10];void*arg;u32 pad;int first;List*list;int second;} Context;
typedef struct{u32 pad;char*name,*value,*text;} Arguments;
typedef struct{u8 p[0x20];Arguments*args;int state;} Work;
typedef struct{int type,index;char*name,*text;long value;int pad;} Message;
extern const char ov45_0225680C[],ov45_022565D0[],ov45_02255FFC[],ov45_022568DC[];
extern void __msl_assertion_failed(const char*,const char*,const char*,int);
extern Context*ov45_022496AC(void*,int,u32*);
extern void*ov00_021EC3A8(u32),*ov00_021EC3BC(void*,u32);
extern void ov00_021EC3D8(void*);
extern void ov45_02244D30(void*,int,void*,Message*,int,int,int,int);
static inline void initializeZero(int a,int b,volatile u32*p,int value){p[0]=value;p[1]=value;p[2]=value;}
void ov45_0224DFC4(void*,Work*);
void ov45_0224DFC4(void*w,Work*work){Context*ctx;List*list;char*name,*text;long value;u32 len;int index;void*newData;if(work->state!=4)__msl_assertion_failed(ov45_0225680C,ov45_022565D0,ov45_02255FFC,0xe01);if(work->state!=4)return;{u32 zero[3]={0,0,0};Message message;zero[0]=0;ctx=ov45_022496AC(w,1,zero);if(ctx==0 || ctx->arg==0)return;list=ctx->list;if(list==0)__msl_assertion_failed(ov45_022568DC,ov45_022565D0,ov45_02255FFC,0xe15);len=strlen(work->args->name);name=ov00_021EC3A8(len+1);if(name==0)return;memcpy(name,work->args->name,len+1);value=atol(work->args->value);len=strlen(work->args->text);text=ov00_021EC3A8(len+1);if(text==0){ov00_021EC3D8(name);return;}memcpy(text,work->args->text,len+1);index=list->count;message.index=index;message.name=name;message.text=text;message.value=value;message.type=1;ov45_02244D30(w,13,ctx->arg,&message,ctx->first,ctx->second,0,24);newData=ov00_021EC3BC(list->names,(list->count+1)*4);if(newData==0){ov00_021EC3D8(name);ov00_021EC3D8(text);return;}list->names=newData;list->names[index]=name;newData=ov00_021EC3BC(list->values,(list->count+1)*4);if(newData==0){ov00_021EC3D8(name);ov00_021EC3D8(text);return;}list->values=newData;list->values[index]=value;newData=ov00_021EC3BC(list->texts,(list->count+1)*4);if(newData==0){ov00_021EC3D8(name);ov00_021EC3D8(text);return;}list->texts=newData;list->texts[index]=text;list->count++;(void)zero;}}
