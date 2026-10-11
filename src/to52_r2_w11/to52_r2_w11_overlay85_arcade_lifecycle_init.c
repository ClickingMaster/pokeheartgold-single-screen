#include <string.h>

#include "private.h"

BOOL ov85_021E88AC(void *manager, int *state) {
    NARC *narc;
    ArcArgs85 *args = OverlayManager_GetArgs(manager);
    ArcScene *scene;
    switch (*state) {
    case 0:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        *(vu32 *)0x04000000 &= 0xffffe0ff;
        *(vu32 *)0x04001000 &= 0xffffe0ff;
        Heap_Create(HEAP_ID_3, (enum HeapID)0x66, 0x80000);
        narc = NARC_New((NarcId)0xd9, (enum HeapID)0x66);
        GF_ASSERT(narc != NULL);
        scene = OverlayManager_CreateAndGetData(manager, sizeof(ArcScene), (enum HeapID)0x66);
        memset(scene, 0, sizeof(ArcScene));
        args->scene = scene;
        scene->unk_00C = args;
        scene->unk_010 = args->unk_30;
        scene->unk_014 = BgConfig_Alloc((enum HeapID)0x66);
        scene->unk_034 = MessageFormat_New((enum HeapID)0x66);
        scene->unk_038 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, (NarcId)0x1b, 0xcf, (enum HeapID)0x66);
        SetKeyRepeatTimers(4, 8);
        ov85_021E8C3C();
        ov85_021E8C5C(scene->unk_014);
        BeginNormalPaletteFade(0, 0x11, 0x11, 0, 16, 1, 0x66);
        ov85_021E8E6C(scene, narc);
        Main_SetVBlankIntrCB(ov85_021E8C14, scene);
        ov85_021E8D64(scene, narc);
        ov85_021E8F58();
        ov85_021E8F88(scene, narc);
        ov85_021E9084(scene);
        ov85_021E9160(scene);
        Sound_SetSceneAndPlayBGM(52, 0, 0);
        sub_02096CE0(args->unk_30);
        sub_0203897C();
        sub_02038C1C(3);
        if (!Comm_GetLocalNetId()) {
            sub_0205A904(13);
        }
        WirelessStatusIcon_Create();
        scene->unk_030 = SysTask_CreateOnVBlankQueue(ov85_021E8BB0, scene, 5);
        NARC_Delete(narc);
        (*state)++;
        break;
    case 1:
        *state = 0;
        return TRUE;
    }
    return FALSE;
}
