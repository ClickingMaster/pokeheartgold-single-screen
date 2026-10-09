#pragma force_active on
#include "global.h"

typedef struct Ov112Core {
    u8 pad00[8];
    u8 *ptr08;
    u8 pad0C[4];
    u8 *ptr10;
    u8 pad14[0x18];
    u8 *ptr2C;
    u8 *ptr30;
    u8 *ptr34;
    u8 *ptr38;
    u64 tick3C;
    u8 *ptr44;
    u8 *ptr48;
    u8 *ptr4C;
    u8 *ptr50;
    u8 *ptr54;
    u8 *ptr58;
    u8 *ptr5C;
    u8 *ptr60;
} Ov112Core;

typedef struct Ov112Packet {
    u8 pad00[0x1C];
    u8 command1C;
    u8 command1D;
    u8 pad1E[0x1A];
    u16 div38;
    u16 progress3A;
    u16 total3C;
} Ov112Packet;

typedef struct Ov112State {
    Ov112Core core;
    u8 pad64[0x80 - sizeof(Ov112Core)];
    Ov112Packet packet;
} Ov112State;
extern Ov112State ov112_021FFAA4;
void ov112_021E5EC4(void *, u32, int);
void ov112_021E5D8C(u8 *src, u8 *dst, u16 size, u8 *out);
void ov112_021E74A8(u8 *a, u8 *b, u8 *c);
void ov112_021E7548(u8 *a, u8 *b, u8 *c, u8 *d);
void ov112_021E7594(u8 *a, u8 *b, u8 *c, u8 *d);

// @fn ov112_021E74A8
void ov112_021E74A8(u8 *a, u8 *b, u8 *c) {
    ov112_021FFAA4.core.ptr10 = a;
    ov112_021FFAA4.core.ptr2C = b;
    ov112_021FFAA4.core.ptr30 = c;
    ov112_021FFAA4.packet.command1C = 0x32;
    ov112_021FFAA4.packet.command1D = 0x20;
    ov112_021E5EC4(NULL, 0, 0xD4C);
    ov112_021FFAA4.packet.total3C = 0xB732;
    ov112_021FFAA4.packet.progress3A = 0;
    ov112_021FFAA4.packet.div38 = (ov112_021FFAA4.packet.total3C + 0x7F) / 0x80;
    if (ov112_021FFAA4.core.ptr48 != NULL) {
        ov112_021FFAA4.core.ptr4C = ov112_021FFAA4.core.ptr44;
        ov112_021FFAA4.core.ptr50 = ov112_021FFAA4.core.ptr48;
        ov112_021E5D8C(ov112_021FFAA4.core.ptr10, ov112_021FFAA4.core.ptr50, 0x8C50, ov112_021FFAA4.core.ptr4C);
        ov112_021FFAA4.core.ptr54 = ov112_021FFAA4.core.ptr44 + 0x119;
        ov112_021FFAA4.core.ptr58 = ov112_021FFAA4.core.ptr48 + 0x8C80;
        ov112_021E5D8C(ov112_021FFAA4.core.ptr2C, ov112_021FFAA4.core.ptr58, 0x28BE, ov112_021FFAA4.core.ptr54);
        ov112_021FFAA4.core.ptr5C = ov112_021FFAA4.core.ptr54 + 0x52;
        ov112_021FFAA4.core.ptr60 = ov112_021FFAA4.core.ptr58 + 0x2900;
        ov112_021E5D8C(ov112_021FFAA4.core.ptr30, ov112_021FFAA4.core.ptr60, 0x224, ov112_021FFAA4.core.ptr5C);
    }
}

// @fn ov112_021E7548
void ov112_021E7548(u8 *a, u8 *b, u8 *c, u8 *d) {
    ov112_021FFAA4.core.ptr08 = a;
    ov112_021FFAA4.core.ptr38 = b;
    ov112_021FFAA4.core.ptr34 = c;
    ov112_021FFAA4.core.ptr30 = d;
    ov112_021FFAA4.packet.command1C = 0x40;
    ov112_021FFAA4.packet.command1D = 0x20;
    ov112_021E5EC4(NULL, 0, 0xD4C);
    ov112_021FFAA4.packet.total3C = 0x2BCA;
    ov112_021FFAA4.packet.progress3A = 0;
    ov112_021FFAA4.packet.div38 = (ov112_021FFAA4.packet.total3C + 0x7F) / 0x80;
}
