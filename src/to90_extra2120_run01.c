#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern int strncmp(const char*,const char*,size_t);
extern long atol(const char*);
extern char *strchr(const char*,int);
typedef struct {u8 pad[0x14];char *url,*host;u32 unused;u16 port,pad2;char *path;} Request;
extern const char ov38_02225B68[],ov38_02225B74[],ov38_02225AD4[],ov38_02225B84[],ov38_02225B94[],ov38_02225B9C[],ov38_02225BA8[],ov38_02225BAC[];
extern void __msl_assertion_failed(const char*,const char*,const char*,int);
extern char *ov00_021FF3D4(const char*);
int ghiParseURL(Request*);
#pragma force_active on
int ghiParseURL(Request *req){char *p;char saved;size_t n;int secure;char *path,*space;
 if(!req)__msl_assertion_failed(ov38_02225B68,ov38_02225B74,ov38_02225AD4,37);
 if(!req)return 0;
 if(!req->url)__msl_assertion_failed(ov38_02225B84,ov38_02225B74,ov38_02225AD4,43);
 p=req->url;if(!p)return 0;
 if(strncmp(p,ov38_02225B94,7)==0){p+=7;secure=0;}else if(strncmp(p,ov38_02225B9C,8)==0){p+=8;secure=1;}else return 0;
 n=strspn(p,ov38_02225BA8);saved=p[n];p[n]=0;req->host=ov00_021FF3D4(p);if(!req->host)return 0;p[n]=saved;p+=n;
 if(*p==':'){p++;req->port=atol(p);if(req->port==0)return 0;while(*++p && *p!='/'){};}else{if(secure)req->port=443;else req->port=80;}
 if(!*p)p=(char*)ov38_02225BAC;path=req->path=ov00_021FF3D4(p);space=strchr(path,' ');while(space){*space='+';path=req->path;space=strchr(path,' ');}if(!path)return 0;return 1;
}
