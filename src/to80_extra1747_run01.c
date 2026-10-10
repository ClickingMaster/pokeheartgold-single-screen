#pragma force_active on
#include "global.h"
#pragma force_active on

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

int MBi_get_blockinfo(Block *out, BlockTable *table, u16 block, Download *info) {
    s8 i;
    u32 offset;
    SegmentInfo *s;
    if (block >= table->count) {
        return 0;
    }
    i = 2;
    for (; i >= 0; i--) {
        if (block >= table->boundaries[i]) {
            break;
        }
    }
    if (i < 0) {
        return 0;
    }
    u32 first = table->boundaries[i];
    u32 unit = *(u32 *)(mbc + 0x1318);
    first = block - first;
    offset = first * unit;
    s = &info->segments[i];
    out->size = s->size - offset;
    if (out->size > *(u32 *)(mbc + 0x1318)) {
        out->size = *(u32 *)(mbc + 0x1318);
    }
    out->position = offset + table->offsets[i];
    out->address = offset + s->destination;
    out->segment = i;
    return 1;
}
