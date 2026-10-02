#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#define B(p,n) (((u8 *)(p))[n])
#define S(p,n) (((s8 *)(p))[n])
#define H(p,n) (*(u16 *)((u8 *)(p)+(n)))
#define V(p,n) (*(volatile u32 *)((u8 *)(p)+(n)))
#define W(p,n) (*(u32 *)((u8 *)(p)+(n)))
#define P(p,n) (*(void **)((u8 *)(p)+(n)))
static inline u32 Raw16(u16 v){return (v>>8)|(v<<8);}
struct Addr {u8 len,type;u16 port;u32 ip;};
static inline u16 Swap16(u16 v){return ((v>>8)&255)|((v<<8)&0xff00);}
static inline u32 Swap32(u32 v){return ((v>>24)&255)|((v>>8)&0xff00)|((v<<8)&0xff0000)|((v<<24)&0xff000000);}
extern u8 _021D4490[],_02110648[],_02110630[],_021D4494[],_021D4620[],_021D4B08[],_02110690[];
extern u32 _02110684;
extern OSThread _021D47C8,_021D4888;
void *SOCLi_SocketGetNextPtr(void *,void *);
int SOCL_CreateSocket(void *);
int SOCL_Bind(int,u16);int SOCL_Connect(int,u16,u32);
int SOCL_ReadFrom(int,void *,int,u16 *,u32 *,int);
int SOCL_WriteTo(int,void *,int,u16,u32,int);
int SOCL_Shutdown(int);int SOCL_Close(int);int SOCL_Listen(int,int);int SOCL_Cleanup(void);
int SOCL_Accept(int,u16 *,u32 *);
u32 SOCL_GetHostID(void);int SOCL_SetResolver(u32,u32);u32 SOCL_InetAtoH(char *);
int SOCL_SocketIsInvalid(void *);
void *SOCLi_CreateCommandPacket(int (*)(void *),void *,int);
int SOCLi_ExecCommandPacketInRecvPipe(void *,void *);
void CPS_SetSsl(int);
void put_in_buffer(void *,void *,void *,u32,void *,u32);
int WCM_SendDCFDataEx(void *,void *,int,void *,int);
void OS_Sleep(u32);
void reset_network_vars(u32);
u32 calc_checksum_do(u8 *,u32,u32);
void send_arprequest(u32);
void *inq_arpcache(u32);

int SOCL_SocketIsInTrash(void *a);
int SOC_Socket(int a,int b,int c);
int SOC_Bind(int a,struct Addr *b,int c);
int SOC_Connect(int a,struct Addr *b,int c);
int SOC_Recv(int a,void *b,int c,int d);
int SOC_Send(int a,void *b,int c,int d);
int SOC_RecvFrom(int a,void *b,int c,int d,struct Addr *volatile e,int f);
int SOC_SendTo(int a,void *b,int c,int d,struct Addr *e,int f);
int SOC_Shutdown(int a,int b);
int SOC_Close(int a);
int SOC_Listen(int a,int b);
int SOC_Cleanup(void);
u32 SOC_GetHostID(void);
int SOC_SetResolver(u32 *a,u32 *b);
int SOC_Accept(int a,struct Addr *b,int c);
int SOC_Fcntl(void *a,int b,int c);
void * AllocFunc_SOCL(u32 a);
void FreeFunc_SOCL(u32 *a);
int SOC_InetAtoN(char *a,u32 *b);
void SOC_U32to4U8(u32 a,u8 *b);
int SOCLi_ExecEnableSslCommand(void *a,void *b);
int SOCLi_EnableSslCallBack(void *a);
void OS_YieldThread__(void);
void empty_func(void);
int default_link_is_on(void);
int CPS_CalmDown(void);
void CPS_SetScavengerCallback(void *a);
void CPS_Cleanup(void);
void CPS_SetThreadPriority(u32 a);
u16 invert_checksum(u16 a);
u16 calc_checksum(void *a,u32 b);
int ip_islocal(u32 a);
u32 get_targetip(u32 a);
int is_broadcast(u32 a);
int is_multicast(u32 a);
int maccmp(u16 *a,u16 *b);
void send_packet(u8 *a,int b,void *c,int d);
void CPSi_RecvCallbackFunc(void *a,void *b,void *c,u32 d);
void * arprequest(u32 a);

