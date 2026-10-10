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

int MBi_IsAbleToRecv(u32 segment, u32 address, u32 size) {
    switch (MBi_defaultLoadSegList[segment]) {
    case 2:
        if (address < 0x27ffe00 || address + size > 0x27fff60) {
            goto invalid;
        }
        return 1;
    case 0:
        if (address < 0x2000000 || address + size > 0x22c0000) {
            goto invalid;
        }
        return 1;
    case 1:
        if (address >= 0x22c0000 && address + size <= 0x2300000) {
            return 1;
        }
        if (address < 0x2000000 || address + size > 0x2300000) {
            goto invalid;
        }
        return 1;
    default:
        return 0;
    }
invalid:
    return 0;
}
