#include "to57_overlay91_c58c_private.h"

int ov91_0225C58C(OverlayManager *manager, int *state) {
    Ov91Work *work = OverlayManager_GetData(manager);
    u8 *args = OverlayManager_GetArgs(manager);
    struct {
        u32 code;
        u32 tmp;
        u8 param[4];
        Ov91Block16 block;
    } stack;

    if (PlazaGameRoster_CheckConnectionLost(work->unk08) == 1) {
        switch (work->unk78) {
        case 0:
            if (IsPaletteFadeFinished() == 1) {
                sub_0200FB70();
            }
            sub_0200FC20(0);
            work->unk78++;
            break;
        case 1:
            if (PlazaGameRoster_ProcessConnectionLost(work->unk08) == 1) {
                work->unk78++;
            }
            break;
        case 2:
            ov91_0225CC4C(work, args);
            return 1;
        default:
            GF_AssertFail();
            return 1;
        }
        return 0;
    }

    switch (*state) {
    case 0:
        work->unk00 = PlazaGameEntry_NewSwalotPlop(work->unk08, 0x6A);
        (*state)++;
        break;
    case 1:
        if (PlazaGameEntry_IsDone(work->unk00) == 1) {
            work->unk70 = PlazaGameEntry_GetSelection(work->unk00);
            PlazaGameEntry_Free(work->unk00);
            work->unk00 = NULL;
            (*state)++;
        }
        break;
    case 2:
        if (gSystem.unk44 & 1) {
            break;
        }
        Main_SetVBlankIntrCB(ov91_0225CB58, work);
        HBlankInterruptDisable();
        work->unk38 = 0;
        work->unk3C = 0;
        work->unk40 = 0;
        {
            u8 *first = &work->unk84.first;
            u8 *bytes = &work->unk84.first;
            *first = 0;
            bytes[1] = 0;
            bytes[2] = 0;
            bytes[3] = 0;
        }
        GF_CreateVramTransferManager(0x20, 0x6A);
        ov91_02261B2C(work);
        work->unk80 = 1;
        work->unk74 = sub_020347A0();
        if (work->unk74 <= 1) {
            GF_AssertFail();
        }
        work->unk44 = Comm_GetLocalNetId();
        work->unk46 = PlazaGameRoster_GetIndexOfNetId(work->unk08, work->unk44);
        if (work->unk44 == 0) {
            work->unk30 = ov91_0225CCC4(0x6A, 0x4B0, work->unk74, work->unk48);
            work->unk7C = 1;
        }
        work->unk34 = ov91_0225CDF4(0x6A, 0x4B0, work->unk74, work->unk46, work->unk48);
        WirelessStatusIcon_Create();
        if (args[0x38] != 0) {
            WifiConnection_StartVoiceChat(0x6A);
        }
        switch (work->unk70) {
        case 0:
            stack.param[0] = 0;
            stack.param[1] = 0;
            break;
        case 1:
            stack.param[0] = 0;
            stack.param[1] = 1;
            break;
        case 2:
            stack.param[0] = 1;
            stack.param[1] = 0;
            break;
        default:
            GF_AssertFail();
            break;
        }
        ov91_0225D3C4(work->unk34, stack.param);
        sub_02037AC0(1);
        (*state)++;
        break;
    case 3:
        if (sub_02037B38(1) != 0) {
            BeginNormalPaletteFade(0, 0x1B, 0x1B, 0xFFFF, 6, 1, 0x6A);
            (*state)++;
        }
        break;
    case 4:
        ov91_0225CEB4(work->unk34, 0);
        if (IsPaletteFadeFinished() != 0) {
            if (work->unk44 == 0) {
                if (Comm_SendCommand(0x16, NULL, 0) != 0) {
                    (*state)++;
                }
            } else {
                (*state)++;
            }
        }
        break;
    case 5:
        ov91_0225CEB4(work->unk34, 0);
        if (work->unk38 != 0) {
            (*state)++;
        }
        break;
    case 6:
        if (ov91_0225CEB4(work->unk34, 1) == 0) {
            (*state)++;
        }
        break;
    case 7:
        if (work->unk3C != 0) {
            stack.tmp = ov91_0225D37C(work->unk34);
            if (Comm_SendCommand(0x19, &stack.tmp, 4) != 0) {
                (*state)++;
            }
        } else {
            if (work->unk44 == 0) {
                u32 status = ov91_0225CCF4(work->unk30);
                if (ov91_0225CD5C(work->unk30) != 0) {
                    stack.code = ov91_0225CD6C(work->unk30);
                    if (ov91_0225CCA8(work, 0x1B, &stack.code, 4) == 1) {
                        ov91_0225CD64(work->unk30);
                    }
                }
                if (status == 0) {
                    ov91_0225CCA8(work, 0x17, NULL, 0);
                }
            }
            ov91_0225D1DC(work->unk34);
            ov91_0225CB64(work);
            ov91_0225CB98(work);
        }
        break;
    case 8:
        ov91_0225D078(work->unk34, 0);
        if (work->unk44 == 0) {
            if (ov91_0225CDAC(work->unk30) == 1) {
                ov91_0225CDC4(work->unk30, &stack.block);
                if (Comm_SendCommand(0x1A, &stack.block, 0x10) != 0) {
                    (*state)++;
                }
            }
        } else {
            (*state)++;
        }
        break;
    case 9:
        ov91_0225D078(work->unk34, 0);
        if (work->unk40 == 1) {
            (*state)++;
        }
        break;
    case 10:
        if (ov91_0225D078(work->unk34, 1) == 0) {
            (*state)++;
        }
        break;
    case 11:
        BeginNormalPaletteFade(0, 0x1A, 0x1A, 0xFFFF, 6, 1, 0x6A);
        ov91_0225D078(work->unk34, 1);
        (*state)++;
        break;
    case 12:
        ov91_0225D078(work->unk34, 1);
        if (IsPaletteFadeFinished() != 0) {
            sub_02037AC0(3);
            (*state)++;
        }
        break;
    case 13:
        if (sub_02037B38(3) == 0) {
            return 0;
        }
        if (args[0x38] != 0) {
            extern void WifiConnection_StopVoiceChat(void);
            WifiConnection_StopVoiceChat();
        }
        if (work->unk44 == 0) {
            ov91_0225CCEC(work->unk30);
            work->unk30 = NULL;
        }
        ov91_0225CE80(work->unk34);
        work->unk34 = NULL;
        Main_SetVBlankIntrCB(NULL, NULL);
        HBlankInterruptDisable();
        GF_DestroyVramTransferManager();
        (*state)++;
        break;
    case 14:
        PlazaGame_CalcRanks(work->unk18, work->unk10);
        work->unk04 = PlazaGameResults_NewSwalotPlop(work->unk08, work->unk18, 0x6A);
        (*state)++;
        break;
    case 15:
        if (PlazaGameResults_IsDone(work->unk04) == 1) {
            u32 result = PlazaGameResults_GetContinue(work->unk04);
            PlazaGameResults_Free(work->unk04);
            work->unk04 = NULL;
            if (result == 0) {
                return 1;
            }
            *state = 0;
        }
        break;
    }
    return 0;
}
