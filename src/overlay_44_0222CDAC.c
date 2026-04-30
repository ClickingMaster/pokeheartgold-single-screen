#include "overlay_44_02232E9C.h"

#include <nitro/gx.h>

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0778.h"
#include "msgdata/msg/msg_0800.h"

#include "save_frontier.h"
#include "save_wifi_history.h"
#include "obj_pltt_transfer.h"
#include "pokedex.h"
#include "pokemon.h"
#include "party.h"
#include "render_window.h"
#include "render_text.h"
#include "system.h"
#include "sys_flags.h"
#include "sound.h"
#include "sound_02004A44.h"
#include "touch_hitbox_controller.h"
#include "vram_transfer_manager.h"
#include "bg_window.h"
#include "dwcaccount.h"
#include "msgdata.h"
#include "text.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "gf_gfx_loader.h"
#include "overlay_00_thumb.h"
#include "unk_0200ACF0.h"
#include "unk_02013534.h"
#include "unk_0200A090.h"
#include "unk_0202C034.h"
#include "unk_02030A98.h"
#include "unk_02005D10.h"
#include "unk_0200FA24.h"
#include "unk_02037C94.h"
#include "unk_02034354.h"
#include "unk_020379A0.h"
#include "unk_02035900.h"
#include "unk_020971F8.h"
#include "unk_02034B0C.h"
#include "unk_0200B150.h"
#include "unk_02034354.h"
#include "unk_0203170C.h"
#include "unk_020210A0.h"
#include "unk_020915B0.h"
#include "unk_0203A3B0.h"

extern WindowTemplate ov44_0223537C;
extern WindowTemplate ov44_02235384;
extern GraphicsModes ov44_022353B0;
extern ObjCharTransferTemplate ov44_022353C0;
extern BgTemplate ov44_022353E8;
extern BgTemplate ov44_02235404;
extern BgTemplate ov44_02235420;
extern BgTemplate ov44_0223543C;
extern BgTemplate ov44_02235458;
extern BgTemplate ov44_02235474;
extern BgTemplate ov44_02235490;
extern BgTemplate ov44_022354AC;
extern GraphicsBanks ov44_02235548;
extern func_type_0222A60C ov44_0223676C[20];
extern s32 _022368A0;

FS_EXTERN_OVERLAY(OVY_42);

s32 ov44_02229EE0(UnkStruct_ov44_022319EC* arg0) {
    return Save_VarsFlags_FlypointFlagAction(Save_VarsFlags_Get(arg0->unk160), 2, 27);
}

s32 ov44_02229EF8(UnkStruct_ov44_022319EC* arg0) {
    return 0;
}

s32 ov44_02229EFC(UnkStruct_ov44_022319EC* arg0) {
    return 1;
}

u32 ov44_02229F00(UnkStruct_ov44_022319EC* arg0, UnkStruct_ov44_02231958* arg1) {
    u32 var_r4 = arg1->unk1B;
    if ((var_r4 - 20 <= 1) && (ov44_02229EE0(arg0) == 0)) {
        var_r4 = 29;
    }
    if ((var_r4 - 22 <= 5) && (ov44_02229EFC(arg0) == 0)) {
        var_r4 = 29;
    }
    if ((var_r4 - 18 <= 1) && (ov44_02229EF8(arg0) == 0)) {
        var_r4 = 29;
    }
    return var_r4;
}

s32 ov44_02229F44(s32 arg0) {
    switch (arg0 - 18) {
    case 0:
    case 1:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        return 1;
    default:
        return 0;
    }
}

u32 ov44_02229F74(u32 arg0) {
    switch (arg0 - 18) {
    case 0:
    case 1:
        return 11;
    case 4:
    case 5:
        return 13;
    case 8:
    case 9:
        return 15;
    case 6:
    case 7:
        return 14;
    default:
        GF_AssertFail();
        return 0;
    }
}

s32 ov44_02229FB4(u32 arg0) {
    switch (arg0 - 9) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return 1;
    default:
        return 0;
    }
}

s32 ov44_02229FDC(u32 arg0) {
    if (ov44_02229FB4(arg0) != 0) {
        return 1;
    }
    switch (arg0 - 15) {
        case 0:
        case 4:
        case 6:
        case 8:
        case 10:
        case 12:
            return 1;
        default:
            return 0;
    }
    return 0;
}

s32 ov44_0222A020(u32 arg0) {
    switch (arg0) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        return 1;
    default:
        return 0;
    }
}

s32 ov44_0222A048(u16 arg0) {
    if (ov44_0222A020(arg0) != 0) {
        return 1;
    }
    switch (arg0) {
        case 1:
        case 8:
        case 18:
        case 20:
        case 22:
        case 24:
        case 26:
            return 1;
        default:
            return 0;
    }
}

s32 ov44_0222A090(u32 arg0) {
    u32 temp_r0;

    temp_r0 = arg0 - 12;
    switch (temp_r0) {
    case 0:
    case 1:
    case 2:
    case 3:
        return 1;
    default:
        return 0;
    }
}

