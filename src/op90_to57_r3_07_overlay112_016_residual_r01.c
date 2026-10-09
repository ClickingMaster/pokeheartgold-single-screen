#pragma force_active on
#include "global.h"

/* IR transfer work area: MWCC -ipa file pools these four file objects into one
   .bss block (heap-sorted by size) at 0x021FFAA4..0x021FFBD8. */
typedef struct Ov112IrHeader {
    u16 status00;
    u8 pad02[6];
    u8 *ptr08;
    u8 pad0C[4];
    u8 *ptr10;
    u32 field14;
    u32 field18;
} Ov112IrHeader; /* 0x1C */

typedef struct Ov112IrCore {
    OSMessage messages[1]; /* 0x021FFAC0 */
    void *field20;
    void *field24;
    void *field28;
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
} Ov112IrCore; /* 0x48 */

typedef struct Ov112IrComm {
    OSMutex mutex;         /* 0x021FFB08 */
    OSMessageQueue queue;  /* 0x021FFB20 */
    u8 command1C;          /* 0x021FFB40 */
    u8 command1D;
    u8 pad1E[2];
    u8 *ptr20;
    u8 pad24[0x12];
    u16 index36;
} Ov112IrComm; /* 0x54 */

typedef struct Ov112IrTransfer {
    u16 div38;             /* 0x021FFB5C */
    u16 progress3A;
    u16 total3C;
    u16 pad3E;
    u8 buffer[0x74];       /* 0x021FFB64 */
} Ov112IrTransfer; /* 0x7C */

Ov112IrHeader ov112_021FFAA4;
Ov112IrCore ov112_021FFAC0;
Ov112IrComm ov112_021FFB08;
Ov112IrTransfer ov112_021FFB5C;

void ov112_021E5EC4(void *, u32, int);
void ov112_021E5D8C(u8 *src, u8 *dst, u16 size, u8 *out);
void ov112_021E74A8(u8 *a, u8 *b, u8 *c);
void ov112_021E7548(u8 *a, u8 *b, u8 *c, u8 *d);
void ov112_021E7594(u8 *a, u8 *b, u8 *c, u8 *d);

// @fn ov112_021E74A8
void ov112_021E74A8(u8 *a, u8 *b, u8 *c) {
    ov112_021FFAA4.ptr10 = a;
    ov112_021FFAC0.ptr2C = b;
    ov112_021FFAC0.ptr30 = c;
    ov112_021FFB08.command1C = 0x32;
    ov112_021FFB08.command1D = 0x20;
    ov112_021E5EC4(NULL, 0, 0xD4C);
    ov112_021FFB5C.total3C = 0xB732;
    ov112_021FFB5C.progress3A = 0;
    ov112_021FFB5C.div38 = (ov112_021FFB5C.total3C + 0x7F) / 0x80;
    if (ov112_021FFAC0.ptr48 != NULL) {
        ov112_021FFAC0.ptr4C = ov112_021FFAC0.ptr44;
        ov112_021FFAC0.ptr50 = ov112_021FFAC0.ptr48;
        ov112_021E5D8C(ov112_021FFAA4.ptr10, ov112_021FFAC0.ptr50, 0x8C50, ov112_021FFAC0.ptr4C);
        ov112_021FFAC0.ptr54 = ov112_021FFAC0.ptr44 + 0x119;
        ov112_021FFAC0.ptr58 = ov112_021FFAC0.ptr48 + 0x8C80;
        ov112_021E5D8C(ov112_021FFAC0.ptr2C, ov112_021FFAC0.ptr58, 0x28BE, ov112_021FFAC0.ptr54);
        ov112_021FFAC0.ptr5C = ov112_021FFAC0.ptr54 + 0x52;
        ov112_021FFAC0.ptr60 = ov112_021FFAC0.ptr58 + 0x2900;
        ov112_021E5D8C(ov112_021FFAC0.ptr30, ov112_021FFAC0.ptr60, 0x224, ov112_021FFAC0.ptr5C);
    }
}

// @fn ov112_021E7548
void ov112_021E7548(u8 *a, u8 *b, u8 *c, u8 *d) {
    ov112_021FFAA4.ptr08 = a;
    ov112_021FFAC0.ptr38 = b;
    ov112_021FFAC0.ptr34 = c;
    ov112_021FFAC0.ptr30 = d;
    ov112_021FFB08.command1C = 0x40;
    ov112_021FFB08.command1D = 0x20;
    ov112_021E5EC4(NULL, 0, 0xD4C);
    ov112_021FFB5C.total3C = 0x2BCA;
    ov112_021FFB5C.progress3A = 0;
    ov112_021FFB5C.div38 = (ov112_021FFB5C.total3C + 0x7F) / 0x80;
}

// @fn ov112_021E7594
void ov112_021E7594(u8 *a, u8 *b, u8 *c, u8 *d) {
    ov112_021FFAC0.ptr2C = a;
    ov112_021FFAC0.ptr30 = b;
    ov112_021FFAC0.ptr38 = c;
    ov112_021FFAC0.ptr34 = d;
    ov112_021FFB08.command1C = 0x52;
    ov112_021FFB08.command1D = 0x20;
    ov112_021E5EC4(NULL, 0, 0xD4C);
    ov112_021FFB5C.total3C = 0x382E;
    ov112_021FFB5C.progress3A = 0;
    ov112_021FFB5C.div38 = (ov112_021FFB5C.total3C + 0x7F) / 0x80;
    if (ov112_021FFAC0.ptr48 != NULL) {
        ov112_021FFAC0.ptr54 = ov112_021FFAC0.ptr44 + 0x119;
        ov112_021FFAC0.ptr58 = ov112_021FFAC0.ptr48 + 0x8C80;
        ov112_021E5D8C(ov112_021FFAC0.ptr2C, ov112_021FFAC0.ptr58, 0x28BE, ov112_021FFAC0.ptr54);
        ov112_021FFAC0.ptr5C = ov112_021FFAC0.ptr54 + 0x52;
        ov112_021FFAC0.ptr60 = ov112_021FFAC0.ptr58 + 0x2900;
        ov112_021E5D8C(ov112_021FFAC0.ptr30, ov112_021FFAC0.ptr60, 0x224, ov112_021FFAC0.ptr5C);
    }
}
