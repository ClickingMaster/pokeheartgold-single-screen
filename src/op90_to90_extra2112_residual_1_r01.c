#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef void (*StateCallback)(int state, void *param);
typedef struct {
    int sock;
    int mode;
    u8 pad8[8];
    int ready;
    int got[3];
    u8 pad20[8];
    u32 deadline;
    u8 pad2c[8];
    StateCallback cb;
    u8 pad38[4];
    void *param;
} Nat;
typedef struct {
    u8 pad0[7];
    u8 type;
    u8 pad8[4];
    u8 index;
} NatPacket;
typedef struct {
    u8 pad0[2];
    u16 port;
    u32 addr;
} NatAddr;
u32 ov00_021FF368(void);
void ov00_0220FB90(int sock, u32 addr, u16 port, NatPacket *pkt, int len);
void ov00_02210658(Nat *nat, NatPacket *pkt, NatAddr *from);
// @fn ov00_02210658
void ov00_02210658(Nat *nat, NatPacket *pkt, NatAddr *from) {
    switch (pkt->type) {
    case 1:
        if (pkt->index > 2) {
            return;
        }
        nat->got[pkt->index] = 1;
        if (nat->ready != 0) {
            return;
        }
        if (nat->got[1] == 0 || nat->got[2] == 0) {
            return;
        }
        if (nat->mode != -1 && nat->got[0] == 0) {
            return;
        }
        nat->ready = 1;
        nat->deadline = ov00_021FF368() + 10000;
        nat->cb(nat->ready, nat->param);
        break;
    case 2:
        pkt->type = 3;
        ov00_0220FB90(nat->sock, from->addr, (u16)(((from->port >> 8) & 0xff) | ((from->port << 8) & 0xff00)), pkt, 0x15);
        break;
    }
}
