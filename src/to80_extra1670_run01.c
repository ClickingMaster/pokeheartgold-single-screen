#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 pad[0x14e4];
    int state[16];
    u8 active;
    s8 index[16];
} Mbc;

extern Mbc *mbc;
extern u8 _02245980;

typedef struct {
    u8 kind, pad;
    u16 file, block;
} Header;

void *MBi_MakeParentSendBuffer(Header *, void *);
int MBi_BlockHeaderEnd(int, u16, void *);
int MBi_CommParentSendDLFileInfo(void);

int MBi_CommParentSendDLFileInfo(void) {
    Header header;
    u8 counts[16];
    int selected = -1;
    u16 mask = 0;
    u16 child;
    u8 attempt, index;
    void *dest;
    MI_CpuFill8(counts, 0, 16);
    for (child = 1; child <= 15; child++) {
        if (mbc->state[child] == 5) {
            counts[mbc->index[child]]++;
        }
    }
    index = _02245980;
    for (attempt = 0; attempt < 16; attempt++) {
        index = (int)(index + 1) % 16;
        if (((u8 *)mbc)[index * 0x5d4 + 0x1d52] && counts[index]) {
            selected = (s8)index;
            break;
        }
    }
    if (selected == -1) {
        return 21;
    }
    _02245980 = selected;
    for (child = 1; child <= 15; child++) {
        if (mbc->state[child] == 5 && selected == mbc->index[child]) {
            mask |= 1 << child;
        }
    }
    header.kind = 3;
    header.file = selected;
    dest = MBi_MakeParentSendBuffer(&header, mbc);
    if (dest) {
        MI_CpuCopy8((u8 *)mbc + 0x1788 + selected * 0x5d4, dest, 0xe4);
    }
    return MBi_BlockHeaderEnd(0xea, mask, mbc);
}
