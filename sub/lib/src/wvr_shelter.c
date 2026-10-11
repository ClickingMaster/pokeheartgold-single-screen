#pragma force_active on
#include <nitro/types.h>
// The SDK headers below expect <nitro/types.h> to be included first.
#include <OS_terminate_proc.h>
// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct AutoloadEntry {
    u32 destination;
    u32 loadSize;
    u32 bssSize;
} AutoloadEntry;

typedef struct WvrWork {
    u8 status;
    u8 reserved[3];
    u32 *image;
    void *heap;
    u32 imageSize;
    u8 unk_10[0x1374 - 0x10];
} WvrWork;

extern const AutoloadEntry SDK_AUTOLOAD_LIST[];
extern const AutoloadEntry SDK_AUTOLOAD_LIST_END[];
extern const u32 SDK_AUTOLOAD_START[];
extern u8 SDK_AUTOLOAD_MAIN_BSS_END[];
extern u8 SDK_SUBPRIV_ARENA_LO[];
extern WvrWork WVR_BSS_START;

// Copies the autoload image (data + cleared bss) right behind the main-memory autoload bss, which
// must be exactly where the ARM7 private arena starts.
static inline void ShelterImage(u32 load, u32 bss, const u8 *source) {
    u32 *dst;
    u32 i;
    u32 total = load + bss;
    WVR_BSS_START.image = (u32 *)SDK_AUTOLOAD_MAIN_BSS_END;
    WVR_BSS_START.imageSize = total;
    if ((u32)SDK_SUBPRIV_ARENA_LO != (u32)SDK_AUTOLOAD_MAIN_BSS_END + total) {
        OS_Terminate();
    }
    dst = WVR_BSS_START.image;
    for (i = 0; i < load / 4; i++) {
        *dst++ = *(const u32 *)source, source += 4;
    }
    for (i = 0; i < bss / 4; i++) {
        *dst++ = 0;
    }
}

void WVR_ShelterExtWram(void);

// Saves the autoload block that targets 0x06000000 (the wireless driver's extended WRAM).
void WVR_ShelterExtWram(void) {
    const u8 *source = (const u8 *)SDK_AUTOLOAD_START;
    const AutoloadEntry *entry = SDK_AUTOLOAD_LIST;
    while (entry != SDK_AUTOLOAD_LIST_END) {
        u32 destination = entry->destination;
        u32 load = entry->loadSize;
        u32 bss = entry->bssSize;
        entry++;
        if (destination == 0x06000000) {
            ShelterImage(load, bss, source);
            break;
        }
        source += load;
    }
}
