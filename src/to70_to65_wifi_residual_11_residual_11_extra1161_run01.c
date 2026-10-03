#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {void *active;u32 p;u8 state,pad;u16 port;u8 q[12];u16 remotePort,p2;u32 ip;u8 r[14];u16 mss;} Socket;
typedef struct Thread {u8 p[0x68];struct Thread *next;u8 q[0xa4-0x6c];Socket *socket;} Thread;
typedef struct {u8 p[12];u16 high,low;} IP;
typedef struct {u16 source,dest;u8 p[8];u8 size;u8 q[7];u8 options[];} TCP;
static inline u16 Swap(int x){return (x>>8)|(x<<8);}
static inline Thread *Threads(void){return (Thread*)OSi_ThreadInfo.list;}
Socket *check_listener(IP *,TCP *);int check_socket(IP *,TCP *,Socket *);Socket *find_socket(IP *,TCP *);void parse_mss(TCP *,Socket *);
Socket *check_listener(IP *ip,TCP *tcp){Socket *s;Thread *thread=Threads();while(thread){s=thread->socket;if(s&&s->active&&s->state==1&&s->port==Swap(tcp->dest)){if(s->remotePort==0||s->remotePort==Swap(tcp->source)){if(s->ip==0||s->ip==(((u32)Swap(ip->high)<<16)|Swap(ip->low)))return s;}}thread=thread->next;}return 0;}
int check_socket(IP *ip,TCP *tcp,Socket *s){int result=0,valid=0,remote=0,port=0;if(s->state!=10&&s->state!=11)valid=1;if(valid){if(s->port==Swap(tcp->dest))port=1;}if(port){if(s->remotePort==Swap(tcp->source))remote=1;}if(remote){if(s->ip==(((u32)Swap(ip->high)<<16)|Swap(ip->low)))result=1;}return result;}
Socket *find_socket(IP *ip,TCP *tcp){Socket *s;Thread *thread=Threads();while(thread){s=thread->socket;if(s&&s->active){if(check_socket(ip,tcp,s))return s;}thread=thread->next;}return 0;}
void parse_mss(TCP *tcp,Socket *s){int length;u8 *p;s->mss=536;p=tcp->options;length=(tcp->size&240)/4-20;if(length--){do{int option=*p++;switch(option){case 0:return;case 1:break;case 2:s->mss=(p[1]<<8)|p[2];p+=3;length-=3;break;default:{int count=*p-1;length-=count;p+=count;break;}}}while(length--);}}
