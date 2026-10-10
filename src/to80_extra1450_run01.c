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

void MBi_ReadSegmentHeader(Segment *s, u32 first, u32 last, int clear) {
    u32 start = s->start, end;
    if (first < 0x4000) {
        first = 0x4000;
    }
    if (last > 0x8000) {
        last = 0x8000;
    }
    end = start + s->size;
    if (first < start) {
        first = start;
    }
    if (last > end) {
        last = end;
    }
    if (first >= last) {
        return;
    }
    if (clear) {
        MI_CpuFill8(s->destination + first, 0, last - first);
    } else {
        MI_CpuCopy8(s->source + first, s->destination + first, last - first);
    }
}
