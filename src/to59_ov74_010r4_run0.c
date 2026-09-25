#include "global.h"
#include "main.h"
#include "title_screen.h"
#include "pm_string.h"
#include "text.h"
#include "render_text.h"
#include "system.h"
#include "constants/sndseq.h"
#include "to58_overlay74_010_private.h"
#include "yes_no_prompt.h"
#include "sprite.h"
#include "gf_gfx_planes.h"
extern void ov74_02236034(int);
extern void GF_RunVramTransferTasks(void);
extern void OamManager_ApplyAndResetBuffers(void);
extern void ov74_02232B18(u8 *);
extern void ov74_02231A1C(u8 *, u8 *, int);
extern u32 ov74_0223C9B8[];
extern void ov74_02232F5C(u8 *);
extern void PlaySE(u32);

void ov74_02232DC4(u8 *work);

void ov74_02232DC4(u8 *work) {
    int i = 0;
    u8 *p = work;
    for (; i < 6; i++, p += 4) Sprite_Delete(*(Sprite **)(p + 0x414));
    GfGfx_EngineATogglePlanes(2, 1);
    ov74_02232DA4(work + 0x498);
    Sprite_SetDrawFlag(*(Sprite **)(work + 0x398), 1);
    Sprite_SetDrawFlag(*(Sprite **)(work + 0x3B8), 1);
    Sprite_SetDrawFlag(*(Sprite **)(work + 0x3A8), 1);
    u8 *q = work;
    int j = 0;
    for (; j < 6; j++, q += 12) {
        *(s32 *)(q + 0x3CC) = -1;
        *(s32 *)(q + 0x3D0) = 14;
    }
    *(u32 *)(work + 0x410) = 0;
}
