#pragma force_active on
#include "wl_internal.h"
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

// End of a buffer list.
#define HEAP_BUF_NONE ((WlHeapBuf *)0xFFFFFFFF)

// IRQs masked while a buffer list changes (wireless).
#define HEAP_BUF_IRQ_MASK 0x1000000

// -ipa file would otherwise inline the small functions of this file into their callers here.
#pragma dont_inline on

// Allocates `size` bytes plus the buffer header from the driver heap and adds the buffer to `man`;
// returns the header (NULL on failure).
void *AllocateHeapBuf(WlHeapBufMan *man, u32 size) {
    WlWork *w = *(WlWork **)WL_WORK_ADDR;
    WlHeapBuf *buf;
    if (!size) {
        return NULL;
    }
    switch (w->heap.param.mode) {
    case 0:
        buf = __VENEER_OS_AllocFromHeap(w->heap.param.source.arena, w->heap.param.sink.heap, size + sizeof(WlHeapBuf));
        break;
    case 1:
        buf = w->heap.param.source.alloc(size + sizeof(WlHeapBuf));
        break;
    }
    if (!buf) {
        return NULL;
    }
    buf->magic = WL_HEAP_BUF_MAGIC;
    buf->owner = 0;
    AddHeapBuf(man, buf);
    return buf;
}

// Removes `buf` from `man` and gives it back to the driver heap; returns DeleteHeapBuf's error.
int ReleaseHeapBuf(WlHeapBufMan *man, WlHeapBuf *buf) {
    WlWork *w = wlWork;
    int result;
    if (buf->magic != WL_HEAP_BUF_MAGIC) {
        return 1;
    }
    result = DeleteHeapBuf(man, buf);
    if (!result) {
        switch (w->heap.param.mode) {
        case 0:
            __VENEER_OS_FreeToHeap(w->heap.param.source.arena, w->heap.param.sink.heap, buf);
            break;
        case 1:
            w->heap.param.sink.free(buf);
            break;
        }
    }
    return result;
}

int MoveHeapBuf(WlHeapBufMan *from, WlHeapBufMan *to, WlHeapBuf *buf) {
    int result;
    u32 irq;
    if (buf->magic != WL_HEAP_BUF_MAGIC) {
        return 1;
    }
    irq = __VENEER_OS_DisableIrqMask(HEAP_BUF_IRQ_MASK);
    result = DeleteHeapBuf(from, buf);
    if (!result) {
        result = AddHeapBuf(to, buf);
    }
    __VENEER_OS_EnableIrqMask(irq);
    return result;
}

// Appends a buffer that has no header yet (a request received from the WMSP).
int NewHeapBuf(WlHeapBufMan *man, WlHeapBuf *buf) {
    u32 irq = __VENEER_OS_DisableIrqMask(HEAP_BUF_IRQ_MASK);
    if (man->count == 0) {
        buf->prev = HEAP_BUF_NONE;
        man->first = buf;
    } else {
        WlHeapBuf *last = man->last;
        buf->prev = last;
        last->next = buf;
    }
    buf->next = HEAP_BUF_NONE;
    buf->owner = man->id;
    buf->magic = WL_HEAP_BUF_MAGIC;
    man->last = buf;
    man->count++;
    __VENEER_OS_EnableIrqMask(irq);
    return 0;
}

// Appends `buf` to `man`; 1: not a heap buffer, 2: held by another manager.
int AddHeapBuf(WlHeapBufMan *man, WlHeapBuf *buf) {
    u32 irq;
    if (buf->magic != WL_HEAP_BUF_MAGIC) {
        return 1;
    }
    if (buf->owner) {
        return 2;
    }
    irq = __VENEER_OS_DisableIrqMask(HEAP_BUF_IRQ_MASK);
    if (man->count == 0) {
        buf->prev = HEAP_BUF_NONE;
        man->first = buf;
    } else {
        WlHeapBuf *last = man->last;
        buf->prev = last;
        last->next = buf;
    }
    buf->next = HEAP_BUF_NONE;
    buf->owner = man->id;
    man->last = buf;
    man->count++;
    __VENEER_OS_EnableIrqMask(irq);
    return 0;
}