u32 ov44_0222A0B4(u32 arg0) {
    u32 var_r1;

    var_r1 = 29;
    switch (arg0) {
    case 19:
        var_r1 = 18;
        break;
    case 21:
        var_r1 = 20;
        break;
    case 23:
        var_r1 = 22;
        break;
    case 27:
        var_r1 = 26;
        break;
    case 25:
        var_r1 = 24;
        break;
    case 15:
        var_r1 = 8;
        break;
    case 9:
        var_r1 = 2;
        break;
    case 10:
        var_r1 = 3;
        break;
    case 11:
        var_r1 = 4;
        break;
    case 12:
        var_r1 = 5;
        break;
    case 13:
        var_r1 = 6;
        break;
    case 14:
        var_r1 = 7;
        break;
    case 16:
        var_r1 = 1;
        break;
    case 18:
        var_r1 = 19;
        break;
    case 20:
        var_r1 = 21;
        break;
    case 22:
        var_r1 = 23;
        break;
    case 26:
        var_r1 = 27;
        break;
    case 24:
        var_r1 = 25;
        break;
    case 8:
        var_r1 = 15;
        break;
    case 2:
        var_r1 = 9;
        break;
    case 3:
        var_r1 = 10;
        break;
    case 4:
        var_r1 = 11;
        break;
    case 5:
        var_r1 = 12;
        break;
    case 6:
        var_r1 = 13;
        break;
    case 7:
        var_r1 = 14;
        break;
    case 1:
        var_r1 = 16;
        break;
    }
    return var_r1;
}

s32 ov44_0222A168(UnkStruct_ov44_022319EC* arg0) {
    Party* party = SaveArray_Party_Get(arg0->unk160);
    int partyCount = Party_GetCount(party);
    int i;
    int var_r5 = 0;
    
    for (i = 0; i < partyCount; i++) {
        Pokemon* pokemon = Party_GetMonByIndex(party, i);
        if ((GetMonData(pokemon, 163, NULL) != 0) && (GetMonData(pokemon, 76, NULL) == 0)) {
            var_r5 += 1;
        }
    }
    return var_r5;
}

void ov44_0222A1B4(u32 arg0) {
    if ((arg0 - 18) <= 1) {
        sub_02039AF8();
        return;
    }
    if (ov44_02229F44(arg0) != 0) {
        sub_02039B18();
        return;
    }
    if (arg0 == 16) {
        sub_02039B58();
        return;
    }
    if ((arg0 - 20) <= 1) {
        sub_020378E4(0);
        sub_02039E9C();
        return;
    }
    sub_020378E4(1);
    sub_02039B7C();
}

void ov44_0222A1FC(UnkStruct_ov44_022319EC* arg0) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    SetKeyRepeatTimers(4, 8);
    NARC* temp_r0 = NARC_New(NARC_a_0_8_8, HEAP_ID_53);
    arg0->unk15C = BgConfig_Alloc(HEAP_ID_53);
    arg0->unk164 = MessageFormat_New(HEAP_ID_53);
    arg0->unk168 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0778_bin, HEAP_ID_53);
    arg0->unk16C = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0800_bin, HEAP_ID_53);
    ov44_0222A830();
    ov44_0222A850(arg0->unk15C);
    ov44_0222AEC0(arg0, temp_r0);
    MI_CpuFill8(&arg0->unkD68, 0, sizeof(UnkStruct_ov44_0222A40C));

    NNSG2dPaletteData* sp2C;
    void* temp_r4 = GfGfxLoader_GetPlttDataFromOpenNarc(temp_r0, 5, &sp2C, HEAP_ID_53);
    MIi_CpuCopy16(sp2C->pRawData, *arg0->unkD68.unk8, 128);
    MIi_CpuCopy16(sp2C->pRawData, *arg0->unkD68.unk88, 128);
    Heap_Free(temp_r4);
    int i;
    int sp18 = 0;
    for (i = 0; i < 3; i++) {
        int j = 0;
        int sp10 = 0;
        int a = i + 1;
        while (TRUE) {
            GF_ASSERT(sp18 < 21);
            for (int k = 1; k < 16; k++) {
                BlendPalette(&arg0->unkD68.unk8[i][k], &arg0->unkD68.unk88[sp18][k], 1, j >> 8, arg0->unkD68.unk8[a][k]);
            }
            sp18++;
            if (sp10 == 1) {
                break;
            }
            j += 768;
            if (j >= 4096) {
                j = 4096;
                sp10 = 1;
            }
        }
    }
    DC_FlushRange(arg0->unkD68.unk88, 672);
    arg0->unkD68.unk4 = 1;
    arg0->unkD68.unk32C = 0;
    arg0->unkD68.unk0 = SysTask_CreateOnVBlankQueue((SysTaskFunc)ov44_0222A40C, &arg0->unkD68, 20);
    Main_SetVBlankIntrCB((GFIntrCB)ov44_0222A7F8, arg0);
    ov44_0222AFE8();
    ov44_0222B030(arg0, temp_r0);
    ov44_0222B0A4(arg0);
    gSystem.screensFlipped = 1;
    GfGfx_SwapDisplay();
    NARC_Delete(temp_r0);
}

void ov44_0222A40C(s32 arg0, UnkStruct_ov44_0222A40C* arg1) {
    if (arg1->unk4 != 0) {
        arg1->unk32B ^= 1;
        if ((1 & arg1->unk32B) == 0) {
            if (arg1->unk32C <= 1) {
                GX_LoadBGPltt(arg1->unk88[arg1->unk328], 0, 32);
            }
            u32 temp_r0 = arg1->unk32C;
            if ((temp_r0 == 0) || (temp_r0 == 2)) {
                GXS_LoadBGPltt(arg1->unk88[arg1->unk328], 0, 32);
            }
            if (arg1->unk32A == 0) {
                arg1->unk328++;
                if (arg1->unk328 >= 21) {
                    arg1->unk328 = 19;
                    arg1->unk32A ^= 1;
                }
            } else {
                arg1->unk328--;
                if (arg1->unk328 < 0) {
                    arg1->unk328 = 1;
                    arg1->unk32A ^= 1;
                }
            }
        }
    }
}

