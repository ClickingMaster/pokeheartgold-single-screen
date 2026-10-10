#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Conn {
    void *handle;
} Conn;

typedef struct W {
    u8 p[4];
    Conn *conn;
    u8 p8[5];
    u8 count;
    u8 pe[7];
    u8 mode, total;
    u8 rest[0x464];
} W;

#define BYTE(w, o) (*(u8 *)((u8 *)(w) + (o)))
#define WORD(w, o) (*(u32 *)((u8 *)(w) + (o)))
#define HALF(w, o) (*(u16 *)((u8 *)(w) + (o)))
extern W *ov00_021F955C(void);
extern void ov00_021F7294(int, int);
extern void ov00_021F956C(int);
extern void ov00_021F64C0(int);
extern void ov00_021F2EE4(int, int);
extern void ov00_0221019C(void *);
extern void ov00_0220D238(void *);
extern void ov00_021F6FF8(void);
extern void ov00_021F6EC4(int);
extern int ov00_021F7D50(int);
extern int ov00_021EE568(void);
void ov00_021F2FDC(int, int, u8 *);
int ov00_021F3194(int, int);

void ov00_021F2FDC(int peer, int event, u8 *data) {
    int i;
    W *left;
    switch (event) {
    case 2:
        if (WORD(ov00_021F955C(), 0x1a0) == 1) {
            if (data[0] == 1) {
                WORD(ov00_021F955C(), 0x20c) = 0;
            }
            i = data[1];
            BYTE((u8 *)ov00_021F955C() + i, 0x2d0) = data[2];
            left = ov00_021F955C();
            WORD((u8 *)left + i * 4, 0xf4) = WORD(ov00_021F955C(), 0x200);
            if (ov00_021F955C()->mode == 0 || ov00_021F955C()->mode == 1) {
                left = ov00_021F955C();
                left->total = ov00_021F955C()->count;
            }
            BYTE(ov00_021F955C(), 0x1ad) = 0;
            ov00_021F956C(9);
        }
        ov00_021F7294(peer, 3);
        break;
    case 3:
        if (WORD(ov00_021F955C(), 0x1a0) == 16) {
            int value;
            W *w = ov00_021F955C();
            WORD(w, 0x1d4) |= 1 << peer;
            value = data[0] | data[1] << 8;
            if (value > HALF(ov00_021F955C(), 0x1b0)) {
                HALF(ov00_021F955C(), 0x1b0) = value;
            }
            value = ov00_021F7D50(0);
            if (value != WORD(ov00_021F955C(), 0x1d4)) {
                return;
            }
            for (i = 1; i <= ov00_021F955C()->count; i++) {
                ov00_021F7294(BYTE((u8 *)ov00_021F955C() + i, 0x2d0), 4);
            }
            ov00_021F956C(17);
        } else {
            ov00_021F7294(peer, 4);
        }
        break;
    case 4:
        if (WORD(ov00_021F955C(), 0x1a0) == 9) {
            ov00_021F64C0(4);
        }
        break;
    }
}

int ov00_021F3194(int error, int code) {
    if (ov00_021EE568() != 5) {
        return 0;
    }
    if (ov00_021F955C()->mode == 2) {
        return 1;
    }
    if (error != 0) {
        ov00_021F2EE4(error, code - 80000);
        return 1;
    }
    BYTE(ov00_021F955C(), 0x2d0) = 0;
    if (BYTE(ov00_021F955C(), 0x1a9) == 1 || BYTE(ov00_021F955C(), 0x1a8) == 1 || BYTE(ov00_021F955C(), 0x1a8) == 2) {
        return 1;
    }
    if (WORD(ov00_021F955C(), 0x19c)) {
        ov00_0221019C((void *)WORD(ov00_021F955C(), 0x19c));
        WORD(ov00_021F955C(), 0x19c) = 0;
    }
    if (ov00_021F955C()->count) {
        if (BYTE(ov00_021F955C(), 0x1a8) == 0) {
            BYTE(ov00_021F955C(), 0x1a8) = 3;
            ov00_0220D238(ov00_021F955C()->conn->handle);
        }
    } else if (ov00_021F955C()->mode == 3) {
        ov00_021F2EE4(6, -80430);
    } else if (WORD(ov00_021F955C(), 0x208)) {
        ov00_021F6FF8();
    } else if (WORD(ov00_021F955C(), 0x1a0) == 1) {
        W *w;
        u64 tick;
        ov00_021F956C(18);
        w = ov00_021F955C();
        tick = OS_GetTick();
        WORD(w, 0x1f0) = (u32)tick;
        WORD(w, 0x1f4) = (u32)(tick >> 32);
    } else {
        ov00_021F6EC4(1);
    }
    return 1;
}
