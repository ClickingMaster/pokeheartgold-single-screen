#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include <string.h>
#include <stdarg.h>
int vsprintf(char *,const char *,va_list);
typedef struct {char *data;int used,capacity;} Buffer;
typedef struct {int handle;u32 state;u8 pad[0x100];Buffer recv,send;int crypt;u8 context[0x102];u8 sendContext[1];} Conn;
void __msl_assertion_failed(const char *,const char *,const char *,int);
void ov45_02252530(int,int *,int *,int);
int ov00_021FF970(int,const void *,int,int);
void ov45_02251DB4(Buffer *,int);
int ov45_02251C50(Buffer *,int);
void ov45_022495DC(void *,int,void *);
void ov45_02252590(Conn *);
void ov45_02252AA0(Conn *);
void ov45_02252800(Conn *);
int ov45_02252CB4(Conn *,const char *);
int ov45_022531D8(signed char *,char **);
void *ov00_021EC3A8(u32);
extern const char ov45_02257334[];
extern const char ov45_02257258[];
extern const char ov45_0225720C[];
extern const char ov45_02257344[];
extern const char ov45_022573C4[];
extern const char ov45_022573E4[];
extern const char ov45_02257408[];
extern const char ov45_02257438[];
extern const char ov45_0225745C[];
extern const char ov45_0225749C[];
extern const char ov45_022574BC[];
extern const char ov45_022574E0[];
extern const char ov45_02257514[];
extern const char ov45_02257538[];
extern const char ov45_022575D8[];
extern const char ov45_022571B4[];
extern const char ov45_02257154[];
extern const char ov45_02257248[];
extern const char ov45_022575FC[];
extern const char ov45_02257148[];
extern const char ov45_022571A4[];
extern char ov45_022577F8[];
int ov45_02252F74(Conn *,const char *,...);
static inline int nonempty(signed char *str){return str&&*str;}
static inline void validate(Conn *c,const char *fn,int line){
if(!c)__msl_assertion_failed(ov45_02257334,ov45_02257258,fn,line);
if(c->state>2)__msl_assertion_failed(ov45_02257344,ov45_02257258,fn,line);
if(!&c->recv)__msl_assertion_failed(ov45_022573C4,ov45_02257258,fn,line);
if(c->recv.capacity<0)__msl_assertion_failed(ov45_022573E4,ov45_02257258,fn,line);
if(c->recv.capacity%8192)__msl_assertion_failed(ov45_02257408,ov45_02257258,fn,line);
if(c->recv.used<0)__msl_assertion_failed(ov45_02257438,ov45_02257258,fn,line);
if(c->recv.used>c->recv.capacity)__msl_assertion_failed(ov45_0225745C,ov45_02257258,fn,line);
if(!&c->send)__msl_assertion_failed(ov45_0225749C,ov45_02257258,fn,line);
if(c->send.capacity<0)__msl_assertion_failed(ov45_022574BC,ov45_02257258,fn,line);
if(c->send.capacity%8192)__msl_assertion_failed(ov45_022574E0,ov45_02257258,fn,line);
if(c->send.used<0)__msl_assertion_failed(ov45_02257514,ov45_02257258,fn,line);
if(c->send.used>c->send.capacity)__msl_assertion_failed(ov45_02257538,ov45_02257258,fn,line);
}
int ov45_022531D8(signed char *str,char **out){signed char *user=NULL,*host=NULL,*p;int n=0,u=0,h=0;if(!str||!*(volatile signed char*)str){__msl_assertion_failed(ov45_022575FC,ov45_02257258,ov45_02257148,0x232);return 0;}p=str;while(*p){if(*(volatile signed char*)p!='!'){p++;n++;}else{user=++p;while(*p){if(*p!='@'){p++;u++;}else{host=++p;while(*p){p++;h++;}}}}}if(n){out[2]=ov00_021EC3A8(n+1);if(out[2]){memcpy(out[2],str,n);out[2][n]=0;}}else out[2]=NULL;if(u){out[3]=ov00_021EC3A8(u+1);if(out[3]){memcpy(out[3],user,u);out[3][u]=0;}}else out[3]=NULL;if(h){out[4]=ov00_021EC3A8(h+1);if(out[4]){memcpy(out[4],host,h);out[4][h]=0;}}else out[4]=NULL;return 1;}
