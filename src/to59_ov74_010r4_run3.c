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

int ov74_022338D4(OverlayManager *man);

int ov74_022338D4(OverlayManager *man) {
    u8 *work;
    work = OverlayManager_GetData(man);
    String_Delete(*(String **)(work + 0x12608));
    String_Delete(*(String **)(work + 0x1260C));
    Heap_Free(*(void **)(work + 0x20));
    RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_TitleScreen);
    OverlayManager_FreeData(man);
    Heap_Destroy((enum HeapID)0x4c);
    ov74_02236034(0);
    return 1;
}
