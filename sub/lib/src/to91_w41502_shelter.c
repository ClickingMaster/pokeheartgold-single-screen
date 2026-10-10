#pragma force_active on
#include <OS_terminate_proc.h>
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 destination, loadSize, bssSize;
} Auto09;

typedef struct {
    u8 status, reserved[3];
    u32 *image;
    void *heap;
    u32 imageSize;
    u8 rest[0x1374 - 16];
} Wvr09;

extern const Auto09 SDK_AUTOLOAD_LIST[];
extern const Auto09 SDK_AUTOLOAD_LIST_END[];
extern const u32 SDK_AUTOLOAD_START[];
extern u8 SDK_AUTOLOAD_MAIN_BSS_END[];
extern u8 SDK_SUBPRIV_ARENA_LO[];
extern Wvr09 WVR_BSS_START;

static inline void ShelterImage09(u32 load, u32 bss, const u8 *source) {
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

void WVR_ShelterExtWram(void) {
    const u8 *source = (const u8 *)SDK_AUTOLOAD_START;
    const Auto09 *entry = SDK_AUTOLOAD_LIST;
    while (entry != SDK_AUTOLOAD_LIST_END) {
        u32 destination = entry->destination, load = entry->loadSize, bss = entry->bssSize;
        entry++;
        if (destination == 0x06000000) {
            ShelterImage09(load, bss, source);
            break;
        }
        source += load;
    }
}
