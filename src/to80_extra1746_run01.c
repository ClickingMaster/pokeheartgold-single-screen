#pragma force_active on
#include "global.h"
#pragma opt_loop_invariants off

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern char *mbc;
extern u32 _02247480;
extern void MBi_MakeParentSendBuffer(void *, void *);
extern int MBi_BlockHeaderEnd(int, u16, void *);
int MBi_CommParentSendMsg(u8, u16);
void MBi_calc_sendblock(int);

typedef struct Cache {
    u32 position, size;
    void *destination;
    int state;
} Cache;

typedef struct Owner {
    char pad[16];
    char name[4];
    u32 length;
    FSArchive *archive;
} Owner;

typedef struct Request {
    char pad[16];
    Cache *cache;
    Owner *owner;
} Request;

void MBi_ReloadCache(Request *);
extern int MBi_CommParentSendDLFileInfo(u16);
extern int MBi_CommParentSendBlock(void);
int MBi_CommParentSendData(void);

int MBi_CommParentSendData(void) {
    u16 i, masks[5];
    int result;
    char *root;
    MIi_CpuClear16(0, masks, 10);
    root = mbc;
    for (i = 1; i <= 15; i++) {
        switch (*(u32 *)((u32)root + 0x14e4 + i * 4)) {
        case 2:
            masks[0] |= 1 << i;
            break;
        case 5:
            masks[1] |= 1 << i;
            break;
        case 4:
            masks[2] |= 1 << i;
            break;
        case 8:
            masks[3] |= 1 << i;
            break;
        case 11:
            masks[4] |= 1 << i;
            break;
        }
    }
    if (masks[3]) {
        result = MBi_CommParentSendMsg(5, masks[3]);
    } else if (masks[0]) {
        result = MBi_CommParentSendMsg(1, masks[0]);
    } else if (masks[4]) {
        result = MBi_CommParentSendMsg(6, masks[4]);
    } else if (masks[2]) {
        result = MBi_CommParentSendMsg(2, masks[2]);
    } else if (masks[1]) {
        result = MBi_CommParentSendDLFileInfo(masks[1]);
    } else {
        result = MBi_CommParentSendBlock();
    }
    if (result == 21) {
        result = MBi_CommParentSendMsg(0, 0xffff);
    }
    return result;
}
