#include "global.h"

typedef struct Ov112Core {
    u8 pad00[8];
    void *ptr08;
    u8 pad0C[4];
    void *ptr10;
    u8 pad14[0x18];
    void *ptr2C;
    void *ptr30;
    void *ptr34;
    void *ptr38;
    u64 tick3C;
    void *ptr44;
    void *ptr48;
    void *ptr4C;
    void *ptr50;
    void *ptr54;
    void *ptr58;
    void *ptr5C;
    void *ptr60;
} Ov112Core;

typedef struct Ov112Packet {
    u8 pad00[0x1C];
    u8 command1C;
    u8 command1D;
    u8 pad1E[0x1A];
    u16 div38;
    u16 progress3A;
    volatile u16 total3C;
} Ov112Packet;

extern Ov112Core ov112_021FFAA4;
extern volatile Ov112Packet ov112_021FFB24;
void ov112_021E5EC4(void *, u32, int);
void ov112_021E5D8C(void *, void *, int);
void ov112_021E7484(int);
void ov112_021E74A8(void *, void *, void *);
void ov112_021E7548(void *, void *, void *, void *);
void ov112_021E7594(void *, void *, void *, void *);
void ov112_021E7620(void *, void *);

#define SET_PACKET_PROGRESS(total)                                     \
    do {                                                               \
        ov112_021FFB24.total3C = (total);                              \
        ov112_021FFB24.progress3A = 0;                                 \
        ov112_021FFB24.div38 = (ov112_021FFB24.total3C + 0x7F) / 0x80; \
    } while (0)

void ov112_021E7484(int active) {
    ov112_021FFB24.command1C = 0x2A;
    if (active != 0) {
        ov112_021FFB24.command1D = 0x2A;
    } else {
        ov112_021FFB24.command1D = 0x2C;
    }
    ov112_021E5EC4(NULL, 0, 0x28);
}
