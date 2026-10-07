#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern void __msl_assertion_failed(const char *,const char *,const char *,int);
extern int strcmp(const char *,const char *);
extern int strnicmp(const char *,const char *);
extern char ov45_022565C0[],ov45_022565D0[],ov45_02255F04[],ov45_02255F14[],ov45_02256708[],ov45_02256768[];
typedef struct {u8 p[0x36c];char name[1];} Connection;
typedef struct {u8 p[8];char *command;void *a,*b;u8 p2[12];char **argv;int argc;} Message;
typedef struct {u32 p;void *own,*join,*update;u8 p2[16];void *disconnect;u8 p3[8];void *arg;} Callbacks;
extern void ov45_02248A98(Connection *,char *,char *);
extern int ov45_0224861C(Connection *,char *);
extern Callbacks *ov45_02247E98(Connection *,char *);
extern void ov45_02247D84(void *,char *);
extern void ov45_02244D30(Connection *,int,void *,void *,void *,int,char *,int);
static inline int validCount(int n){int valid=1;if(n!=2&&n!=3)valid=0;return valid;}
void ov45_0224BB4C(Connection *,Message *);
void ov45_0224BC8C(Connection *,Message *);
void ov45_0224BC8C(Connection *c,Message *msg){struct{char *id,*command,*value;} own;struct{char *id,*command;int mode;char *value,*extra;} packet;char *small;char *id,*command,*other,*extra;char **argv;int argc;Callbacks *cb;if(!c)__msl_assertion_failed(ov45_022565C0,ov45_022565D0,ov45_02255F14,0x794);if(!validCount(msg->argc))__msl_assertion_failed(ov45_02256768,ov45_022565D0,ov45_02255F14,0x79a);argc=msg->argc;if(argc!=2&&argc!=3)return;argv=msg->argv;id=argv[0];other=argv[1];command=msg->command;extra=argc==3?argv[2]:ov45_02256708;ov45_02248A98(c,other,id);cb=ov45_02247E98(c,id);if(!cb)return;if(!strnicmp(other,c->name)){if(cb->own){own.id=id;own.command=command;own.value=extra;ov45_02244D30(c,5,cb->own,&own,cb->arg,0,0,12);}ov45_02247D84(c,id);return;}if(!ov45_0224861C(c,id))return;if(cb->update){packet.id=id;packet.command=other;packet.mode=2;packet.value=extra;packet.extra=command;ov45_02244D30(c,7,cb->update,&packet,cb->arg,0,id,20);}if(cb->disconnect){small=id;ov45_02244D30(c,12,cb->disconnect,&small,cb->arg,0,id,4);}}
