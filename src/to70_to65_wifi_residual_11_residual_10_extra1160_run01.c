#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int maccmp(void *, void *);
extern void reg_arpcache(void *, u32, int);
extern void reply_arp(void *);
extern u32 get_targetip(u32);
extern void *inq_arpcache(u32);
extern void send_arprequest(u32);
extern int calc_checksum(void *, u32);
extern void send_ip(void *, u32, int, int, u32, int);
extern u8 _021D4B08[];

typedef struct {
    u8 pad, duplicate;
    u8 p[0x4e];
    u32 ip;
} State;

extern State _021D4620;

static inline u16 swap16(u16 value) {
    int x = value;
    return (x >> 8) | (x << 8);
}

static inline u32 readIP(const u16 *p) {
    u32 lo = swap16(p[1]), hi = swap16(p[0]);
    return (hi << 16) | lo;
}

typedef struct {
    u16 hardware, protocol, sizes, operation;
    u8 mac[6];
    u16 source[2];
    u8 targetmac[6];
    u16 target[2];
} Arp;

typedef struct {
    u8 p[12];
    u16 source[2];
} Ip;

typedef struct {
    u8 type, code;
    u16 checksum, id, sequence;
} Icmp;

typedef struct {
    OSThread *waiting;
    u32 state;
    u8 type, pad;
    u16 id;
    u8 p[0x1c - 12];
    u32 ip;
    u8 p2[0x3c - 0x20];
    u32 max;
    void *dst;
    volatile u32 size;
} Socket;

static inline void setReplySize(u32 amount, Socket *s) {
    if (amount > s->max) {
        s->size = s->max;
    } else {
        s->size = amount;
    }
}

typedef struct Thread {
    u8 p[0x68];
    struct Thread *next;
    u8 p2[0xa4 - 0x6c];
    Socket *socket;
} Thread;

void dispatch_arp(Arp *, u32);
void reply_icmp(Ip *, Icmp *, u32);
void process_icmp_reply(Ip *, Icmp *, u32);

void dispatch_arp(Arp *p, u32 length) {
    u16 operation;
    u32 source, target;
    int self, ours;
    if (length < 28) {
        return;
    }
    if (maccmp(p->mac, _021D4B08) == 0 || ((volatile State *)&_021D4620)->ip == 0) {
        return;
    }
    if (p->hardware != 0x100 || p->protocol != 8 || p->sizes != 0x406) {
        return;
    }
    operation = swap16(p->operation);
    if (operation != 1 && operation != 2) {
        return;
    }
    source = readIP(p->source);
    target = readIP(p->target);
    self = source == _021D4620.ip;
    ours = _021D4620.ip == target;
    if (!self) {
        reg_arpcache(p->mac, source, ours);
    }
    if (operation == 1 && ours) {
        reply_arp(p);
        return;
    }
    if (operation == 2 && ours && self) {
        _021D4620.duplicate = 1;
    }
}

void reply_icmp(Ip *ip, Icmp *icmp, u32 length) {
    u32 target = get_targetip(readIP(ip->source));
    int checksum;
    if (!target) {
        return;
    }
    if (!inq_arpcache(target)) {
        send_arprequest(target);
        return;
    }
    icmp->type = 0;
    icmp->checksum = 0;
    checksum = calc_checksum(icmp, length);
    icmp->checksum = (checksum >> 8) | (checksum << 8);
    send_ip(icmp, length, 0, 0, readIP(ip->source), 1);
}

void process_icmp_reply(Ip *ip, Icmp *icmp, u32 length) {
    Socket *s;
    OSIntrMode mode = OS_DisableInterrupts();
    Thread *t = *(Thread **)((u8 *)&OSi_ThreadInfo + 8);
    while (t) {
        s = t->socket;
        if (s && s->waiting && s->type == 11 && icmp->id == (u16)(u32)s->waiting && s->id == icmp->sequence && s->size == 0 && s->ip == readIP(ip->source)) {
            setReplySize(length - 8, s);
            MI_CpuCopy8((u8 *)icmp + 8, s->dst, s->size);
            if (s->state == 3) {
                s->state = 0;
                OS_WakeupThreadDirect(s->waiting);
            }
            break;
        }
        t = t->next;
    }
    OS_RestoreInterrupts(mode);
}