s32 ov44_0222A4B4(OverlayManager* overlayMananger, s32* arg1) {
    switch (*arg1) {
    case 0:
        UnkStruct_ov44_0222A758_args* overlayArgs = OverlayManager_GetArgs(overlayMananger);
        if (sub_02039998() != 0) {
            switch (overlayArgs->unk4) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 12:
                Heap_Create(HEAP_ID_3, HEAP_ID_128, 0x10000);
                break;
            }
        } else {
            Heap_Create(HEAP_ID_3, HEAP_ID_128, 0x10000);
        }
        HandleLoadOverlay(FS_OVERLAY_ID(OVY_42), OVY_LOAD_ASYNC);
        if (overlayArgs->unk4 == 1) {
            Heap_Create(HEAP_ID_3, HEAP_ID_53, 0x70000);
        } else {
            Heap_Create(HEAP_ID_3, HEAP_ID_53, 0xA0000);
        }
        UnkStruct_ov44_022319EC* data = OverlayManager_CreateAndGetData(overlayMananger, 4288, HEAP_ID_53);
        MI_CpuFill8(data, 0, 4288);
        GF_CreateVramTransferManager(32, HEAP_ID_53);
        data->unk180 = 8;
        data->unk160 = overlayArgs->unk0;
        data->unk0 = sub_0202C6F4(data->unk160);
        data->unk354 = overlayArgs->unk4;
        data->unk34C = 8;
        data->unk370 = -1;
        GfGfx_EngineATogglePlanes(15, 0);
        GfGfx_EngineBTogglePlanes(15, 0);
        ov44_0222ADF8(data);
        ov44_0222A1FC(data);
        BeginNormalPaletteFade(0, 1, 1, 0, 6, 1, HEAP_ID_53);
        if (sub_02039998() != 0) {
            sub_0203A880();
        } else {
            LoadDwcOverlay();
            sub_02039FD8(HEAP_ID_53);
        }
        GF_ASSERT(sub_02021148(4) == 1);
        *arg1 += 1;
        break;
    case 1:
        OverlayManager_GetData(overlayMananger);
        *arg1 = 0;
        return 1;
    }
    return 0;
}

s32 ov44_0222A60C(OverlayManager* overlayManager, s32* arg1) {
    UnkStruct_ov44_022319EC* data = OverlayManager_GetData(overlayManager);
    int r1 = arg1[0];
    switch (r1) {
    case 0:
        if (IsPaletteFadeFinished()) {
            *arg1 = 1;
        }
        break;
    case 1:
        int temp_r2 = data->unk348;
        func_type_0222A60C temp_r6 = ov44_0223676C[temp_r2];
        if (temp_r6 != NULL) {
            _022368A0 = temp_r2;
            *arg1 = temp_r6(data, r1);
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            return 1;
        }
    }
    SpriteList* temp_r0 = data->unk190;
    if (temp_r0 != NULL) {
        SpriteList_RenderAndAnimateSprites(temp_r0);
    }
    if (ov44_02231BB0(&data->unk38C) == 1) {
        ov44_02231C60(&data->unk38C);
    }
    return 0;
}

void ov44_0222A68C(UnkStruct_ov44_022319EC* arg0) {
    Main_SetVBlankIntrCB(NULL, 0);
    if (arg0->unkD68.unk0) {
        SysTask_Destroy(arg0->unkD68.unk0);
        arg0->unkD68.unk0 = NULL;
        arg0->unkD68.unk4 = 0;
    }
    if (ov44_02231BB0(&arg0->unk38C) == 1) {
        ov44_02231B4C(&arg0->unk38C);
    }
    if (ov44_0222F98C(arg0) == 1) {
        ov44_0222FA28(arg0);
    }
    ov44_0222ABB8(&arg0->unkB0C);
    sub_020135AC(arg0->unk2CC);
    for (int i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(arg0->unk2BC[i]);
    }
    SpriteList_Delete(arg0->unk190);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    ov44_0222B164(arg0);
    ov44_0222AE74(arg0->unk15C);
    DestroyMsgData(arg0->unk16C);
    DestroyMsgData(arg0->unk168);
    MessageFormat_Delete(arg0->unk164);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_42));
}

s32 ov44_0222A758(OverlayManager* overlayManager) {
    UnkStruct_ov44_022319EC* data = OverlayManager_GetData(overlayManager);
    ov44_0222A68C(data);
    UnkStruct_ov44_0222A758_args* args = OverlayManager_GetArgs(overlayManager);
    args->unk4 = data->unk34C;
    if ((data->unk34C == 8) || (data->unk34C == 10)) {
        UnloadDwcOverlay();
        TextFlags_SetCanTouchSpeedUpPrint(0);
    } else {
        args->unk8 = ov00_021E6EBC();
    }
    ov44_0222AE44(data);
    OverlayManager_FreeData(overlayManager);
    sub_02021238();
    GF_DestroyVramTransferManager();
    Heap_Destroy(HEAP_ID_53);
    switch (data->unk34C) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 12:
        Heap_Destroy(HEAP_ID_128);
        break;
    }
    if (ov44_0222E074() == 1) {
        GF_SndHandleSetInitialVolume(0, 120);
    }
    return 1;
}

void ov44_0222A7F8(UnkStruct_ov44_022319EC* arg0) {
    DoScheduledBgGpuUpdates(arg0->unk15C);
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    if (arg0->unk10BC == 1) {
        SetMasterBrightnessNeutral(0);
        SetMasterBrightnessNeutral(1);
        arg0->unk10BC = 0;
    }
}

void ov44_0222A830(void) {
    GraphicsBanks graphicsBanks = ov44_02235548;
    GfGfx_SetBanks(&graphicsBanks);
}

