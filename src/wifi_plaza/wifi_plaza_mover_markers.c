#include "global.h"

#include "wifi_plaza/wifi_plaza_internal.h"
#include "wifi_plaza/wifi_plaza_models.h"

// Marker progress of the nine movers of the room event.
typedef struct WiFiPlazaMoverMarkerWork {
    int room;
    struct WiFiPlazaWork *lobby;
    struct WiFiPlazaModels *models;
    u8 gap[0x9E];

    struct {
        s16 x, z;
    } markers[3]; // cell positions (pixels) of the three marker map tags
} WiFiPlazaMoverMarkerWork;

void WiFiPlazaEvent_UpdateMoverMarkers(void *parent, int index);

// For each of the three sides of mover `index` (lobby slot index * 3 + side), reports to the lobby every marker
// from the count the lobby already holds on that the front of the side has passed (movers travel towards -x:
// front x below the marker x plus one cell).
void WiFiPlazaEvent_UpdateMoverMarkers(void *parent, int index) {
    WiFiPlazaMoverMarkerWork *work = parent;

    struct {
        int index, base;
    } input;

    input.index = index;
    int i = 0, j;
    int twice = (s32)((u32)index * 2);
    input.base = (s32)((u32)input.index + (u32)twice);
    VecFx32 front;
    for (i = 0; i < 3; i++) {
        j = WiFiPlazaWork_GetGroupMarkerCount(work->lobby, (s32)((u32)i + (u32)input.base));
        WiFiPlazaModels_GetMoverFrontPos(work->models, input.index, i, &front);
        if (j < 3) {
            u8 *p = (u8 *)work + j * 4;
            do {
                if ((*(s16 *)(p + 0xaa) + 16) * 4096 > front.x) {
                    WiFiPlazaWork_SetGroupMarkerPassed(work->lobby, (s32)((u32)i + (u32)input.base), j);
                }
                j++;
                p += 4;
            } while (j < 3);
        }
    }
}
