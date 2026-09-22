#include "global.h"

typedef struct Ov112AA4 {
    u8 pad00[0x20];
    void *ptr20;
    void *ptr24;
    u8 pad28[4];
    void *ptr2C;
    void *ptr30;
    u8 pad34[4];
    void *ptr38;
    u64 tick3C;
    u8 pad44[0xC];
    void *ptr50;
    u8 pad54[4];
    void *ptr58;
} Ov112AA4;
typedef struct Ov112B24 {
    u8 pad00[0x1C];
    u8 state1C;
    u8 state1D;
    u8 pad1E[2];
    u8 *ptr20;
    u8 *ptr24;
    u32 pos28;
    u32 pos2C;
    u16 count30;
    u16 count32;
    u16 field34;
    u16 index36;
    u16 field38;
    u16 index3A;
} Ov112B24;
extern Ov112AA4 ov112_021FFAA4;
extern u8 *ov112_021FFAF0[];
extern u8 *ov112_021FFAF8[];
extern Ov112B24 ov112_021FFB24;

extern void OS_Halt(void);
extern u64 OS_GetTick(void);
extern void ov112_021E5A5C(void);
extern void ov112_021E5A68(void *, int, int, int);
extern void ov112_021E5E18(void *);
extern void ov112_021E5EC4(void *, u32, int);
extern void ov112_021E6134(void);
extern u64 _ll_udiv(u64, u64);
extern float _f_ulltof(u64);
extern float _fdiv(float, float);
void ov112_021E5EEC(void);
void ov112_021E6004(void);

void ov112_021E5EEC(void) {
    u8 packet[0x80];
    u16 pos = ov112_021FFB24.pos28;
    u16 count = ov112_021FFB24.count30;
    u8 **table;
    u8 length;
    if (count > 0x80) {
        count = 0x80;
    }
    table = NULL;
    switch (ov112_021FFB24.state1D) {
    case 0x3A:
        if (ov112_021FFAA4.ptr50 != 0) {
            table = (u8 **)ov112_021FFAF0;
        }
        break;
    case 0x3C:
        if (ov112_021FFAA4.ptr58 != 0) {
            table = (u8 **)ov112_021FFAF8;
        }
        break;
    }
    if (table == NULL) {
        if ((pos & 0x7F) != 0) {
            count = 0x80 - (pos & 0x7F);
            packet[0] = pos;
            MI_CpuCopy8(ov112_021FFB24.ptr20, packet + 1, count);
            ov112_021E5A68(packet, (u8)(count + 1), 0xA, (u8)(pos >> 8));
        } else {
            ov112_021E5A68(ov112_021FFB24.ptr20, (u8)count, (u8)(2 | (pos & 0x80)), (u8)(pos >> 8));
        }
    } else {
        length = (*table)[ov112_021FFB24.index36];
        if (length > 0x80) {
            OS_Halt();
        }
        if (length == 0x80) {
            ov112_021E5A68(ov112_021FFB24.ptr20, 0x80, (u8)(2 | (pos & 0x80)), (u8)(pos >> 8));
        } else {
            ov112_021E5A68(ov112_021FFB24.ptr20, length, (u8)(pos & 0x80), (u8)(pos >> 8));
        }
    }
    ov112_021FFB24.ptr20 = (u8 *)ov112_021FFB24.ptr20 + count;
    ov112_021FFB24.pos28 += count;
    ov112_021FFB24.count30 -= count;
    (ov112_021FFB24.index36)++;
    (ov112_021FFB24.index3A)++;
}

void ov112_021E6004(void) {
    u64 elapsed = OS_GetTick() - ov112_021FFAA4.tick3C;
    _fdiv(_f_ulltof(_ll_udiv(elapsed << 6, 0x82EA)), 1000.0f);
    switch (ov112_021FFB24.state1D) {
    case 0x3A:
        ov112_021FFB24.state1D = 0x3C;
        if (ov112_021FFAA4.ptr58 != NULL) {
            ov112_021E5EC4(ov112_021FFAA4.ptr58, 0xD700, 0x28BE);
        } else {
            ov112_021E5EC4(ov112_021FFAA4.ptr2C, 0xD700, 0x28BE);
        }
        ov112_021E5EEC();
        break;
    case 0x3C:
        ov112_021FFB24.state1D = 0x3E;
        MI_CpuCopy8(ov112_021FFAA4.ptr24 + 0x10,
            ov112_021FFAA4.ptr30 + 8,
            0x28);
        ov112_021E5EC4(ov112_021FFAA4.ptr30, 0xD480, 0x224);
        ov112_021E5EEC();
        break;
    case 0x3E:
        if (ov112_021FFB24.state1C == 0x52) {
            ov112_021FFB24.state1D = 0x48;
            ov112_021E5EC4((void *)0xCE80, (u32)ov112_021FFAA4.ptr38, 0xD4C);
            ov112_021E6134();
        } else {
            ov112_021E5A68(NULL, 0, 0x24, 1);
        }
        break;
    case 0xB0:
    case 0xB2:
    case 0xB4:
    case 0xB6:
    case 0xB8:
    case 0xBA:
    case 0xBC:
    case 0xBE:
        ov112_021E5A68(NULL, 0, 0x24, 1);
        break;
    default:
        ov112_021E5E18((void *)0xC);
        ov112_021E5A5C();
        break;
    }
}
