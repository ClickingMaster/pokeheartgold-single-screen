#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{u8 bytes[0x834];} State;
typedef struct{u8 p[12];State *state;u8 p2[8];u16 port;u8 p3[2];u32 ip;} Socket;
#define B(s,o) ((s)->bytes[o])
#define P(s,o) ((s)->bytes+(o))
extern u8 *(*_021D4634)(int);
extern void (*_021D4660)(void *);
extern volatile u16 _021106B0[];
extern u32 date2sec(void);
extern void set_random(void *,int),finished_md5(State *,void *,int),finished_sha1(State *,void *,int),update_digest(State *,void *,int),find_session_from_IP(State *,u32,u16);
extern int make_ciphertext(State *,void *),CPSi_TcpWrite2Raw(void *,int,int,int,Socket *);
void send_change_cipher_spec_and_finished(Socket *);
void send_client_hello(Socket *);
void send_change_cipher_spec_and_finished(Socket *sock){State *s=sock->state;u8 *packet=_021D4634(131);if(!packet){B(s,0x455)=9;return;}packet[0]=20;packet[1]=3;packet[2]=0;packet[3]=0;packet[4]=1;packet[5]=1;MI_CpuFill8(P(s,0x1cc),0,8);packet[6]=22;packet[7]=3;packet[8]=0;packet[9]=0;packet[10]=40;packet[11]=20;packet[12]=0;packet[13]=0;packet[14]=36;MI_CpuCopy8(P(s,0x3a4),P(s,0x3fc),0x58);finished_md5(s,packet+15,0);MI_CpuCopy8(P(s,0x3fc),P(s,0x3a4),0x58);MI_CpuCopy8(P(s,0x2ec),P(s,0x348),0x5c);finished_sha1(s,packet+31,0);MI_CpuCopy8(P(s,0x348),P(s,0x2ec),0x5c);update_digest(s,packet+11,40);CPSi_TcpWrite2Raw(packet,make_ciphertext(s,packet+6)+6,0,0,sock);_021D4660(packet);}
void send_client_hello(Socket *sock){State *s=sock->state;u8 *packet,*p;u32 now,i;int n;packet=_021D4634(152);if(!packet){B(s,0x455)=9;return;}packet[9]=3;packet[10]=0;now=date2sec();B(s,0x34)=now>>24;B(s,0x35)=now>>16;B(s,0x36)=now>>8;B(s,0x37)=now;set_random(P(s,0x38),28);MI_CpuCopy8(P(s,0x34),packet+11,32);find_session_from_IP(s,sock->ip,sock->port);if(B(s,0x30)){packet[43]=32;MI_CpuCopy8(P(s,0x74),packet+44,32);p=packet+76;}else{packet[43]=0;p=packet+44;}p[0]=0;p[1]=4;p+=2;for(i=0;i<2;i++){p[0]=_021106B0[i]>>8;p[1]=_021106B0[i];p+=2;}p[0]=1;p[1]=0;n=(u32)(p+2)-(u32)packet-5;packet[0]=22;packet[1]=3;packet[2]=0;packet[3]=n>>8;packet[4]=n;packet[5]=1;packet[6]=(n-4)>>16;packet[7]=(n-4)>>8;packet[8]=n-4;CPSi_TcpWrite2Raw(packet,n+5,0,0,sock);update_digest(s,packet+5,n);_021D4660(packet);}
