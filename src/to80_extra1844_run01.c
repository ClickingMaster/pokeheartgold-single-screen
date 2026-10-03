#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <string.h>
long atol(const char*);
typedef void *(*Alloc)(char*,int);
typedef void (*Free)(char*,void*,int);
typedef struct {u8 p[0x1010]; Alloc alloc;Free free;u8 q[12];char url[256];char*host,*path;int unused,mode;u16 port;} Ctx;
typedef struct {char *key,*value;} Pair;
typedef struct {Pair*items;int limit,count;} List;
extern char ov00_022167F0[],ov00_022167F8[],ov00_02216804[],ov00_02216808[],ov00_0221680C[],ov00_0221682C[],ov00_0221684C[],ov00_02216868[];
int ov00_021E9C84(Ctx*,char*);
int ov00_021E9DD8(Ctx*,List*,char*,char*);
#pragma force_active on
int ov00_021E9C84(Ctx*c,char*url){
 char *port=0,*p;
 if(strlen(url)>=256)return 0;
 strncpy(c->url,url,256);
 if(strlen(url)!=strlen(c->url))return 0;
 if(strstr(c->url,ov00_022167F0)){c->host=c->url+7;c->mode=0;c->port=80;}
 else{p=strstr(c->url,ov00_022167F8);if(!p)return 0;c->host=p+8;c->mode=1;c->port=443;}
 p=strstr(c->host,ov00_02216804);if(p){*p=0;port=p+1;}
 p=strstr(c->host,ov00_02216808);if(!p)c->path=0;else{*p=0;c->path=p+1;}
 if(port)c->port=atol(port);return 1;
}