// Unlinks `buf` from `man`; 1: not a heap buffer, 2: not held by `man`.
int DeleteHeapBuf(WlHeapBufMan *man, WlHeapBuf *buf) {
    u32 irq;
    if (buf->magic != WL_HEAP_BUF_MAGIC) {
        return 1;
    }
    if (buf->owner != man->id) {
        return 2;
    }
    irq = __VENEER_OS_DisableIrqMask(HEAP_BUF_IRQ_MASK);
    man->count--;
    if (man->count == 0) {
        man->first = HEAP_BUF_NONE;
        man->last = HEAP_BUF_NONE;
    } else if (buf == man->first) {
        WlHeapBuf *next = buf->next;
        man->first = next;
        next->prev = HEAP_BUF_NONE;
    } else {
        WlHeapBuf *prev = buf->prev;
        if (buf == man->last) {
            man->last = prev;
            prev->next = HEAP_BUF_NONE;
        } else {
            buf->next->prev = prev;
            buf->prev->next = buf->next;
        }
    }
    buf->owner = 0;
    __VENEER_OS_EnableIrqMask(irq);
    return 0;
}

WlHeapBuf *GetHeapBufNextAdrs(WlHeapBuf *buf) {
    return buf->next;
}

void InitHeapBufMan(WlHeapBufMan *man, u32 id) {
    man->first = HEAP_BUF_NONE;
    man->last = HEAP_BUF_NONE;
    man->count = 0;
    man->id = id;
}

// Takes the heap parameters and sets up the 11 buffer managers (ids 2..12).
void InitializeHeapBuf(WlHeapParam *param) {
    WlWork *w = wlWork;
    WlBss *bss = &w->bss;
    w->heap.param.mode = param->mode;
    w->heap.param.source.arena = param->source.arena;
    w->heap.param.sink.heap = param->sink.heap;
    InitHeapBufMan(&w->heap.bufMan[0], 2);
    InitHeapBufMan(&w->heap.bufMan[1], 3);
    InitHeapBufMan(&w->heap.bufMan[2], 4);
    InitHeapBufMan(&w->heap.bufMan[3], 5);
    InitHeapBufMan(&w->heap.bufMan[4], 6);
    InitHeapBufMan(&w->heap.bufMan[5], 7);
    InitHeapBufMan(&w->heap.bufMan[6], 8);
    InitHeapBufMan(&w->heap.bufMan[7], 9);
    InitHeapBufMan(&w->heap.bufMan[8], 10);
    InitHeapBufMan(&w->heap.bufMan[9], 11);
    InitHeapBufMan(&w->heap.bufMan[10], 12);
    bss->gameInfo = (u8 *)AllocateHeapBuf(&w->heap.bufMan[0], 129) + sizeof(WlHeapBuf);
    bss->gameInfoLength = 0;
    bss->unk_a4 = 0;
}

// Frees the buffers of managers 1..8.
void ReleaseAllWlHeapBuf(void) {
    WlWork *w = wlWork;
    ReleaseAllHeapBuf(&w->heap.bufMan[1]);
    ReleaseAllHeapBuf(&w->heap.bufMan[2]);
    ReleaseAllHeapBuf(&w->heap.bufMan[3]);
    ReleaseAllHeapBuf(&w->heap.bufMan[4]);
    ReleaseAllHeapBuf(&w->heap.bufMan[5]);
    ReleaseAllHeapBuf(&w->heap.bufMan[6]);
    ReleaseAllHeapBuf(&w->heap.bufMan[7]);
    ReleaseAllHeapBuf(&w->heap.bufMan[8]);
}

void ReleaseAllHeapBuf(WlHeapBufMan *man) {
    WlHeapBuf *buf = man->first;
    if (man->count) {
        while (buf != HEAP_BUF_NONE) {
            WlHeapBuf *next = buf->next;
            ReleaseHeapBuf(man, buf);
            buf = next;
        }
    }
}
