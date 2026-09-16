#include "constants/sndseq.h"

#include "msgdata/msg/msg_0196.h"

#include "filesystem_files_def.h"
#include "slop_r1_overlay32_module02_internal.h"
#include "sys_task_api.h"

void ov32_0225D60C(void) {
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
}
