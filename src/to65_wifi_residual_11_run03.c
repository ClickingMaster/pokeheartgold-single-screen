#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define B(p, n) (((u8 *)(p))[n])
#define S(p, n) (((s8 *)(p))[n])
#define H(p, n) (*(u16 *)((u8 *)(p) + (n)))
#define V(p, n) (*(volatile u32 *)((u8 *)(p) + (n)))
#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define P(p, n) (*(void **)((u8 *)(p) + (n)))

static inline u32 Raw16(u16 v) {
    return (v >> 8) | (v << 8);
}

struct Addr {
    u8 len, type;
    u16 port;
    u32 ip;
};

static inline u16 Swap16(u16 v) {
    return ((v >> 8) & 255) | ((v << 8) & 0xff00);
}

static inline u32 Swap32(u32 v) {
    return ((v >> 24) & 255) | ((v >> 8) & 0xff00) | ((v << 8) & 0xff0000) | ((v << 24) & 0xff000000);
}

extern u8 _021D4490[];
extern u8 _02110648[];
extern u8 _02110630[];
extern u8 _021D4494[];
extern u8 _021D4620[];
extern u8 _021D4B08[];
extern u8 _02110690[];
extern u32 _02110684;
extern OSThread _021D47C8;
extern OSThread _021D4888;
void *SOCLi_SocketGetNextPtr(void *, void *);
int SOCL_CreateSocket(void *);
int SOCL_Bind(int, u16);
int SOCL_Connect(int, u16, u32);
int SOCL_ReadFrom(int, void *, int, u16 *, u32 *, int);
int SOCL_WriteTo(int, void *, int, u16, u32, int);
int SOCL_Shutdown(int);
int SOCL_Close(int);
int SOCL_Listen(int, int);
int SOCL_Cleanup(void);
int SOCL_Accept(int, u16 *, u32 *);
u32 SOCL_GetHostID(void);
int SOCL_SetResolver(u32, u32);
u32 SOCL_InetAtoH(char *);
int SOCL_SocketIsInvalid(void *);
void *SOCLi_CreateCommandPacket(int (*)(void *), void *, int);
int SOCLi_ExecCommandPacketInRecvPipe(void *, void *);
void CPS_SetSsl(int);
void put_in_buffer(void *, void *, void *, u32, void *, u32);
int WCM_SendDCFDataEx(void *, void *, int, void *, int);
void OS_Sleep(u32);
void reset_network_vars(u32);
u32 calc_checksum_do(u8 *, u32, u32);
void send_arprequest(u32);
void *inq_arpcache(u32);

int SOCL_SocketIsInTrash(void *a);
int SOC_Socket(int a, int b, int c);
int SOC_Bind(int a, struct Addr *b, int c);
int SOC_Connect(int a, struct Addr *b, int c);
int SOC_Recv(int a, void *b, int c, int d);
int SOC_Send(int a, void *b, int c, int d);
int SOC_RecvFrom(int a, void *b, int c, int d, struct Addr *volatile e, int f);
int SOC_SendTo(int a, void *b, int c, int d, struct Addr *e, int f);
int SOC_Shutdown(int a, int b);
int SOC_Close(int a);
int SOC_Listen(int a, int b);
int SOC_Cleanup(void);
u32 SOC_GetHostID(void);
int SOC_SetResolver(u32 *a, u32 *b);
int SOC_Accept(int a, struct Addr *b, int c);
int SOC_Fcntl(void *a, int b, int c);
void *AllocFunc_SOCL(u32 a);
void FreeFunc_SOCL(u32 *a);
int SOC_InetAtoN(char *a, u32 *b);
void SOC_U32to4U8(u32 a, u8 *b);
int SOCLi_ExecEnableSslCommand(void *a, void *b);
int SOCLi_EnableSslCallBack(void *a);
void OS_YieldThread__(void);
void empty_func(void);
int default_link_is_on(void);
int CPS_CalmDown(void);
void CPS_SetScavengerCallback(void *a);
void CPS_Cleanup(void);
void CPS_SetThreadPriority(u32 a);
u16 invert_checksum(u16 a);
u16 calc_checksum(void *a, u32 b);
int ip_islocal(u32 a);
u32 get_targetip(u32 a);
int is_broadcast(u32 a);
int is_multicast(u32 a);
int maccmp(u16 *a, u16 *b);
void send_packet(u8 *a, int b, void *c, int d);
void CPSi_RecvCallbackFunc(void *a, void *b, void *c, u32 d);
void *arprequest(u32 a);

