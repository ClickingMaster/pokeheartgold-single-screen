#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#pragma opt_strength_reduction off

typedef struct Packet {
    struct Packet *next;
    u32 flags, other;
    u8 data[0x46c];
} Packet;

typedef char PacketExtent[(sizeof(Packet) == 1144) ? 1 : -1];
extern Packet *_021D6FE4[2];

static inline void ConstructLinks(u32 count) {
    u32 offset, index, remaining;
    remaining = count - 1;
    index = 0;
    if (remaining) {
        offset = 0;
        do {
            Packet *base = _021D6FE4[1];
            u8(*arena)[count * sizeof(Packet)] = (void *)base;
            Packet *current = (Packet *)&(*arena)[offset];
            index++;
            current->next = base + index;
            offset += sizeof(Packet);
        } while (index < remaining);
    }
    _021D6FE4[1][count - 1].next = NULL;
    _021D6FE4[0] = _021D6FE4[1];
}

s32 vct_init_packet_buffer(void *, u32);

s32 vct_init_packet_buffer(void *buffer, u32 bytes) {
    u32 count = bytes / sizeof(Packet);
    _021D6FE4[1] = buffer;
    if (count < 4 || count > 72) {
        return 0;
    }
    if (!buffer) {
        return 0;
    }
    if ((u32)buffer & 31) {
        _021D6FE4[1] = NULL;
        return 0;
    }
    MIi_CpuClearFast(0, buffer, bytes);
    ConstructLinks(count);
    return 1;
}
