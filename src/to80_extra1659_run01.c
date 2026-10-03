#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Parse { u8 pad[0x328]; char *raw,*prefix,*a,*b,*c,*command,*target,*tail;int d,e; } Parse;
extern void *ov00_021EC3A8(unsigned);
extern void ov00_021EC3D8(void *);
extern int ov45_022531D8(char *,void *);
extern int ov45_022533E4(char *,void *);
extern char ov45_022575FC[],ov45_02257258[],ov45_022571C4[];
extern void __msl_assertion_failed(char *,char *,char *,int);
int ov45_02253528(Parse *,char *);
int ov45_02253528(Parse *p,char *s){
 int prefix=0,command=0,target=0,tail=0,len;char *old;
 if(!s||!*s){__msl_assertion_failed(ov45_022575FC,ov45_02257258,ov45_022571C4,0x2e4);return 0;}
 len=strlen(s);p->raw=ov00_021EC3A8(len+1);if(!p->raw)return 0;memcpy(p->raw,s,len);p->raw[len]=0;
 while(*s=='\n'||*s=='\r')s++;
 if(*s==':'){s++;p->prefix=s;if(*s)while(*s!=' '&&*s){prefix++;s++;}}
 while(*s==' ')s++;
 if(*s){p->command=s;while(*s!=' '&&*s){command++;s++;}}
 while(*s==' ')s++;
 if(*s!=':'&&*s){p->target=s;while(*s!=' '&&*s){target++;s++;}}
 while(*s==' ')s++;
 if(*s){p->tail=s;while(*s){s++;tail++;}}
 if(prefix){old=p->prefix;p->prefix=ov00_021EC3A8(prefix+1);if(p->prefix){memcpy(p->prefix,old,prefix);p->prefix[prefix]=0;}
 if(!ov45_022531D8(p->prefix,&p->raw)){if(p->raw){ov00_021EC3D8(p->raw);p->raw=0;}if(p->prefix){ov00_021EC3D8(p->prefix);p->prefix=0;}return 0;}}
 else{p->prefix=0;p->a=0;p->b=0;p->c=0;}
 if(target){if(!ov45_022533E4(p->target,&p->raw)){ if(p->raw){ov00_021EC3D8(p->raw);p->raw=0;}
 if(p->prefix){ov00_021EC3D8(p->prefix);p->prefix=0;}
 if(p->a){ov00_021EC3D8(p->a);p->a=0;}
 if(p->b){ov00_021EC3D8(p->b);p->b=0;}
 if(p->c){ov00_021EC3D8(p->c);p->c=0;}
 return 0;}}
 else if(tail){if(!ov45_022533E4(p->tail,&p->raw)){ if(p->raw){ov00_021EC3D8(p->raw);p->raw=0;}
 if(p->prefix){ov00_021EC3D8(p->prefix);p->prefix=0;}
 if(p->a){ov00_021EC3D8(p->a);p->a=0;}
 if(p->b){ov00_021EC3D8(p->b);p->b=0;}
 if(p->c){ov00_021EC3D8(p->c);p->c=0;}
 return 0;}}
 else{p->d=0;p->e=0;}
 if(tail){old=p->tail;p->tail=ov00_021EC3A8(tail+1);if(p->tail){memcpy(p->tail,old,tail);p->tail[tail]=0;}}else p->tail=0;
 if(command){old=p->command;p->command=ov00_021EC3A8(command+1);if(p->command){memcpy(p->command,old,command);p->command[command]=0;}}else p->command=0;
 if(target){old=p->target;p->target=ov00_021EC3A8(target+1);if(p->target){memcpy(p->target,old,target);p->target[target]=0;}}else p->target=0;
 return 1;
}
