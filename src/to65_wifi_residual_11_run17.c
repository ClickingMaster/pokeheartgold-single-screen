#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define H(p, o)  (*(u16 *)((u8 *)(p) + (o)))
#define SWAP(x)  ((u16)(((x) >> 8) | ((x) << 8)))
#define IP(p, o) (((u32)SWAP(H(p, o)) << 16) | (u32)SWAP(H(p, (o) + 2)))

typedef struct S {
    OSThread *thread;
    u32 wait;
    u8 state, ssl;
    u16 port;
    u32 c, tick, localIp;
    u16 remotePort, backupPort;
    u32 ip, backupIp, recv, seq;
    u16 window, mss;
    u32 ack, count;
    void *callback;
    u32 cap;
    void *buf;
    u32 len;
    u8 pad[20];
    void *writeBuf;
    u32 writeLen;
} S;

#define SOC(t) (*(S **)((u8 *)(t) + 0xa4))

typedef struct {
    u8 down, error;
    u16 mtu;
    u32 unused4;
    u16 eport, pad;
    u32 a, b, c;
    void (*fixed)(void);
    u8 d[44];
    int (*active)(void);
    u32 d4c, ip;
    u8 e[20];
    u64 rng, mul, add;
} N;

extern N _021D4620;
extern u8 _021D4B08[];
extern u8 _0211069C[];
extern u8 _021D4888[];
extern void OS_YieldThread__(void);
#pragma warn_uninitializedvar off
extern int CPSi_SslClose();
extern int CPSi_SslConnect();
extern int CPSi_SslConsume();
extern int CPSi_SslGetLength();
extern int CPSi_SslListen();
extern void *CPSi_SslRead();
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
extern u8 *receive_packet();
extern int reset_network_vars();
extern int send_arprequest();
extern int send_ip();
extern int send_ping();
extern int send_tcp();
extern int send_udp();
extern int tcp_send_rst();
extern int throw_packet();
int valid_IP(u32 a, u32 b);
int no_need_inq(u32 a);
void tcp_send_handshake(S *s, int a, int b);
void tcp_send_ack(S *s, int b);
void tcp_send_finack(S *s, int b);
S *find_socket(u8 *a, u8 *b);
void dt_rst(u8 *a, u8 *b);
void CPS_SocRegister(S *s);
void CPS_SocUnRegister(void);
void CPS_SocDatagramMode(void);
void CPS_SocBind(u16 a, u16 b, u32 ip);
void CPS_SocUse(void);
void CPS_SocRelease(void);
void CPS_SocDup(OSThread *t);
void CPSi_TcpListenRaw(S *s);
void CPS_SetUdpCallback(void *p);
void CPS_TcpListen(void);
int CPS_TcpConnect(void);
u32 CPS_SocWho(u16 *a, u32 *b);
void CPSi_TcpShutdownRaw(S *s);
void CPS_TcpShutdown(void);
void CPS_TcpClose(void);
void *udp_read_raw(u32 *p, S *s);
void *CPSi_TcpReadRaw(u32 *p, S *s);
void *CPS_SocRead(u32 *p);
void CPSi_SocConsumeRaw(u32 n, S *s);
void CPS_SocConsume(u32 n);
int tcp_write_do2(void *p, u32 a, void *q, u32 b, S *s, int z);
u32 CPS_SocWrite(void *p, u32 a);
int CPS_SocGetLength(void);
void CPS_SocFlush(void);
void set_fixed_ip(void);
u8 *pad_mem(u8 c, u32 want, u8 *p, u32 n);
u32 get_seqno(void);
void reply_icmp(u8 *a, u8 *b, u32 n);
void dispatch_icmp(u8 *a, u8 *b, u32 n);
void parse_mss(u8 *p, S *s);
S *check_listener(u8 *a, u8 *b);
int check_socket(u8 *a, u8 *b, S *s);
void dt_syn_LISTEN(u8 *a, u8 *b, S *s);
int find_specific_socket(u8 *a, u8 *b, u32 n);
void dt_syn(u8 *a, u8 *b, u32 n);
void dt_synack(u8 *a, u8 *b, u32 n);
void dt_fin(u8 *a, u8 *b, u32 n);
void tcpip(void);
int CPSi_TcpConnectRaw(S *s);
u32 CPSi_SocWrite2(void *p, u32 a, void *q, u32 b);
void process_icmp_reply(u8 *a, u8 *b, u32 n);
u16 CPS_SocGetEport(void);
u32 tcp_write_do(u8 *p, u32 len, S *s, int z);
u8 *dhcp_setcommon(u8 *p, int a, u32 *out);
void dispatch_tcp(u8 *a, u8 *b, u32 n);