void ov44_0222A850(BgConfig* arg0) {
    GraphicsModes graphicsModes = ov44_022353B0;
    SetBothScreensModesAndDisable(&graphicsModes);
    
    BgTemplate bgTemplate0 = ov44_022353E8;
    InitBgFromTemplate(arg0, 0, &bgTemplate0, 0);
    BG_ClearCharDataRange(0, 32, 0, HEAP_ID_53);
    BgClearTilemapBufferAndCommit(arg0, 0);
    
    BgTemplate bgTemplate1 = ov44_02235404;
    InitBgFromTemplate(arg0, 1, &bgTemplate1, 0);
    BG_ClearCharDataRange(1, 32, 0, HEAP_ID_53);
    BgClearTilemapBufferAndCommit(arg0, 1);

    BgTemplate bgTemplate2 = ov44_02235490;
    InitBgFromTemplate(arg0, 2, &bgTemplate2, 0);
    BG_ClearCharDataRange(2, 32, 0, HEAP_ID_53);
    BgClearTilemapBufferAndCommit(arg0, 2);

    BgTemplate bgTemplate3 = ov44_02235420;
    InitBgFromTemplate(arg0, 3, &bgTemplate3, 0);
    BG_ClearCharDataRange(3, 32, 0, HEAP_ID_53);
    BgClearTilemapBufferAndCommit(arg0, 3);

    BgTemplate bgTemplate4 = ov44_0223543C;
    InitBgFromTemplate(arg0, 4, &bgTemplate4, 0);
    BgClearTilemapBufferAndCommit(arg0, 4);

    BgTemplate bgTemplate5 = ov44_02235458;
    InitBgFromTemplate(arg0, 5, &bgTemplate5, 0);
    BgClearTilemapBufferAndCommit(arg0, 5);

    BgTemplate bgTemplate6 = ov44_02235474;
    InitBgFromTemplate(arg0, 6, &bgTemplate6, 0);
    BgClearTilemapBufferAndCommit(arg0, 6);

    BgTemplate bgTemplate7 = ov44_022354AC;
    InitBgFromTemplate(arg0, 7, &bgTemplate7, 0);
    BgClearTilemapBufferAndCommit(arg0, 7);
    BG_ClearCharDataRange(0, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(1, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(2, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(3, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(4, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(5, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(6, 32, 0, HEAP_ID_53);
    BG_ClearCharDataRange(7, 32, 0, HEAP_ID_53);
    SetBgPriority(0, 3);
    SetBgPriority(1, 1);
    SetBgPriority(3, 0);
    SetBgPriority(2, 0);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
}

void ov44_0222AA94(UnkStruct_ov44_022319EC* arg0, UnkStruct_ov44_02232B74* arg1) {
    for (int i = 0; i < 33; i++) {
        if (arg0->unkA88[i] == NULL) {
            arg0->unkA88[i] = arg1;
            return;
        }
    }
    GF_AssertFail();
    return;
}

void ov44_0222AAC0(UnkStruct_ov44_022319EC* arg0, UnkStruct_ov44_02232B74* arg1) {
    for (int i = 0; i < 33; i++) {
        if (arg0->unkA88[i] == arg1) {
            arg0->unkA88[i] = NULL;
            return;
        }
    }
    GF_AssertFail();
    return;
}

UnkStruct_ov44_02232B74* ov44_0222AAEC(UnkStruct_ov44_022319EC* arg0, u32 arg1) {
    for (s32 i = 0; i < 33; i++) {
        if ((arg0->unkA88[i] != NULL) && (arg1 == ov44_02231EF4(arg0->unkA88[i]))) {
            return arg0->unkA88[i];
        }
    }
    return NULL;
}

void ov44_0222AB24(BgConfig* arg0, UnkStruct_ov44_0222AB24* arg1, NARC* arg2, enum HeapID arg3) {
    GfGfxLoader_GXLoadPalFromOpenNarc(arg2, 14, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_8_OFFSET, 64, arg3);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg2, 14, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_13_OFFSET, 64, arg3);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg2, 13, arg0, GF_BG_LYR_MAIN_1, 0, 0, 0, arg3);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg2, 13, arg0, GF_BG_LYR_SUB_2, 0, 0, 0, arg3);
    if (arg1->unk8 == 0) {
        arg1->unk8 = GfGfxLoader_GetCharDataFromOpenNarc(arg2, 13, 0, &arg1->unkC, arg3);
    }
    if (arg1->unk0 == NULL) {
        arg1->unk0 = GfGfxLoader_GetScrnDataFromOpenNarc(arg2, 15, 0, &arg1->unk4, arg3);
    }
}

void ov44_0222ABB8(UnkStruct_ov44_0222AB24* arg0) {
    if (arg0->unk0) {
        Heap_Free(arg0->unk0);
        arg0->unk0 = NULL;
    }
    if (arg0->unk8) {
        Heap_Free(arg0->unk8);
        arg0->unk8 = NULL;
    }
}

void ov44_0222ABDC(BgConfig* arg0, UnkStruct_ov44_0222AB24* arg1, s32 arg2, u8 arg3, u8 arg4, s32 arg5, s32 arg6) {
    CopyToBgTilemapRect(arg0, arg2, arg3, arg4, 2, 2, arg1->unk4->rawData, (arg5 * 2), 0, arg1->unk4->screenWidth / 8, arg1->unk4->screenHeight / 8);
    s32 r0 = ov44_0222ADC4(arg2);
    BgTilemapRectChangePalette(arg0, arg2, arg3, arg4, 2, 2, r0 + arg6);
    ScheduleBgTilemapBufferTransfer(arg0, arg2);
}

void ov44_0222AC54(BgConfig* arg0, UnkStruct_ov44_0222AB24* arg1, s32 arg2, u8 arg3, u8 arg4, u32 arg5) {
    s32 var_r4;
    s32 var_r5;

    var_r4 = 0;
    switch (arg5) {
    case 1:
        var_r5 = 3;
        break;
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        var_r4 = 1;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        var_r5 = 6;
        break;
    case 15:
        var_r4 = 1;
    case 8:
        var_r5 = 7;
        break;
    case 23:
    case 25:
    case 27:
        var_r4 = 1;
    case 22:
    case 24:
    case 26:
        var_r5 = 10;
        break;
    case 18:
    case 19:
        var_r5 = 5;
        break;
    case 21:
        var_r4 = 1;
    case 20:
        var_r5 = 11;
        break;
    case 16:
        var_r5 = 4;
        break;
    case 0:
        var_r5 = 8;
        break;
    case 28:
    case 29:
        var_r5 = 5;
        break;
    default:
        var_r5 = 5;
        break;
    }
    ov44_0222ABDC(arg0, arg1, arg2, arg3, arg4, var_r5, var_r4);
}

void ov44_0222ACE8(Window* arg0, UnkStruct_ov44_0222AB24* arg1, u8 arg2, u16 arg3, s32 arg4, s32 arg5) {
    BlitBitmapRectToWindow(arg0, arg1->unkC->pRawData, arg4 * 16, 0, arg1->unkC->W * 8, arg1->unkC->H * 8, arg2, arg3, 16, 16);
    s32 r0 = ov44_0222ADC4(arg0->bgId);
    SetWindowPaletteNum(arg0, r0 + arg5);
}

void ov44_0222AD34(Window* arg0, UnkStruct_ov44_0222AB24* arg1, u8 arg2, u16 arg3, u32 arg4) {
    u8 var_r5;
    u8 var_r4 = 0;
    switch (arg4) {
    case 1:
        var_r5 = 3;
        break;
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        var_r4 = 1;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        var_r5 = 6;
        break;
    case 15:
        var_r4 = 1;
    case 8:
        var_r5 = 7;
        break;
    case 23:
    case 25:
    case 27:
        var_r4 = 1;
    case 22:
    case 24:
    case 26:
        var_r5 = 10;
        break;
    case 18:
    case 19:
        var_r5 = 5;
        break;
    case 21:
        var_r4 = 1;
    case 20:
        var_r5 = 11;
        break;
    case 16:
        var_r5 = 4;
        break;
    case 0:
        var_r5 = 8;
        break;
    case 28:
    case 29:
        var_r5 = 5;
        break;
    default:
        var_r5 = 5;
        break;
    }
    ov44_0222ACE8(arg0, arg1, arg2, arg3, var_r5, var_r4);
}

s32 ov44_0222ADC4(u32 arg0) {
    if (arg0 < 4) {
        return 8;
    }
    return 13;
}

u32 ov44_0222ADD0(void) {
    BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, HEAP_ID_53);
    ov44_0222E030();
    return 18;
}

void ov44_0222ADF8(UnkStruct_ov44_022319EC* arg0) {
    arg0->unk174 = String_New(380, HEAP_ID_53);
    arg0->unk17C = String_New(380, HEAP_ID_53);
    arg0->unk170 = String_New(380, HEAP_ID_53);
    arg0->unk178 = String_New(180, HEAP_ID_53);
    arg0->unk348 = 0;
}

void ov44_0222AE44(UnkStruct_ov44_022319EC* arg0) {
    String_Delete(arg0->unk178);
    String_Delete(arg0->unk174);
    String_Delete(arg0->unk17C);
    String_Delete(arg0->unk170);
}

void ov44_0222AE74(BgConfig* arg0) {
    FreeBgTilemapBuffer(arg0, 7);
    FreeBgTilemapBuffer(arg0, 6);
    FreeBgTilemapBuffer(arg0, 5);
    FreeBgTilemapBuffer(arg0, 4);
    FreeBgTilemapBuffer(arg0, 3);
    FreeBgTilemapBuffer(arg0, 2);
    FreeBgTilemapBuffer(arg0, 1);
    FreeBgTilemapBuffer(arg0, 0);
    Heap_Free(arg0);
}

void ov44_0222AEC0(UnkStruct_ov44_022319EC* arg0, NARC* arg1) {
    BgConfig* bgConfig = arg0->unk15C;
    GfGfx_EngineATogglePlanes(1, 1);
    GfGfx_EngineATogglePlanes(2, 1);
    GfGfx_EngineATogglePlanes(4, 1);
    GfGfx_EngineATogglePlanes(8, 1);
    GfGfx_EngineBTogglePlanes(1, 1);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg1, 3, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_53);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg1, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, HEAP_ID_53);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_53);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_53);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg1, 2, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, HEAP_ID_53);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg1, 6, bgConfig, GF_BG_LYR_MAIN_0, 0, 0, 0, HEAP_ID_53);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg1, 11, bgConfig, GF_BG_LYR_SUB_0, 0, 0, 0, HEAP_ID_53);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg1, 12, bgConfig, GF_BG_LYR_SUB_0, 0, 0, 0, HEAP_ID_53);
    Options* options = Save_PlayerData_GetOptionsAddr(arg0->unk160);
    LoadUserFrameGfx2(bgConfig, GF_BG_LYR_MAIN_2, 482, 10, Options_GetFrame(options), HEAP_ID_53);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_2, 473, 11, 0, HEAP_ID_53);
    LoadUserFrameGfx1(bgConfig, GF_BG_LYR_MAIN_1, 473, 11, 0, HEAP_ID_53);
}