extern u8 _021D451C[];
extern u8 _021D44AC[];
extern u8 _021D44B4[];
extern u8 _021D44D4[];
extern char _02110678[];
int SOCL_Startup(void *);
u32 SOCL_Resolve(char *);
void *STD_CopyLString(char *, const char *, u32);
void *SOC_GetHostByName(char *a);
int SOC_GetSockName(void *a, struct Addr *b, int c);
int SOC_Startup(u8 *a);
char *SOC_InetNtoP(int a, void *b, char *c, u32 d);
int check_tcpudpsum(void *a, u32 b, u8 *c, u32 d);
int ip_isme(u32 a);
void reply_arp(u8 *a);
u32 calc_checksum_do(u8 *a, u32 b, u32 c);
int SOCL_EnableSsl(void *a, void *b);
extern u8 _021D46A0[];
extern u8 _021D4948[];
void CPSi_SslCleanup(void);
int SOCL_GetStatus(void *);
void throw_packet(void);
void reset_network_vars(u32 a);
int SOC_Poll(u8 *a, u32 b, s64 time);
extern u8 _021D450C[];
void *SOC_InetNtoA(u32, ...);
void reg_arpcache(u8 *, u32, int);
void *SOC_InetNtoA(u32 a, ...);
void send_arprequest(u32 a);
void send_ether(u8 *a, int b, void *c, int d, u32 ip, u16 proto);
void dispatch_arp(u8 *a, u32 b);
void *receive_packet(u32 *);

int SOC_GetSockName(void *a, struct Addr *b, int c) {
    u32 ip, port;
    if (!a) {
        return -39;
    }
    ip = SOCL_GetHostID();
    port = a ? H(a, 0x74) : 0;
    if (!ip) {
        port = 0;
    }
    b->len = 8;
    b->type = 2;
    b->port = Swap16((u16)port);
    b->ip = Swap32(ip);
    return 0;
}

u32 SOC_GetHostID(void) {
    return Swap32(SOCL_GetHostID());
}

int SOC_SetResolver(u32 *a, u32 *b) {
    return SOCL_SetResolver(Swap32(*a), Swap32(*b));
}

int SOC_Listen(int a, int b) {
    return SOCL_Listen(a, b);
}

int SOC_Accept(int a, struct Addr *b, int c) {
    u16 port;
    u32 ip;
    int r = SOCL_Accept(a, &port, &ip);
    if (r < 0) {
        return r;
    }
    b->port = Swap16((u16)port);
    b->ip = Swap32(ip);
    return r;
}

int SOC_Fcntl(void *a, int b, int c) {
    if (!a) {
        return -1;
    }
    switch (b) {
    case 3:
        return S(a, 0x72) == 1 ? 0 : 4;
    case 4:
        if (c & 4) {
            S(a, 0x72) = 0;
        } else {
            S(a, 0x72) = 1;
        }
        break;
    }
    return 0;
}

void *AllocFunc_SOCL(u32 a) {
    u32 size = a + 4;
    u32 *p = ((void *(*)(int, u32))P(_021D4494, 12))(0, size);
    if (p) {
        *p = size;
        p++;
    }
    return p;
}

void FreeFunc_SOCL(u32 *a) {
    if (a) {
        ((void (*)(int, void *, u32))P(_021D4494, 4))(0, a - 1, a[-1]);
    }
}

int SOC_Startup(u8 *a) {
    W(_021D4494, 0x40) = W(a, 12) == 1;
    W(_021D4494, 0x44) = Swap32(W(a, 16));
    W(_021D4494, 0x48) = Swap32(W(a, 20));
    W(_021D4494, 0x4c) = Swap32(W(a, 24));
    W(_021D4494, 0x50) = Swap32(W(a, 28));
    W(_021D4494, 0x54) = Swap32(W(a, 32));
    P(_021D4494, 0x58) = AllocFunc_SOCL;
    P(_021D4494, 0x5c) = FreeFunc_SOCL;
    W(_021D4494, 12) = W(a, 4);
    W(_021D4494, 4) = W(a, 8);
    W(_021D4494, 0x60) = 64;
    W(_021D4494, 0x70) = W(a, 0x2c);
    W(_021D4494, 0x74) = W(a, 0x30);
    return SOCL_Startup(_021D44D4);
}

int SOC_Cleanup(void) {
    return SOCL_Cleanup();
}

void *SOC_InetNtoA(u32 a, ...) {
    SOC_InetNtoP(2, &a, (char *)_021D450C, 16);
    return _021D450C;
}

int SOC_InetAtoN(char *a, u32 *b) {
    u32 ip = SOCL_InetAtoH(a);
    if (ip == 0) {
        return 0;
    }
    *b = Swap32(ip);
    return 1;
}

char *SOC_InetNtoP(int a, void *b, char *c, u32 d) {
    u32 v;
    u8 bytes[4];
    if (a != 2) {
        return 0;
    }
    if (d < 16) {
        return 0;
    }
    MI_CpuCopy8(b, &v, 4);
    SOC_U32to4U8(v, bytes);
    OS_SNPrintf(c, 16, _02110678, bytes[3], bytes[2], bytes[1], bytes[0]);
    return c;
}

void SOC_U32to4U8(u32 a, u8 *b) {
    b[0] = a >> 24;
    b[1] = a >> 16;
    b[2] = a >> 8;
    b[3] = a;
}
