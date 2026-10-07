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
void ov45_0224BB4C(Connection *arg,Message *incoming){Message *msg=incoming;struct{char *id,*command;int mode;char *value;int extra;} packet;char *small;char *command,*id;Callbacks *cb;Connection *c=arg;char *value;if(!c)__msl_assertion_failed(ov45_022565C0,ov45_022565D0,ov45_02255F04,0x752);command=msg->command;id=msg->argv[0];value=msg->argc>1?msg->argv[1]:ov45_02256708;if(!strcmp(command,c->name))return;ov45_02248A98(c,command,id);if(!ov45_0224861C(c,id))return;cb=ov45_02247E98(c,id);if(!cb)return;if(cb->update){packet.id=id;packet.command=command;packet.mode=0;packet.value=value;packet.extra=0;ov45_02244D30(c,7,cb->update,&packet,cb->arg,0,id,20);}if(cb->disconnect){small=id;ov45_02244D30(c,12,cb->disconnect,&small,cb->arg,0,id,4);}}