void ov44_0222AFE8(void) {
    ObjCharTransferTemplate objCharTransferTemplate = ov44_022353C0;
    ObjCharTransfer_InitEx(&objCharTransferTemplate, GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(20, HEAP_ID_53);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
    G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_128K);
    G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
}

void ov44_0222B030(UnkStruct_ov44_022319EC* arg0, NARC* arg1) {
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, HEAP_ID_53);
    arg0->unk190 = G2dRenderer_Init(8, &arg0->unk194, HEAP_ID_53);
    G2dRenderer_SetSubSurfaceCoords(&arg0->unk194, 0, 0x100000);
    for (int i = 0; i < 4; i ++) {
        arg0->unk2BC[i] = Create2DGfxResObjMan(1, (GfGfxResType)i, HEAP_ID_53);
    }
    arg0->unk2CC = FontSystem_NewInit(1, HEAP_ID_53);
}

void ov44_0222B0A4(UnkStruct_ov44_022319EC* arg0) {
    GfGfx_EngineATogglePlanes(16, 1);
}

void ov44_0222B0B0(UnkStruct_ov44_022319EC* arg0) {
    if (WindowIsInUse(&arg0->unk2F0) != 0) {
        sub_0200E5D4(&arg0->unk2F0, 0);
        RemoveWindow(&arg0->unk2F0);
    }
    AddWindowParameterized(arg0->unk15C, &arg0->unk2F0, 3, 5, 1, 22, 2, 13, 458);
    FillWindowPixelBuffer(&arg0->unk2F0, 0);

    // Nintendo Wi-Fi Connection
    ReadMsgDataIntoString(arg0->unk168, 21, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unk2F0, 1, arg0->unk178, FontID_String_GetCenterAlignmentX(0, arg0->unk178, 0, 176), 0, 255, 0xF0E00, NULL);
    ScheduleWindowCopyToVram(&arg0->unk2F0);
}

