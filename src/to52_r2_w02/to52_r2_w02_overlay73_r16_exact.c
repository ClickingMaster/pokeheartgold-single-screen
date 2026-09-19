#include "private.h"

BOOL ov73_021E7E4C(void *manager, int *state) {
    void *work;
    W02Modes modes;
    void *mem;

    switch (*state) {
    case 0:
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        *(vu32 *)0x04000000 &= 0xFFFFE0FF;
        *(vu32 *)0x04001000 &= 0xFFFFE0FF;
        Heap_Create(3, 0x96, 5 << 16);
        work = OverlayManager_CreateAndGetData(manager, 0x12CC, 0x96);
        memset(work, 0, 0x12CC);
        PTR(work, 4) = BgConfig_Alloc(0x96);
        _021EA940[0] = work;
        modes = ov73_021EA68C;
        SetBothScreensModesAndDisable(&modes);
        PTR(work, 0xBD0) = MessageFormat_New_Custom(0xB, 0x20, 0x96);
        PTR(work, 0xBD4) = NewMsgDataFromNarc(0, 0x1B, 0x31F, 0x96);
        PTR(work, 0xBD8) = NewMsgDataFromNarc(0, 0x1B, 0x30A, 0x96);
        PTR(work, 0xBDC) = NewMsgDataFromNarc(0, 0x1B, 0x320, 0x96);
        SetKeyRepeatTimers(4, 8);
        ov73_021E8148(work, manager);
        ov73_021E8454(work);
        Sound_SetSceneAndPlayBGM(0x34, 0, 0);
        mem = Heap_Alloc(0x96, 0x20020);
        PTR(work, 0x24) = mem;
        PTR(work, 0x28) = NNS_FndCreateExpHeapEx((void *)(((u32)mem + 0x1F) & ~0x1F), 0x20000, 0);
        Sound_SetSceneAndPlayBGM(0xB, 0x47D, 1);
        *(vu16 *)0x04000304 &= 0xFFFF7FFF;
        *state = 1;
        break;
    case 1:
        LoadDwcOverlay();
        LoadOVY38();
        sub_02034D8C();
        *state = 0;
        return TRUE;
    default:
        break;
    }
    return FALSE;
}
