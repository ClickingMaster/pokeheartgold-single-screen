#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u8 magic[4];
    int type;
    u8 kind;
    u8 len;
    u16 port;
    u32 addr;
    u32 id;
    u32 data[32];
} Packet;
typedef struct {
    u8 pad0[0x1a];
    u16 port;
    u32 addr;
    u8 pad20[0xe4 - 0x20];
    void *sock;
    u8 pade8[0x200 - 0xe8];
    u32 id;
} Session;
extern const char ov00_02217B54[];
Session *ov00_021F955C(void);
u32 ov00_0220F724(void *a, int b, int c);
int ov00_0221386C(void *sock, u32 addr, u16 port, Packet *pkt, int len);
void ov00_021F44C8(u8 kind, void *peer, u16 port, const u32 *data, u32 count);
// @fn ov00_021F44C8
void ov00_021F44C8(u8 kind, void *peer, u16 port, const u32 *data, u32 count) {
    Packet pkt;
    int i;
    int ret;
    if (count > 32) {
        count = 32;
    }
    if (data == NULL || count == 0) {
        count = 0;
    } else {
        MIi_CpuCopy32(data, pkt.data, count * 4);
    }
    memcpy(pkt.magic, ov00_02217B54, 5);
    pkt.type = 3;
    pkt.kind = kind;
    pkt.len = count * 4;
    pkt.port = ov00_021F955C()->port;
    pkt.addr = ov00_021F955C()->addr;
    pkt.id = ov00_021F955C()->id;
    for (i = 0; i < 5; i++) {
        ret = ov00_0221386C(ov00_021F955C()->sock, ov00_0220F724(peer, 0, 0), port, &pkt, pkt.len + 0x14);
        if (ret == 0) {
            return;
        }
        if (ret != 2) {
            return;
        }
    }
}
