#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 p[0x28];
    volatile u32 head;
    u32 pad;
    volatile u32 tail;
    u8 p2[0x58 - 0x34];
    u8 *buffer;
    volatile u32 length;
} Work;

extern Work _021D4620;
extern u8 _02110690[];

void put_in_buffer(void *src0, void *src1, u8 *data, u32 len, void *extra, u32 extraLen);

void put_in_buffer(void *src0, void *src1, u8 *data, u32 len, void *extra, u32 extraLen) {
    u32 total, next;
    u16 size;
    if (_021D4620.buffer == 0 || _021D4620.length == 0) {
        return;
    }
    total = len + extraLen;
    if (total < 8 || total > 1508) {
        return;
    }
    if (data[0] != _02110690[0] || data[1] != _02110690[1] || data[2] != _02110690[2] || data[6] != 8) {
        return;
    }
    if (data[7] != 0 && data[7] != 6) {
        return;
    }
    size = (total + 9) & ~1;
    next = _021D4620.head + size;
    if (_021D4620.head < _021D4620.tail && _021D4620.tail <= next) {
        return;
    }
    if (next == _021D4620.length) {
        next = 0;
        if (_021D4620.tail == 0) {
            return;
        }
    } else if (next > _021D4620.length) {
        next = size;
        if (_021D4620.tail <= size) {
            return;
        }
    }
    if (_021D4620.head + size > _021D4620.length) {
        if (_021D4620.length - _021D4620.head >= 2) {
            *(u16 *)(_021D4620.buffer + _021D4620.head) = 0;
        }
        _021D4620.head = 0;
    }
    *(u16 *)(_021D4620.buffer + _021D4620.head) = size;
    MI_CpuCopy8(src1, _021D4620.buffer + _021D4620.head + 2, 6);
    MI_CpuCopy8(src0, _021D4620.buffer + _021D4620.head + 8, 6);
    MI_CpuCopy8(data + 6, _021D4620.buffer + _021D4620.head + 14, len - 6);
    if (extra && extraLen) {
        MI_CpuCopy8(extra, _021D4620.buffer + _021D4620.head + 8 + len, extraLen);
    }
    _021D4620.head = next;
}