void ov44_0222B164(UnkStruct_ov44_022319EC* arg0) {
    ov44_0222F7BC(arg0);
    if (WindowIsInUse(&arg0->unk2F0) != 0) {
        RemoveWindow(&arg0->unk2F0);
    }
    if (WindowIsInUse(&arg0->unk2D0) != 0) {
        RemoveWindow(&arg0->unk2D0);
    }
    if (WindowIsInUse(&arg0->unk330) != 0) {
        RemoveWindow(&arg0->unk330);
    }
    if (WindowIsInUse(&arg0->unk2E0) != 0) {
        RemoveWindow(&arg0->unk2E0);
    }
    if (WindowIsInUse(&arg0->unk310) != 0) {
        RemoveWindow(&arg0->unk310);
    }
    if (arg0->unk150 != NULL) {
        DestroyListMenu(arg0->unk150, NULL, NULL);
    }
    if (arg0->unk14C != NULL) {
        ListMenuItems_Delete(arg0->unk14C);
        arg0->unk14C = NULL;
    }
    if (WindowIsInUse(&arg0->unk300) != 0) {
        RemoveWindow(&arg0->unk300);
    }
}

s32 ov44_0222B228(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (sub_02039998() != 0) {
        ov44_0222F780(arg0, ov00_021E6EBC());
        arg0->unk4 = (UnkStruct_ov44_0223197C*)sub_020398C8();
        u32 temp_r0 = ov44_02229F00(arg0, &arg0->unk4->unk0);
        if ((temp_r0 == 8) || (temp_r0 == 18) || (temp_r0 == 20) || (temp_r0 == 22) || (temp_r0 == 23) || (temp_r0 == 24) || (temp_r0 == 25) || (temp_r0 == 26) || (temp_r0 == 27) || (temp_r0 == 19)) {
            ov44_0222F818(arg0, 16);
            sub_020343E4();
            sub_02039378();
            UnkStruct_ov44_0223197C* temp_r0_2 = (UnkStruct_ov44_0223197C*)sub_020398C8();
            arg0->unk4 = temp_r0_2;
            temp_r0_2->unk0.unk21 = temp_r0_2->unk0.unk22;
            arg0->unk35C = 20;
            arg0->unk348 = 65;
        } else {
            ov44_0222F510(arg0, 77, 0);
            sub_020398D4(1, 1);
            sub_02037AC0(16);
            arg0->unk348 = 61;
        }
    } else {
        Sound_SetSceneAndPlayBGM(11, 1149, 1);
        ov44_0222B0B0(arg0);
        sub_0202C08C(arg0->unk0);
        if (DWC_CheckHasProfile() == 0) {
            SetBgPriority(0, 3);
            SetBgPriority(1, 0);
            SetBgPriority(3, 0);
            SetBgPriority(2, 1);
            ov44_0222F66C(arg0, 18);
            arg0->unk188 = ov44_02231A28(arg0->unk15C, &ov44_02235384, 0);
            arg0->unk348 = 6;
        } else {
            sub_0202C08C(arg0->unk0);
            if (DWC_CheckValidConsole() == 0) {
                ov44_0222F66C(arg0, 20);
                arg0->unk348 = 3;
            } else {
                ov44_0222F510(arg0, 17, 1);
                arg0->unk348 = 1;
            }
        }
    }
    return arg1;
}

s32 ov44_0222B36C(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->unk180) == 0) {
        arg0->unk188 = ov44_02231A28(arg0->unk15C, &ov44_0223537C, 0);
        arg0->unk348 = 2;
    }
    return arg1;
}

s32 ov44_0222B3A8(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    enum YesNoResponse response = YesNoPrompt_HandleInput(arg0->unk188);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(arg0->unk188);
        arg0->unk4 = sub_020396FC(arg0->unk160, 1188);
        ov44_0222F510(arg0, 23, 1);
        GF_ASSERT(arg0->unk18C == NULL);
        arg0->unk18C = WaitingIcon_New(&arg0->unk2D0, 482);
        arg0->unk348 = 14;
    }
    else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(arg0->unk188);
        arg0->unk348 = 34;
    }
    else {
        return arg1;
    }
    return arg1;
}

