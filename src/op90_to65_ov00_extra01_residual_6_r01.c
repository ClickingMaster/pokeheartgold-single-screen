#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 v[6];
} Magic;

typedef struct {
    Magic magic;
    u8 type, pad;
    u32 ip;
    u8 code, tag, flag;
    u8 random[4];
    u8 port[2];
    char name[64];
} Packet;

typedef struct {
    int socket, peer;
    u32 ip, tag, pad, first, second, third, unused, ticks, deadline;
} Work;

extern const Magic ov00_02219D28;
extern const char ov00_0221B16C[];
extern u32 ov00_0221B664[];
extern char *strcpy(char *, const char *);
extern u32 ov00_0220FBE4(void);
extern u32 ov00_021FF368(void);
extern int ov00_0220FB90(int, int, int, void *, int);
extern u16 ov00_0220FC48(int);

static inline u32 swap32(u32 n) {
    return ((n >> 24) & 0xff) | ((n >> 8) & 0xff00) | ((n << 8) & 0xff0000) | ((n << 24) & 0xff000000);
}

void ov00_0220FC7C(Work *);

void ov00_0220FC7C(Work *w) {
    u8 buf[0x58];
    Packet *packet = (Packet *)buf;
    u32 a, b, c, d;
    int len;
    u16 port;
    memcpy(&packet->magic, &ov00_02219D28, sizeof(Magic));
    packet->type = 2;
    packet->pad = 0;
    packet->tag = w->tag;
    packet->ip = swap32(w->ip);
    packet->flag = w->peer != -1;
    a = ov00_0220FBE4();
    b = ov00_0220FBE4();
    c = ov00_0220FBE4();
    d = ov00_0220FBE4();
    d = ((c >> 24) & 0xff) | ((d >> 8) & 0xff00) | ((b << 8) & 0xff0000) | ((a << 24) & 0xff000000);
    buf[15] = d >> 24;
    buf[16] = d >> 16;
    buf[17] = d >> 8;
    buf[18] = d;
    buf[19] = 0;
    buf[20] = 0;
    strcpy((char *)&buf[21], ov00_0221B16C);
    len = strlen(ov00_0221B16C) + 22;
    if (packet->flag && w->first == 0) {
        packet->code = 0;
        ov00_0220FB90(w->peer, ov00_0221B664[3], 0x6cfd, packet, len);
    }
    if (w->second == 0) {
        packet->code = 1;
        ov00_0220FB90(w->socket, ov00_0221B664[3], 0x6cfd, packet, len);
    }
    port = (u16)(((ov00_0220FC48(packet->flag ? w->peer : w->socket) >> 8) & 0xff) | ((ov00_0220FC48(packet->flag ? w->peer : w->socket) << 8) & 0xff00));
    buf[20] = port;
    buf[19] = port >> 8;
    if (w->third == 0) {
        packet->code = 2;
        ov00_0220FB90(w->socket, ov00_0221B664[2], 0x6cfd, packet, len);
    }
    w->deadline = ov00_021FF368() + 500;
    w->ticks = 30;
}
