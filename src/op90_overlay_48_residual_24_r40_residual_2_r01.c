#pragma force_active on
#include "global.h"
#include "assert.h"
#include "bg_window.h"
#include "vram_transfer_manager.h"

typedef struct Ov48Screen {
    u16 width;
    u16 height;
    u8 unk4[8];
    u16 data[1];
} Ov48Screen;

typedef struct Ov48Pltt {
    u8 unk0[0xC];
    u8 *data;
} Ov48Pltt;

typedef struct Ov48Anim {
    u16 counter;
    u16 period;
    u8 enabled[3];
    u8 unk7[9];
    Ov48Screen *screens[2];
    u16 numScreens;
    u16 curScreen;
    u8 unk1C[4];
    Ov48Pltt *pltt;
    u8 numPltts;
    u8 curPltt;
    u8 plttSlot;
} Ov48Anim;

void ov48_0225A95C(Ov48Anim *anim, BgConfig **bgConfig);

// @fn ov48_0225A95C
void ov48_0225A95C(Ov48Anim *anim, BgConfig **bgConfig) {
    u16 counter = anim->counter;
    u16 period = anim->period;
    int screen = counter * anim->numScreens / period;
    int pltt = counter * anim->numPltts / period;
    int i;
    Ov48Screen *scr;

    anim->counter = (counter + 1) % period;
    if (anim->numScreens != 0 && screen != anim->curScreen) {
        anim->curScreen = screen;
        for (i = 0; i < 3; i++) {
            if (anim->enabled[i] == 1) {
                scr = anim->screens[anim->curScreen];
                CopyToBgTilemapRect(*bgConfig, 6, 0, i * 6, 32, 6, scr->data, 0, i * 6, scr->width >> 3, scr->height >> 3);
                ScheduleBgTilemapBufferTransfer(*bgConfig, 6);
            }
        }
    }
    if (anim->numPltts != 0 && pltt != anim->curPltt) {
        anim->curPltt = pltt;
        GF_ASSERT(GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_SUB, anim->plttSlot * 32, anim->pltt->data + anim->curPltt * 32, 32));
    }
}
