#pragma force_active on
#include "global.h"
#include "player_data.h"

u16 ov74_0223D080[8];
u16 ov74_0223D090[10];

typedef struct {
    u8 prefix[16];
    PlayerProfile *profile;
} Work;

u16 ov74_02228C84(u16);
void ov74_IntToWstring5(u16 *, u32);
void ov74_02228D20(Work *w);

// @fn ov74_02228D20
void ov74_02228D20(Work *w) {
    int i;
    const u16 *src = PlayerProfile_GetNamePtr(w->profile);
    u16 *dst = ov74_0223D080;
    for (i = 0; i < 8; i++) {
        *dst = ov74_02228C84(*src);
        src++;
        dst++;
    }
    ov74_IntToWstring5(ov74_0223D090, PlayerProfile_GetTrainerID_VisibleHalf(w->profile));
    ov74_0223D090[5] = 0;
}
