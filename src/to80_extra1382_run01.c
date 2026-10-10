#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 pad[14];
    u16 count;
    u8 *entries[1];
} Packet;

typedef struct {
    u8 bytes[6], active;
} Peer;

typedef struct {
    u8 pad0[0x1300];
    Peer peers[16];
    u8 pad1[0];
    u8 pad2[0x1370 - 0x1370];
    void (*callback)(int);
    u8 pad3[0x1b95 - 0x1374];
    u8 active;
} Work;

extern Work *ov13_0224F5B4;
extern const char ov13_02245828[];
extern int memcmp(const void *, const void *, u32);
void ov13_0223BC24(Packet *);

void ov13_0223BC24(Packet *pkt) {
    u8 *data;
    int i, j;
    Peer *peer;
    Work *work;
    int count;
    if (ov13_0224F5B4->active) {
        if (ov13_0224F5B4->callback) {
            ov13_0224F5B4->callback(ov13_0224F5B4->active);
        }
        return;
    }
    DC_InvalidateRange((char *)ov13_0224F5B4 + 0xf00, 0x400);
    count = pkt->count;
    i = 0;
    if (count <= 0) {
        return;
    }
    work = ov13_0224F5B4;
    do {
        data = pkt->entries[i];
        if (memcmp(data + 12, ov13_02245828, 8) == 0 && (data[21] & 1)) {
            for (j = 0, peer = work->peers; j < 16; j++, peer++) {
                if (memcmp(data + 4, peer->bytes, 6) == 0) {
                    if (work->peers[j].active) {
                        break;
                    }
                    if (!work->callback) {
                        work->active = 1;
                        return;
                    }
                    work->callback(1);
                    return;
                }
            }
        }
    } while (++i < count);
}