s32 ov44_0222B42C(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if ((3 & gSystem.newKeys) || (System_GetTouchNew() == 1)) {
        ov44_0222F66C(arg0, 21);
        SetBgPriority(3, 0);
        SetBgPriority(2, 1);
        SetBgPriority(1, 0);
        SetBgPriority(0, 3);
        arg0->unk188 = ov44_02231A28(arg0->unk15C, &ov44_02235384, 1);
        arg0->unk348 = 4;
    }
    return arg1;
}

s32 ov44_0222B494(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    enum YesNoResponse response = YesNoPrompt_HandleInput(arg0->unk188);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(arg0->unk188);
        ClearFrameAndWindow2(&arg0->unk300, 0);
        RemoveWindow(&arg0->unk300);
        ov44_0222F66C(arg0, 22);
        arg0->unk188 = ov44_02231A28(arg0->unk15C, &ov44_02235384, 1);
        arg0->unk348 = 5;
    }
    else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(arg0->unk188);
        ClearFrameAndWindow2(&arg0->unk300, 0);
        RemoveWindow(&arg0->unk300);
        arg0->unk348 = 34;
    }
    else {
        return arg1;
    }
    return arg1;
}

s32 ov44_0222B528(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    enum YesNoResponse response = YesNoPrompt_HandleInput(arg0->unk188);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(arg0->unk188);
        SetBgPriority(3, 0);
        SetBgPriority(2, 0);
        SetBgPriority(1, 1);
        SetBgPriority(0, 3);
        ClearFrameAndWindow2(&arg0->unk300, 0);
        RemoveWindow(&arg0->unk300);
        sub_0202C03C(arg0->unk0);
        void* saveArray = SaveArray_Get(arg0->unk160, 31);
        sub_02031710(saveArray);
        Save_Frontier_GetStatic(arg0->unk160);
        sub_02031188();
        arg0->unk348 = 14;
        arg0->unk4 = sub_020396FC(arg0->unk160, 1188);
        ov44_0222F510(arg0, 23, 1);
        GF_ASSERT(arg0->unk18C == NULL);
        arg0->unk18C = WaitingIcon_New(&arg0->unk2D0, 482);
        arg0->unk368 = 1;
    }
    else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(arg0->unk188);
        SetBgPriority(3, 0);
        SetBgPriority(2, 0);
        SetBgPriority(1, 1);
        SetBgPriority(0, 3);
        ClearFrameAndWindow2(&arg0->unk300, 0);
        RemoveWindow(&arg0->unk300);
        ov44_0222F66C(arg0, 20);
        arg0->unk348 = 3;
    }
    else {
        return arg1;    
    }
    return arg1;
}

s32 ov44_0222B64C(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    enum YesNoResponse response = YesNoPrompt_HandleInput(arg0->unk188);
    if (response == YESNORESPONSE_YES) {
        YesNoPrompt_Destroy(arg0->unk188);
        SetBgPriority(0, 3);
        SetBgPriority(1, 1);
        SetBgPriority(3, 0);
        SetBgPriority(2, 0);
        ClearFrameAndWindow2(&arg0->unk300, 0);
        RemoveWindow(&arg0->unk300);
        arg0->unk348 = 14;
        arg0->unk4 = sub_020396FC(arg0->unk160, 1188);
        ov44_0222F510(arg0, 23, 1);
        GF_ASSERT(arg0->unk18C == NULL);
        arg0->unk18C = WaitingIcon_New(&arg0->unk2D0, 482);
        arg0->unk368 = 1;
    }
    else if (response == YESNORESPONSE_NO) {
        YesNoPrompt_Destroy(arg0->unk188);
        SetBgPriority(0, 3);
        SetBgPriority(1, 1);
        SetBgPriority(3, 0);
        SetBgPriority(2, 0);
        ClearFrameAndWindow2(&arg0->unk300, 0);
        RemoveWindow(&arg0->unk300);
        arg0->unk348 = 34;
    }
    else {
        return arg1;
    }
    return arg1;
}

s32 ov44_0222B744(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    SetBgPriority(0, 3);
    SetBgPriority(1, 1);
    SetBgPriority(3, 0);
    SetBgPriority(2, 0);
    arg0->unk348 = 15;
    return arg1;
}

void ov44_0222B778(UnkStruct_ov44_022319EC* arg0, u32 arg1) {
    PlayerProfile* playerProfile = Save_PlayerData_GetProfile(arg0->unk160);
    Pokedex* pokedex = Save_Pokedex_Get(arg0->unk160);
    Party* party = SaveArray_Party_Get(arg0->unk160);
    SaveWiFiHistory* saveWiFiHistory = Save_WiFiHistory_Get(arg0->unk160);
    s32 partyCount = Party_GetCount(party);

    for (s32 i = 0; i < partyCount; i++) {
        Pokemon* pokemon = Party_GetMonByIndex(party, i);
        arg0->unk4->unk0.unk0[i] = GetMonData(pokemon, 5, 0);
        arg0->unk4->unk0.unkC[i]  = GetMonData(pokemon, 6, 0);
    }
    arg0->unk4->unk0.unk18[0] = PlayerProfile_GetVersion(playerProfile);
    arg0->unk4->unk0.unk18[1] = PlayerProfile_GetLanguage(playerProfile);
    arg0->unk4->unk0.unk18[2] = Pokedex_GetNatDexFlag(pokedex);
    arg0->unk4->unk0.unk1B = 29;
    ov44_0222F830(arg0, arg1);
    arg0->unk4->unk0.unk1C[0] = 0;
    arg0->unk4->unk0.unk1C[1] = PlayerProfile_GetAvatar(playerProfile);
    arg0->unk4->unk0.unk1C[2] = PlayerProfile_GetTrainerGender(playerProfile);
    arg0->unk4->unk0.unk1C[3] = WifiHistory_GetPlayerCountry(saveWiFiHistory);
    arg0->unk4->unk0.unk1C[4] = WiFiHistory_GetPlayerRegion(saveWiFiHistory);
    arg0->unk4->unk0.unk21 = 1;
    arg0->unk4->unk0.unk22 = 1;
    ov00_021E6D60(arg0->unk4, 36);
    ov00_021E6CCC(arg0->unk4->unk24, 36);
}