extern u8 _021D451C[],_021D44AC[],_021D44B4[],_021D44D4[];
extern char _02110678[];
int SOCL_Startup(void *);u32 SOCL_Resolve(char *);
void *STD_CopyLString(char *,const char *,u32);
void * SOC_GetHostByName(char *a);
int SOC_GetSockName(void *a,struct Addr *b,int c);
int SOC_Startup(u8 *a);
char * SOC_InetNtoP(int a,void *b,char *c,u32 d);
int check_tcpudpsum(void *a,u32 b,u8 *c,u32 d);
int ip_isme(u32 a);
void reply_arp(u8 *a);
u32 calc_checksum_do(u8 *a,u32 b,u32 c);
int SOCL_EnableSsl(void *a,void *b);
extern u8 _021D46A0[],_021D4948[];
void CPSi_SslCleanup(void);
int SOCL_GetStatus(void *);
void throw_packet(void);
void reset_network_vars(u32 a);
int SOC_Poll(u8 *a,u32 b,s64 time);
extern u8 _021D450C[];
void *SOC_InetNtoA(u32,...);
void reg_arpcache(u8 *,u32,int);
void * SOC_InetNtoA(u32 a,...);
void send_arprequest(u32 a);
void send_ether(u8 *a,int b,void *c,int d,u32 ip,u16 proto);
void dispatch_arp(u8 *a,u32 b);
void *receive_packet(u32 *);
int CPS_CalmDown(void) { OSIntrMode m=OS_DisableInterrupts();int r=OS_IsThreadTerminated(&_021D47C8);if(!r&&!W(_021D4620,0x44)){W(_021D4620,0x44)=1;OS_WakeupThreadDirect(&_021D47C8);}OS_RestoreInterrupts(m);return r; }

void CPS_SetScavengerCallback(void *a) { P(_021D4620,0x3c)=a; }

void CPS_Cleanup(void) { CPS_CalmDown();OS_JoinThread(&_021D47C8);OS_DestroyThread(&_021D4888);W(_021D4620,0x54)=0;reset_network_vars(0);W(_021D4620,0x58)=0;W(_021D4620,0x5c)=0; }

void CPS_SetThreadPriority(u32 a) { _02110684=a;OS_SetThreadPriority(&_021D4888,a);OS_SetThreadPriority(&_021D47C8,a); }

u32 calc_checksum_do(u8 *a,u32 b,u32 c){if((u32)a&1){while(b>1){c+=(u16)((a[0]<<8)|a[1]);b-=2;a+=2;}}else{c=(u16)(((u16)c>>8)|((u16)c<<8));while(b>1){c+=*(u16 *)a;a+=2;b-=2;}c=((c>>8)&0x00ff00ff)|((c<<8)&0xff00ff00);c=(c>>16)|(c<<16);}if(b)c+=*a<<8;c=(u16)c+(c>>16);c+=c>>16;return (u16)c;}

u16 invert_checksum(u16 a) { a^=65535;if(!a)a=65535;return a; }

u16 calc_checksum(void *a,u32 b) { return invert_checksum(calc_checksum_do(a,b,0)); }

int check_tcpudpsum(void *a,u32 b,u8 *c,u32 d){u32 r=calc_checksum_do(a,b,d);r=calc_checksum_do(c+12,8,r);r+=b;if(r&0x10000)r=(u16)(r+1);return r!=65535;}

int ip_islocal(u32 a) { int r=1;if(a!=0xffffffff&&a!=0x7f000001&&((a&W(_021D4620,0x1c))!=(W(_021D4620,0x50)&W(_021D4620,0x1c))))r=0;return r; }

u32 get_targetip(u32 a) { if(!ip_islocal(a))a=W(_021D4620,0x2c);return a; }

int is_broadcast(u32 a) { int r=0;if(ip_islocal(a)){u32 mask=~W(_021D4620,0x1c);if((a&mask)==mask)r=1;}return r; }

int is_multicast(u32 a) { return (a&0xf0000000)==0xe0000000; }

int ip_isme(u32 a){return W(_021D4620,0x50)==0||a==W(_021D4620,0x50)||a==0x7f000001||is_broadcast(a)||is_multicast(a);}

int maccmp(u16 *a,u16 *b) { int i;for(i=0;i<3;i++)if(*a++!=*b++)return 1;return 0; }

void send_packet(u8 *a,int b,void *c,int d) { int r;MI_CpuCopy8(_02110690,a+6,6);r=WCM_SendDCFDataEx(a,a+6,b-6,c,d);B(_021D4620,0)=r<0; }
