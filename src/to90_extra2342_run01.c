#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u32 start, length, pad[2];
} File;

typedef struct {
    u8 pad[8];
    u32 count;
    File entries[1];
} Fat;

typedef struct {
    u8 pad[0x34];
    FSFile file;
    u8 padding[0x84 - 0x34 - sizeof(FSFile)];
    Fat *fat;
    u8 pad88[8];
    int chunk;
} Arc;

extern Arc *_021E0910;
int NNS_SndArcReadFile(u32, void *, int, u32);

int NNS_SndArcReadFile(u32 id, void *dst, int len, u32 offset) {
    Arc *a = _021E0910;
    File *f;
    int total, chunk, n;
    if (id >= a->fat->count) {
        return -1;
    }
    chunk = a->chunk;
    f = &a->fat->entries[id];
    if (chunk == 0) {
        chunk = len;
    }
    total = 0;
    while (total < len) {
        int size = len - total;
        if (size > chunk) {
            size = chunk;
        }
        if ((u32)size > f->length - offset) {
            size = f->length - offset;
        }
        if (!size) {
            break;
        }
        if (!FS_SeekFile(&a->file, f->start + offset, FS_SEEK_SET)) {
            return -1;
        }
        n = FS_ReadFile(&a->file, dst, size);
        if (n < 0) {
            return n;
        }
        total += n;
        offset += n;
        dst = (u8 *)dst + n;
    }
    return total;
}
