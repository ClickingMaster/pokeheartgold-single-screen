#pragma force_active on
#include "global.h"
typedef struct {
    void *host;
    u8 pad4[0x18];
    fx32 x;
    u8 pad20[6];
    u8 x26;
    u8 pad27[0x38 - 0x27];
} Ent;
typedef struct {
    int height;
    u8 pad4[0x18];
} Track;
typedef struct {
    u8 pad[0x90];
    Ent ents[3];
    u8 pad2[0xfb4 - 0x90 - 3 * 0x38];
    Track tracks[3];
} Work;
int ov96_021E6104();
void ov96_021EB0A4(void *host, s32 x, s32 y, s32 *outX, s32 *outY);
BOOL ov96_021E872C(int x, int y, int a2, int a3, int r, int *out);
u8 ov96_021F5F68(Work *work, u16 a1, u16 a2);
// @fn ov96_021F5F68
u8 ov96_021F5F68(Work *work, u16 a1, u16 a2) {
    fx32 min = ov96_021E6104(work) << FX32_SHIFT;
    u8 i;
    u8 count = 0;
    u8 best;
    s32 dist[3];
    u8 valid[3];
    int out;
    s32 x;
    s32 y;
    for (i = 0; i < 3; i++) {
        Ent *ent = &work->ents[i];
        if (ent->x26 != 0) {
            valid[i] = FALSE;
            dist[i] = 0;
        } else {
            ov96_021EB0A4(ent->host, ent->x / FX32_ONE, (0x60000 - (work->tracks[i].height << 6)) / FX32_ONE, &x, &y);
            if (ov96_021E872C(x, y, a1, a2, ov96_021E6104(), &out)) {
                dist[i] = out;
                valid[i] = TRUE;
                count++;
            } else {
                dist[i] = 0;
                valid[i] = FALSE;
            }
        }
    }
    if (count == 0) {
        return 3;
    }
    best = 3;
    for (i = 0; i < 3; i++) {
        if (valid[i] && dist[i] < min) {
            best = i;
            min = dist[i];
        }
    }
    if (best == 3) {
        GF_ASSERT(FALSE);
        return 3;
    }
    return best;
}
