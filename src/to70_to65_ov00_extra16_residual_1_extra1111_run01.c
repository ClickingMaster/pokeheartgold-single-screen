#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct W {
    u8 p[0x10];
    void *net;
    u8 p14[0x2bc];
    u8 peers[1];
} W;

typedef struct Header {
    u8 zero, kind;
    u16 port;
    u32 address;
} Header;

extern W *ov00_021F955C(void);
extern const u8 ov00_02219D28[];
extern int memcmp(const void *, const void *, u32);
extern void ov00_02211A88(void *, u8 *, u32, Header *);
extern void ov00_0221075C(u8 *, u32, Header *);
int ov00_021F2880(void *, u32, int, u8 *, u32);
#define WORD(w, o) (*(u32 *)((u8 *)(w) + (o)))
#define BYTE(w, o) (*(u8 *)((u8 *)(w) + (o)))
#define HALF(w, o) (*(u16 *)((u8 *)(w) + (o)))

typedef struct Peer {
    u8 id, value;
    u16 zero;
    u32 extra;
} Peer;

extern const u8 ov00_02217A74[];
extern const u8 ov00_02217A80[];
extern const u8 ov00_02217A8C[];
extern int ov00_021EE6D0(void);
extern int ov00_0220D000(void *, u32);
extern void *ov00_021EE72C(int);
extern Peer *ov00_021EE7A4(int);
extern u32 strtoul(const char *, char **, int);
extern void ov00_0220D00C(void *, const void *, int);
extern void ov00_021F2EE4(int, int);
extern void ov00_021F2EC4(void);
extern void ov00_0220D2A4(void *, Peer *, int);
extern void ov00_021F64C0(int);
void ov00_021F2974(void *, void *, u32, int, int, u8 *);

void ov00_021F2974(void *unused, void *socket, u32 address, int port, int budget, u8 *option) {
    int slot;
    W *w;
    Peer *peer;
    void **entry;
    if (ov00_021F955C() == 0 || WORD(ov00_021F955C(), 0x1a0) != 7 || BYTE(ov00_021F955C(), 0x1a9) != 0) {
        ov00_0220D00C(socket, ov00_02217A74, -1);
        return;
    }
    slot = ov00_021EE6D0();
    if (slot == -1) {
        ov00_0220D00C(socket, ov00_02217A80, -1);
        ov00_021F2EE4(6, -87100);
        return;
    }
    w = ov00_021F955C();
    if (address != WORD((u8 *)w + BYTE(ov00_021F955C(), 0xd) * 4, 0x210)) {
        goto different;
    }
    w = ov00_021F955C();
    if (port == HALF((u8 *)w + BYTE(ov00_021F955C(), 0xd) * 2, 0x290)) {
        goto accepted;
    }
different:
    if (option[0]) {
        u32 value = strtoul((char *)option, 0, 10);
        W *left = ov00_021F955C();
        if (value == WORD((u8 *)left + BYTE(ov00_021F955C(), 0xd) * 4, 0xf4)) {
            left = ov00_021F955C();
            WORD((u8 *)left + BYTE(ov00_021F955C(), 0xd) * 4, 0x210) = address;
            left = ov00_021F955C();
            HALF((u8 *)left + BYTE(ov00_021F955C(), 0xd) * 2, 0x290) = port;
            goto accepted;
        }
    }
    ov00_0220D00C(socket, ov00_02217A8C, -1);
    return;
accepted:
    w = ov00_021F955C();
    WORD(w, 0x18c) = 0;
    WORD(w, 0x190) = 0;
    if (ov00_0220D000(socket, WORD(ov00_021F955C(), 8)) == 0) {
        ov00_021F2EE4(6, -80410);
        return;
    }
    ov00_021F2EC4();
    if (BYTE(ov00_021F955C(), 0xd) == 0) {
        int half = budget >> 1;
        if (half >= 65535) {
            half = 65535;
        }
        HALF(ov00_021F955C(), 0x1b0) = half;
    }
    entry = ov00_021EE72C(slot);
    peer = ov00_021EE7A4(slot);
    *entry = socket;
    w = ov00_021F955C();
    BYTE(w, 0xd)
    ++;
    peer->id = slot;
    w = ov00_021F955C();
    peer->value = w->peers[BYTE(ov00_021F955C(), 0xd) - 1];
    peer->zero = 0;
    peer->extra = 0;
    ov00_0220D2A4(socket, peer, 0);
    ov00_021F64C0(2);
}
