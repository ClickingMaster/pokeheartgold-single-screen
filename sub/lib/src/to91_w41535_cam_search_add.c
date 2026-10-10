#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a)      (*(volatile u16 *)(a))
#define WL        (*(u8 **)0x0380fff4)
#define R16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define R32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o)   (*(u8 **)((u8 *)(p) + (o)))
extern int MatchMacAdrs(const u16 *, const u16 *);
extern void CAM_InitElement(u32, const u8 *);

typedef struct Station40788 {
    u16 state, association;
    u16 mac[3];
    u16 frames, reserved[5], aid, age, life;
} Station40788;

typedef struct {
    Station40788 *entries;
    u16 capacity, count;
} Cam40788;

typedef struct {
    u8 prefix[0x31c];
    Cam40788 cam;
    u8 middle[0x208];
    u16 active;
} Work40788;

#define WORK40788 (*(Work40788 **)0x0380fff4)
u32 CAM_SearchAdd(u16 *address);

u32 CAM_SearchAdd(u16 *input) {
    u16 *address;
    Work40788 **root;
    Cam40788 *cam;
    Station40788 *record;
    u32 index, seen, freeIndex;
    Work40788 *work;
    address = input;
    root = (Work40788 **)0x0380fff4;
    work = *root;
    cam = &work->cam;
    if (address[0] & 1) {
        return 0;
    }
    index = 1;
    if (work->active > 1) {
        record = cam->entries + 1;
        freeIndex = 0;
        seen = 0;
        for (; index < cam->count; index++, record++) {
            if (record->state) {
                if (MatchMacAdrs(record->mac, address)) {
                    return index;
                }
                work = *root;
                seen++;
                if (seen >= work->active) {
                    break;
                }
            } else if (freeIndex == 0) {
                freeIndex = index;
            }
        }
        if (freeIndex) {
            index = freeIndex;
        }
    }
    if (index >= work->cam.count) {
        record = cam->entries;
        u32 best, length, i;
        length = cam->count;
        best = 0x10000;
        i = 1;
        index = 0;
        for (; i < length; i++) {
            Station40788 *entry = record + i;
            if (entry->state < 0x30 && entry->aid == 0 && best > entry->age) {
                best = entry->age;
                index = i;
            }
        }
        if (index == 0) {
            return 255;
        }
    }
    CAM_InitElement(index, (const u8 *)address);
    return index;
}
