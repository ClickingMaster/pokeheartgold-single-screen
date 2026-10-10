#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Segment {
    u32 start;
    char *source, *destination;
    u32 size;
} Segment;

typedef struct Info {
    u32 a, b, pad;
    char segments[48], tail[136];
} Info;

extern u32 MBi_defaultLoadSegList[];
extern void MBi_SetSegmentInfo(void *, u32 *, void *, u32 *);
void MBi_ReadSegmentHeader(Segment *, u32, u32, int);
void MBi_MakeDownloadFileInfo(Info *, char *);
int MBi_IsAbleToRecv(u32, u32, u32);
int IsAbleToLoad(u32, u32, u32);

typedef struct SegmentInfo {
    u32 destination, load, size, extra;
} SegmentInfo;

typedef struct Download {
    u32 fields[3];
    SegmentInfo segments[3];
} Download;

typedef struct BlockTable {
    u32 offsets[3];
    u16 boundaries[3], count;
} BlockTable;

typedef struct Block {
    u32 address, size, position;
    u8 segment;
} Block;

extern char *mbc;
int MBi_MakeBlockInfoTable(BlockTable *, Download *);
int MBi_get_blockinfo(Block *, BlockTable *, u16, Download *);
#pragma force_active on

int IsAbleToLoad(u32 segment, u32 address, u32 size) {
    u32 type = MBi_defaultLoadSegList[segment];
    if (type == 0) {
        goto receive;
    }
    if (type == 1) {
        goto load;
    }
    if (type != 2) {
        goto invalid;
    }
receive:
    return MBi_IsAbleToRecv(segment, address, size);
load:
    if (address >= 0x2000000 && address < 0x23fe800) {
        u32 end = address + size;
        if (address < 0x2300000 && end > 0x2300000) {
            return 0;
        }
        if (end <= 0x2300000) {
            return 1;
        }
        if (end < 0x23fe800 && size <= 0x40000) {
            return 1;
        }
        return 0;
    }
    if (address >= 0x37f8000 && address < 0x380f000) {
        return address + size <= 0x380f000;
    }
    goto badAddress;
invalid:
    return 0;
badAddress:
    return 0;
}