s32 ov44_0222B870(UnkStruct_ov44_022319EC* arg0) {
    s32 counter = 0;
    for (s32 i = 0; i < 32; i++) {
        arg0->unkC[i] = 0;
        if ((arg0->unk10BE > i) && (ov44_02231974(i) != 0)) {
            UnkStruct_ov44_02231958* temp_r0 = ov44_02231958(arg0, i);
            u32 temp_r0_2 = ov44_02229F00(arg0, temp_r0);
            if ((temp_r0_2 != 0) && (temp_r0_2 != 28) && (temp_r0->unk1C[1] != 0)) {
                arg0->unkC[counter] = i + 1;
                counter += 1;
            } else if ((temp_r0_2 == 28) && (ov44_02230048(arg0, i + 1) != 0)) {
                ov44_02230028(arg0, i + 1);
            }
        }
    }
    return counter;
}

s32 ov44_0222B8E0(UnkStruct_ov44_022319EC* arg0) {
    s32 counter = 0;
    s32 sp8;
    for (s32 i = 0; i < 32; i++) {
        UnkStruct_ov44_02231958* temp_r0 = ov44_02231958(arg0, i);
        s32 temp_r0_2 = ov44_02229F00(arg0, temp_r0);
        if ((temp_r0_2 != arg0->unk4C[i]) || (temp_r0->unk21 != arg0->unkCC[i])) {
            UnkStruct_ov44_02232B74* temp_r0_3 = ov44_0222AAEC(arg0, i + 1);
            if (temp_r0_3 != NULL) {
                sp8 = ov44_02229FDC(arg0->unk4C[i]);
                s32 temp_r0_4 = ov44_02229FDC(temp_r0_2);
                if ((sp8 == 1) && (temp_r0_4 == 0)) {
                    ov44_02231FA8(&arg0->unk38C, temp_r0_3, 0);
                } else if ((sp8 == 0) && (temp_r0_4 == 1)) {
                    ov44_02231FA8(&arg0->unk38C, temp_r0_3, 1);
                }
                arg0->unk4C[i] = temp_r0_2;
                arg0->unkCC[i] = temp_r0->unk21;
                sub_0202C190(arg0->unk0, i, 7, temp_r0->unk1C[1]);
                sub_0202C190(arg0->unk0, i, 8, temp_r0->unk1C[2]);
                counter += 1;
            }
        }
    }
    return counter;
}

void ov44_0222B9A0(UnkStruct_ov44_022319EC* arg0) {
    u32* temp_r0 = sub_020392D8();
    s32 temp_r6 = temp_r0[0];
    s32 var_r5 = ov00_021E6A70(temp_r6, temp_r0[1]);
    s32 var_r7;
    if ((var_r5 == 11) || (temp_r6 == 25)) {
        if (temp_r0[1] != 2) {
            var_r7 = 14;
        } else {
            var_r7 = 11;
        }
        var_r5 = 11;
    } else if (temp_r6 == 26) {
        var_r7 = 13;
        var_r5 = 12;
    } else if (var_r5 >= 0) {
        var_r7 = var_r5;
    } else {
        var_r7 = 11;
    }
    ov44_0222F7BC(arg0);
    BufferIntegerAsString(arg0->unk164, 0, temp_r6, 5, PRINTING_MODE_LEADING_ZEROS, 1);
    arg0->unk35C = 30;
    ov44_0222F66C(arg0, var_r7);
    switch (var_r5) {
        case 1:
        case 4:
        case 5:
        case 11:
            arg0->unk348 = 10;
            return;
        case 6:
        case 7:
        case 8:
        case 9:
            arg0->unk348 = 10;
            return;
        case 10:
            arg0->unk348 = 10;
            return;
    }
    arg0->unk348 = 35;
}

s32 ov44_0222BA6C(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if ((3 & gSystem.newKeys) || (System_GetTouchNew() == 1)) {
        if (WindowIsInUse(&arg0->unk300) != 0) {
            ClearFrameAndWindow2(&arg0->unk300, 0);
            RemoveWindow(&arg0->unk300);
        }
        arg0->unk348 = 8;
    }
    return arg1;
}

s32 ov44_0222BAB8(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    arg0->unk348 = 9;
    return arg1;
}

s32 ov44_0222BAC4(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    arg0->unk348 = 11;
    return arg1;
}

s32 ov44_0222BAD0(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    s32 temp_r1;

    temp_r1 = arg0->unk35C;
    if (temp_r1 > 0) {
        arg0->unk35C = temp_r1 - 1;
        return arg1;
    }
    if ((3 & gSystem.newKeys) || (System_GetTouchNew() == 1)) {
        if (WindowIsInUse(&arg0->unk300) != 0) {
            ClearFrameAndWindow2(&arg0->unk300, 0);
            RemoveWindow(&arg0->unk300);
        }
        ov44_0222F510(arg0, 88, 0);
        arg0->unk348 = 11;
    }
    return arg1;
}