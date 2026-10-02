#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SWAP(x) ((u16)(((x)>>8)|((x)<<8)))
#define IP(p,o) (((u32)SWAP(H(p,o))<<16)|(u32)SWAP(H(p,(o)+2)))
typedef struct S {OSThread*thread;u32 wait;u8 state,ssl;u16 port;u32 c,tick,localIp;u16 remotePort,backupPort;u32 ip,backupIp,recv,seq;u16 window,mss;u32 ack,count;void*callback;u32 cap;void*buf;u32 len;u8 pad[20];void*writeBuf;u32 writeLen;} S;
#define SOC(t) (*(S**)((u8*)(t)+0xa4))
typedef struct {u8 down,error;u16 mtu;u32 unused4;u16 eport,pad;u32 a,b,c;void(*fixed)(void);u8 d[44];int(*active)(void);u32 d4c,ip;u8 e[20];u64 rng,mul,add;} N;
extern N _021D4620;
extern u8 _021D4B08[],_0211069C[];
extern u8 _021D4888[];
extern void OS_YieldThread__(void);
#pragma warn_uninitializedvar off
extern int CPSi_SslClose();
extern int CPSi_SslConnect();
extern int CPSi_SslConsume();
extern int CPSi_SslGetLength();
extern int CPSi_SslListen();
extern void* CPSi_SslRead();
extern int CPSi_SslShutdown();
extern int CPSi_SslWrite2();
extern int CPSi_TcpWrite2Raw();
extern int calc_checksum();
extern int check_tcpudpsum();
extern int dispatch_arp();
extern int dispatch_ip();
extern int dt_ack();
extern int get_targetip();
extern int inq_arpcache();
extern u8* receive_packet();
extern int reset_network_vars();
extern int send_arprequest();
extern int send_ip();
extern int send_ping();
extern int send_tcp();
extern int send_udp();
extern int tcp_send_rst();
extern int throw_packet();
int valid_IP(u32 a,u32 b);
int no_need_inq(u32 a);
void tcp_send_handshake(S*s,int a,int b);
void tcp_send_ack(S*s,int b);
void tcp_send_finack(S*s,int b);
S* find_socket(u8*a,u8*b);
void dt_rst(u8*a,u8*b);
void CPS_SocRegister(S*s);
void CPS_SocUnRegister(void);
void CPS_SocDatagramMode(void);
void CPS_SocBind(u16 a,u16 b,u32 ip);
void CPS_SocUse(void);
void CPS_SocRelease(void);
void CPS_SocDup(OSThread*t);
void CPSi_TcpListenRaw(S*s);
void CPS_SetUdpCallback(void*p);
void CPS_TcpListen(void);
int CPS_TcpConnect(void);
u32 CPS_SocWho(u16*a,u32*b);
void CPSi_TcpShutdownRaw(S*s);
void CPS_TcpShutdown(void);
void CPS_TcpClose(void);
void* udp_read_raw(u32*p,S*s);
void* CPSi_TcpReadRaw(u32*p,S*s);
void* CPS_SocRead(u32*p);
void CPSi_SocConsumeRaw(u32 n,S*s);
void CPS_SocConsume(u32 n);
int tcp_write_do2(void*p,u32 a,void*q,u32 b,S*s,int z);
u32 CPS_SocWrite(void*p,u32 a);
int CPS_SocGetLength(void);
void CPS_SocFlush(void);
void set_fixed_ip(void);
u8* pad_mem(u8 c,u32 want,u8*p,u32 n);
u32 get_seqno(void);
void reply_icmp(u8*a,u8*b,u32 n);
void dispatch_icmp(u8*a,u8*b,u32 n);
void parse_mss(u8*p,S*s);
S* check_listener(u8*a,u8*b);
int check_socket(u8*a,u8*b,S*s);
void dt_syn_LISTEN(u8*a,u8*b,S*s);
int find_specific_socket(u8*a,u8*b,u32 n);
void dt_syn(u8*a,u8*b,u32 n);
void dt_synack(u8*a,u8*b,u32 n);
void dt_fin(u8*a,u8*b,u32 n);
void tcpip(void);
int CPSi_TcpConnectRaw(S*s);
u32 CPSi_SocWrite2(void*p,u32 a,void*q,u32 b);
void process_icmp_reply(u8*a,u8*b,u32 n);
u16 CPS_SocGetEport(void);
u32 tcp_write_do(u8*p,u32 len,S*s,int z);
u8* dhcp_setcommon(u8*p,int a,u32*out);
void dispatch_tcp(u8*a,u8*b,u32 n);
void dt_fin(u8*a,u8*b,u32 n) {S*s=find_socket(a,b);if(!s)return;switch(s->state){case 7:s->recv++;tcp_send_ack(s,0);s->state=9;return;case 8:s->recv++;tcp_send_ack(s,0);s->state=0;if(s->wait!=2)return;s->wait=0;OS_WakeupThreadDirect(s->thread);return;case 4:s->recv++;tcp_send_finack(s,0);s->state=6;return;}tcp_send_rst(a,b,n,0);}

void dt_rst(u8*a,u8*b) {S*s=find_socket(a,b);if(!s)return;OS_YieldThread();s->state=0;if(s->wait-1>1)return;s->wait=0;OS_WakeupThreadDirect(s->thread);}

void dispatch_tcp(u8*a,u8*b,u32 n) {int flags;if(check_tcpudpsum(b,n,a,6))return;n-=(b[12]&240)/4;flags=b[13];switch(flags&23){case 2:if(flags&40)return;dt_syn(a,b,n);return;case 18:if(flags&40)return;dt_synack(a,b,n);return;case 16:case 17:dt_ack(a,b,n);return;case 1:dt_fin(a,b,n);return;}if(flags&4)dt_rst(a,b);else tcp_send_rst(a,b,n,0);}