int CPS_TcpConnect(void) {
    S *s = SOC(OSi_ThreadInfo.current);
    if (s) {
        if (s->ssl) {
            return CPSi_SslConnect(s);
        }
        return CPSi_TcpConnectRaw(s);
    }
    return 1;
}

u32 CPS_SocWho(u16 *a, u32 *b) {
    S *s = SOC(OSi_ThreadInfo.current);
    if (s && (s->state == 4 || s->state == 10)) {
        if (a) {
            *a = s->remotePort;
        }
        if (b) {
            *b = s->localIp;
        }
        return s->ip;
    }
    return 0;
}

void CPSi_TcpShutdownRaw(S *s) {
    OS_YieldThread();
    if ((u8)(s->state + 253) <= 1) {
        tcp_send_finack(s, 25);
        s->state = 7;
        return;
    }
    if (!s->state) {
        return;
    }
    tcp_send_ack(s, 26);
}

void CPS_TcpShutdown(void) {
    S *s = SOC(OSi_ThreadInfo.current);
    if (!s) {
        return;
    }
    if (s->ssl) {
        CPSi_SslShutdown(s);
    }
    CPSi_TcpShutdownRaw(s);
}

void CPS_TcpClose(void) {
    S *s = SOC(OSi_ThreadInfo.current);
    int tick;
    if (!s) {
        return;
    }
    if (s->ssl) {
        CPSi_SslClose(s);
    }
    tick = OS_GetTick() >> 16;
    while (_021D4620.active() && s->state && ((int)(OS_GetTick() >> 16) - tick) < 39) {
        OS_YieldThread__();
    }
    s->state = 0;
}

void *udp_read_raw(u32 *p, S *s) {
    u32 n;
    OSIntrMode mode = OS_DisableInterrupts();
    n = s->len;
    while (!n) {
        s->wait = 3;
        OS_SleepThread(0);
        n = s->len;
    }
    OS_RestoreInterrupts(mode);
    *p = n;
    return s->buf;
}

void *CPSi_TcpReadRaw(u32 *p, S *s) {
    if (!s->len && s->state == 4) {
        OSIntrMode mode = OS_DisableInterrupts();
        while (!s->len && s->state == 4) {
            s->wait = 2;
            OS_SleepThread(0);
        }
        OS_RestoreInterrupts(mode);
    } else {
        OS_YieldThread();
    }
    *p = s->len;
    return *p ? s->buf : 0;
}

void *CPS_SocRead(u32 *p) {
    S *s = SOC(OSi_ThreadInfo.current);
    if (s) {
        if ((u8)(s->state + 246) <= 1) {
            return udp_read_raw(p, s);
        }
        if (s->ssl) {
            return CPSi_SslRead(p, s);
        }
        return CPSi_TcpReadRaw(p, s);
    }
    *p = 0;
    return 0;
}

void CPSi_SocConsumeRaw(u32 n, S *s) {
    OSIntrMode mode = OS_DisableInterrupts();
    int full = 0;
    if (s->len == s->cap && n) {
        full = 1;
    }
    if (n >= s->len) {
        s->len = 0;
    } else {
        u8 *p = s->buf;
        u32 left = s->len - n;
        s->len = left;
        memmove(p, p + n, left);
    }
    OS_RestoreInterrupts(mode);
    switch (s->state) {
    case 10:
    case 11:
        return;
    }
    if (s->len && !full) {
        return;
    }
    tcp_send_ack(s, 27);
}

void CPS_SocConsume(u32 n) {
    S *s = SOC(OSi_ThreadInfo.current);
    if (!s) {
        return;
    }
    if (s->ssl) {
        CPSi_SslConsume(n, s);
    } else {
        CPSi_SocConsumeRaw(n, s);
    }
}
