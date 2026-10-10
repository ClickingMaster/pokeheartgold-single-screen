#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 type, pad;
    u16 a, b;
} Parent;

typedef struct {
    u8 type, pad;
    u16 value;
    u8 tail[1];
} Child;

typedef struct {
    u32 p;
    u8 *buffer;
    u32 piece;
    int count;
} Global;

extern Global ov13_0224CF84;
u8 *MBi_MakeParentSendBuffer(Parent *, u8 *);
u8 *MBi_SetRecvBufferFromChild(u8 *, Child *, int);
u8 *MBi_ReceiveRequestDataPiece(Child *, int);
int IsGetAllRequestData(int);

u8 *MBi_MakeParentSendBuffer(Parent *p, u8 *out) {
    u8 *next = out;
    *next++ = p->type;
    switch (p->type) {
    case 1:
    case 2:
    case 3:
    case 5:
    case 6:
        break;
    case 4:
        next[0] = p->a;
        next[1] = (p->a & 0xff00) >> 8;
        next[2] = p->b;
        next[3] = (p->b & 0xff00) >> 8;
        next += 4;
        break;
    default:
        return 0;
    }
    return next;
}

u8 *MBi_SetRecvBufferFromChild(u8 *source, Child *dest, int child) {
    u8 *next;
    u8 type = source[0];
    dest->type = type;
    switch (type) {
    case 7:
        if (IsGetAllRequestData(child)) {
            return ov13_0224CF84.buffer + (child - 1) * 32;
        }
        {
            int piece = source[1];
            ((u8 *)dest)[2] = piece;
            if (piece > ov13_0224CF84.count) {
                return 0;
            }
            MI_CpuCopy8(source + 2, (u8 *)dest + 3, ov13_0224CF84.piece);
            next = MBi_ReceiveRequestDataPiece(dest, child);
            break;
        }
    case 8:
        dest->value = source[1];
        dest->value |= (source[2] << 8) & 0xff00;
        next = source + 3;
        break;
    case 9:
        dest->value = source[1];
        dest->value |= (source[2] << 8) & 0xff00;
        next = source + 3;
        MI_CpuCopy8(next, (u8 *)dest + 4, ov13_0224CF84.piece);
        next += ov13_0224CF84.piece;
        break;
    default:
        return 0;
    }
    return next;
}

u8 *MBi_ReceiveRequestDataPiece(Child *p, int child) {
    int slot, index;
    if (!ov13_0224CF84.buffer) {
        return 0;
    }
    index = ((u8 *)p)[2];
    if (index > ov13_0224CF84.count) {
        return 0;
    }
    slot = child - 1;
    MI_CpuCopy8((u8 *)p + 3, ov13_0224CF84.buffer + slot * 32 + index * ov13_0224CF84.piece, ov13_0224CF84.piece);
    {
        u32 *bits = (u32 *)(ov13_0224CF84.buffer + 0x1e0);
        bits[slot] |= 1u << index;
    }
    if (!IsGetAllRequestData(child)) {
        return 0;
    }
    return ov13_0224CF84.buffer + slot * 32;
}

int IsGetAllRequestData(int child) {
    u16 i = 0;
    if (ov13_0224CF84.count > 0) {
        u32 bits = *(u32 *)(ov13_0224CF84.buffer + 0x1e0 + (child - 1) * 4);
        do {
            if (!(bits & (1u << i))) {
                return 0;
            }
            i++;
        } while (i < ov13_0224CF84.count);
    }
    return 1;
}
