#pragma force_active on
#include "global.h"
#pragma force_active on

// Marker progress of the nine movers of the room event (WiFiPlazaEventMovers in include/wifi_plaza_event.h,
// which this file does not include: the layout below keeps the original's accesses).
typedef struct WiFiPlazaMoverMarkerWork {
    int room;
    void *lobby;  // 0x04
    void *models; // 0x08
    u8 gap[0x9E];

    struct {
        s16 x, z;
    } markers[3]; // 0xAA cell positions (pixels) of the three marker map tags
} WiFiPlazaMoverMarkerWork;

typedef struct WiFiPlazaMoverMarkerPos {
    int x, y, z;
} WiFiPlazaMoverMarkerPos;

extern int WiFiPlazaWork_GetGroupMarkerCount(void *lobby, int slot);
extern void WiFiPlazaModels_GetMoverFrontPos(void *models, int index, int side, WiFiPlazaMoverMarkerPos *position);
extern void WiFiPlazaWork_SetGroupMarkerPassed(void *lobby, int slot, int marker);

void WiFiPlazaEvent_UpdateMoverMarkers(void *parent, int index);

// For each of the three sides of mover `index` (lobby slot index * 3 + side), reports to the lobby every marker
// from the count the lobby already holds on that the front of the side has passed (movers travel towards -x:
// front x below the marker x plus one cell).
void WiFiPlazaEvent_UpdateMoverMarkers(void *parent, int index) {
    WiFiPlazaMoverMarkerWork *w = parent;

    struct {
        int index, base;
    } input;

    input.index = index;
    int i = 0, j;
    int twice = (s32)((u32)index * 2);
    input.base = (s32)((u32)input.index + (u32)twice);
    WiFiPlazaMoverMarkerPos v;
    for (i = 0; i < 3; i++) {
        j = WiFiPlazaWork_GetGroupMarkerCount(w->lobby, (s32)((u32)i + (u32)input.base));
        WiFiPlazaModels_GetMoverFrontPos(w->models, input.index, i, &v);
        if (j < 3) {
            u8 *p = (u8 *)w + j * 4;
            do {
                if ((*(s16 *)(p + 0xaa) + 16) * 4096 > v.x) {
                    WiFiPlazaWork_SetGroupMarkerPassed(w->lobby, (s32)((u32)i + (u32)input.base), j);
                }
                j++;
                p += 4;
            } while (j < 3);
        }
    }
}
