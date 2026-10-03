#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
extern int strncmp(const char*,const char*,unsigned);
extern char *strstr(const char*,const char*);

extern long atol(const char*);

typedef struct {u8 a[0x1d4];
void *p;
u8 gap[4];
char *input;
int capacity,length,offset;
char *saved;
int savedcap;
u8 rest[16];
} State;

typedef struct {State *state;
} Ctx;

extern char ov00_02218030[],ov00_02218034[],ov00_02218060[],ov00_02218084[],ov00_02218068[],ov00_02218074[],ov00_0221808C[],ov00_022180B4[],ov00_022180BC[],ov00_022180C4[],ov00_022180F4[];

extern void ov00_02205064(Ctx*,void*);
extern int ov00_0220267C(Ctx*,void*,void*,int*,int,void*);
extern int ov00_02202454(Ctx*,void*,void*,int*,int*,void*);
extern void ov00_0220A86C(Ctx*,int,char*);
extern void ov00_02202990(Ctx*,int,int);
extern void ov00_0220A3BC(Ctx*,char*,...);
extern char *ov00_021EC3BC(char*,int);
extern void ov00_0220A890(Ctx*,char*);
extern int ov00_02206720(Ctx*,void**,int);
extern int ov00_022067A8(Ctx*,void*,char*);
extern int ov00_0220A3C8(Ctx*,char*,int);
extern int ov00_02201134(Ctx*,char*);
extern int ov00_0220676C(Ctx*);
extern void ov00_021FF3C0(int);

int ov00_02200BC0(Ctx*);

int ov00_02200BC0(Ctx *ctx){void *item;int len;int done=0;int ret,more;
State *s=ctx->state;

char *end,*line;
do{ov00_02205064(ctx,(u8*)s+0x1f4);
ret=ov00_0220267C(ctx,s->p,(u8*)s+0x1f4,&done,1,ov00_02218030);
if(ret)return ret;
ret=ov00_02202454(ctx,s->p,&s->input,&len,&done,ov00_02218030);
if(ret){if(ret!=3)return ret;
ov00_0220A86C(ctx,5,ov00_02218034);
ov00_02202990(ctx,3,1);
return 3;
}end=strstr(s->input,ov00_02218060);
while(end){*end=0;
ov00_0220A3BC(ctx,ov00_02218068,s->input);
len=end-s->input;
if(len>s->savedcap){s->savedcap+=len<2048?2048:len;
line=ov00_021EC3BC(s->saved,s->savedcap+1);
if(!line){ov00_0220A890(ctx,ov00_02218074);
return 1;
}s->saved=line;
}memcpy(s->saved,s->input,len+1);
s->length-=end+7-s->input;
memmove(s->input,end+7,s->length+1);
line=s->saved;
end=strstr(line,ov00_02218084);
if(end){int number=atol(end+4);
if(!ov00_02206720(ctx,&item,number)){ov00_0220A3BC(ctx,ov00_0221808C,number);
}else{ret=ov00_022067A8(ctx,item,s->saved);
if(ret)return ret;
}}else{if(ov00_0220A3C8(ctx,line,1))return 4;
line=s->saved;
if(!strncmp(line,ov00_022180B4,4)){ret=ov00_02201134(ctx,line);
if(ret)return ret;
}else if(strncmp(line,ov00_022180BC,10))ov00_0220A3BC(ctx,ov00_022180C4);
}end=strstr(s->input,ov00_02218060);
}if(done){ov00_0220A86C(ctx,7,ov00_022180F4);
ov00_02202990(ctx,3,1);
return 0;
}more=ov00_0220676C(ctx);
if(more)ov00_021FF3C0(10);
}while(more);
return 0;
}
