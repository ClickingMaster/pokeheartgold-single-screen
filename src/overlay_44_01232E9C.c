#include "overlay_44_02232E9C.h"

#include <nitro/gx.h>

#include "save_frontier.h"
#include "render_window.h"
#include "render_text.h"
#include "system.h"
#include "sound.h"
#include "sound_02004A44.h"
#include "touch_hitbox_controller.h"
#include "bg_window.h"
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

extern u8 _0223535C[4];
extern u8 ov44_02235360[4];
extern u8 ov44_02235364[4];
extern u8 ov44_02235368[4];
extern s8 ov44_0223536C[8];
extern const TouchscreenHitbox ov44_02235394;
extern u32 ov44_022353D0[6];
extern SpriteTemplate ov44_02235570[3];
extern func_type_02236680 ov44_02236680[6];

extern UnkStruct_ov44_02231A28 ov44_02235374;
extern const WindowTemplate ov44_0223538C;
extern TouchscreenHitbox ov44_022354E8[4];

extern test _02236660[4];
extern test ov44_022353A0[2];
extern ListMenuTemplate ov44_022354C8;
extern ListMenuTemplate ov44_022366FC;
extern test ov44_0223671C[4];

extern ListMenuTemplate ov44_02235508;
extern ListMenuTemplate ov44_02235528;
extern test ov44_0223669C[4];
extern test ov44_022366BC[4];
extern test ov44_022366DC[4];

s32 ov44_0222CE40(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
        s32 temp_r0 = Handle2dMenuInput_DeleteOnFinish(arg0->unk184, HEAP_ID_53);
        if (temp_r0 == -1) {
            if ((sub_020390C4() >= 4) || (sub_02039264() != 0) || (sub_020373B4(0) == 0)) {
                ov44_0222F7BC(arg0);
                ov44_0222F818(arg0, 16);
                Clear2dMenuWindowAndDelete(arg0->unk184, HEAP_ID_53);
                arg0->unk370 = -1;
                arg0->unk35C = 20;
                arg0->unk348 = 65;
            } else if (sub_020393C8() != 0) {
                ov44_0222B9A0(arg0);
            }
            return arg1;
        }
        if (temp_r0 == 0) {
            if (sub_020393C8() == 0) {
                ov44_0222F818(arg0, 16);
                sub_020393B4();
                arg0->unk370 = -1;
                arg0->unk35C = 20;
                arg0->unk348 = 65;
            } else {
                ov44_0222B9A0(arg0);
            }
        } else {
            arg0->unk348 = 24;
        }
    }
    else {
        enum YesNoResponse temp_r0_2 = YesNoPrompt_HandleInput(arg0->unk188);
        if (temp_r0_2 == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(arg0->unk188);
            if (sub_020393C8() == 0) {
                ov44_0222F818(arg0, 16);
                sub_020393B4();
                arg0->unk370 = -1;
                arg0->unk35C = 20;
                arg0->unk348 = 65;
            } else {
                ov44_0222B9A0(arg0);
            }
        }
        else if (temp_r0_2 == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(arg0->unk188);
            arg0->unk348 = 24;
        }
        else {
            if ((sub_020390C4() >= 4) || (sub_02039264() != 0) || (sub_020373B4(0) == 0)) {
                ov44_0222F7BC(arg0);
                ov44_0222F818(arg0, 16);
                YesNoPrompt_Destroy(arg0->unk188);
                arg0->unk370 = -1;
                arg0->unk35C = 20;
                arg0->unk348 = 65;
            } 
            else if (sub_020393C8() != 0) {
                ov44_0222B9A0(arg0);
            }
            return arg1;
        }
    }
    ov44_0222F7BC(arg0);
    return arg1;
}

s32 ov44_0222CFE0(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (sub_020393C8() != 0) {
        ov44_0222B9A0(arg0);
        return arg1;
    }
    if (ov44_02229F00(arg0, &arg0->unk4->unk0) != 16) {
        ov44_0222F818(arg0, 16);
    }
    if (TextPrinterCheckActive(arg0->unk180) != 0) {
        arg0->unk35C = 30;
        return arg1;
    }
    arg0->unk35C -= 1;
    if ((3 & gSystem.newKeys) || (arg0->unk35C == 0) || (System_GetTouchNew() == 1)) {
        ov44_0222F7BC(arg0);
        sub_02039358();
        sub_02039B58();
        if (ov44_02231BB0(&arg0->unk38C) == 0) {
            arg0->unk35C = 20;
            arg0->unk348 = 65;
        } else {
            ov44_0222F8F0(arg0);
            ov44_0222C120(arg0);
            arg0->unk370 = -1;
            arg0->unk348 = 19;
            ov44_022319EC(arg0);
        }
    }
    return arg1;
}

s32 ov44_0222D0A4(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (sub_020393C8() != 0) {
        ov44_0222B9A0(arg0);
        return arg1;
    }
    if (TextPrinterCheckActive(arg0->unk180) != 0) {
        return arg1;
    }
    if ((3 & gSystem.newKeys) || (System_GetTouchNew() == 1)) {
        ov44_0222F7BC(arg0);
        ov44_0222F818(arg0, 16);
        sub_02039358();
        arg0->unk35C = 20;
        arg0->unk348 = 65;
    }
    return arg1;
}

s32 ov44_0222D10C(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (sub_020393C8() != 0) {
        ov44_0222B9A0(arg0);
        return arg1;
    }
    if (TextPrinterCheckActive(arg0->unk180) != 0) {
        arg0->unk35C = 30;
        return arg1;
    }
    arg0->unk35C -= 1;
    if ((3 & gSystem.newKeys) || (arg0->unk35C == 0) || (System_GetTouchNew() == 1)) {
        ov44_0222F7BC(arg0);
        ov44_0222F818(arg0, 16);
        sub_02039B58();
        if (ov44_02231BB0(&arg0->unk38C) == 0) {
            arg0->unk348 = ov44_0222ADD0();
        } else {
            ov44_0222F8F0(arg0);
            ov44_0222C120(arg0);
            arg0->unk370 = -1;
            arg0->unk348 = 19;
            ov44_022319EC(arg0);
        }
    }
    return arg1;
}

s32 ov44_0222D1BC(s32 arg0, s32 arg1) {
    return arg1;
}

s32 ov44_0222D1C0(s32 arg0, s32 arg1) {
    return arg1;
}

s32 ov44_0222D1C4(s32 arg0, s32 arg1) {
    return arg1;
}

s32 ov44_0222D1C8(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (arg0->unk35C > 0) {
        arg0->unk35C--;
        return arg1;
    }
    if ((3 & gSystem.newKeys) || (System_GetTouchNew() == 1)) {
        ov44_0222F510(arg0, 26, 1);
        arg0->unk348 = 59;
        arg0->unk35C = 1;
    }
    return arg1;
}

s32 ov44_0222D214(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    ov44_02232018(&arg0->unk38C);
    ov44_0222F510(arg0, 10, 0);
    arg0->unk348 = 40;
    return arg1;
}

s32 ov44_0222D23C(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    ov44_02232018(&arg0->unk38C);
    if (TextPrinterCheckActive(arg0->unk180) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            arg0->unk184 = Std_CreateYesNoMenu(arg0->unk15C, &ov44_0223538C, 473, 11, HEAP_ID_53);
        } else {
            arg0->unk188 = ov44_02231A14(arg0->unk15C, &ov44_02235374, 0);
        }
        arg0->unk348 = 41;
    }
    return arg1;
}

s32 ov44_0222D2B0(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
        s32 var_r5 = Handle2dMenuInput_DeleteOnFinish(arg0->unk184, HEAP_ID_53);
        ov44_02232018(&arg0->unk38C);
        if ((arg0->unk370 == -1) && (ov00_021E709C() != -1)) {
            ov00_021E70B8(arg0->unk4->unk0.unk21);
            if (var_r5 == -1) {
                Clear2dMenuWindowAndDelete(arg0->unk184, HEAP_ID_53);
            }
            var_r5 = 1;
        }
        if (var_r5 == -1) {
            return arg1;
        }
        else if (var_r5 == 0) {
            ov44_0222F818(arg0, 16);
            sub_02039B58();
            ov44_022319EC(arg0);
        } else {
            ov44_022319EC(arg0);
            ov44_022319BC(arg0, 1);
        }
    }
    else {
        enum YesNoResponse var_r5_2 = YesNoPrompt_HandleInput(arg0->unk188);
        ov44_02232018(&arg0->unk38C);
        if ((arg0->unk370 == -1) && (ov00_021E709C() != -1)) {
            ov00_021E70B8(arg0->unk4->unk0.unk21);
            var_r5_2 = YESNORESPONSE_NO;
        }
        if (var_r5_2 == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(arg0->unk188);
            ov44_0222F818(arg0, 16);
            sub_02039B58();
            ov44_022319EC(arg0);
        }
        else if (var_r5_2 == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(arg0->unk188);
            ov44_022319EC(arg0);
            ov44_022319BC(arg0, 1);
        }
        else {
            return arg1;
        }
    }
    arg0->unk348 = 19;
    return arg1;
}

s32 ov44_0222D3DC(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    s32 listLen = 3;
    ListMenuTemplate listMenuTemplate = ov44_022366FC; 
    test* listMenuItems = ov44_0223671C;

    if (ov44_02229EF8(arg0) == 1) {
        listLen++;
        listMenuTemplate.totalItems++;
        listMenuTemplate.maxShowed++;
        
        s32 index = listLen - 1;
        listMenuItems[index] = _02236660[0];
    }
    if (ov44_02229EE0(arg0) == 1) {
        listLen++;
        listMenuTemplate.totalItems++;
        listMenuTemplate.maxShowed++;

        s32 index = listLen - 1;
        listMenuItems[index] = _02236660[3];
    }
    if (ov44_02229EFC(arg0) == 1) {
        listLen++;
        listMenuTemplate.totalItems++;
        listMenuTemplate.maxShowed++;

        s32 index = listLen - 1;
        listMenuItems[index] = _02236660[1];
    }
    listMenuTemplate.totalItems++;
    listMenuTemplate.maxShowed++;
    listMenuItems[listLen] = _02236660[2];
    arg0->unk154 = ListMenuItems_New(listLen + 1, HEAP_ID_53);

    for (s32 i = 0; i < listLen + 1; i++) {
        ListMenuItems_AppendFromMsgData(arg0->unk154, arg0->unk168, listMenuItems[i].strno, listMenuItems[i].value);
    }
    if (WindowIsInUse(&arg0->unk320) != 0) {
        sub_0200E5D4(&arg0->unk320, 0);
        RemoveWindow(&arg0->unk320);
    }
    s32 height = (listLen + 1) * 2;
    AddWindowParameterized(arg0->unk15C, &arg0->unk320, 2, 16, 3, 15, height, 13, 59 - height);
    DrawFrameAndWindow1(&arg0->unk320, 1, 473, 11);
    listMenuTemplate.items = arg0->unk154;
    listMenuTemplate.window = &arg0->unk320;
    arg0->unk158 = ListMenuInit(&listMenuTemplate, 0, arg0->unk374, HEAP_ID_53);
    ScheduleWindowCopyToVram(&arg0->unk320);
    ov44_0222F510(arg0, 8, 0);
    ov44_02232018(&arg0->unk38C);
    arg0->unk348 = 37;
    return arg1;
}

s32 ov44_0222D594(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    s32 sp0 = ov44_0222A168(arg0);
    s32 temp_r6 = ov44_0222E02C(arg0);
    ov44_02232018(&arg0->unk38C);
    if (sub_020393C8() != 0) {
        sub_0200E5D4(&arg0->unk320, 0);
        RemoveWindow(&arg0->unk320);
        DestroyListMenu(arg0->unk158, NULL, &arg0->unk374);
        ListMenuItems_Delete(arg0->unk154);
        ov44_0222F7BC(arg0);
        ov44_0222B9A0(arg0);
        return arg1;
    }
    if (TextPrinterCheckActive(arg0->unk180) != 0) {
        if (ov44_0222C500(arg0) != 0) {
            arg0->unk348 = 19;
            sub_0200E5D4(&arg0->unk320, 0);
            RemoveWindow(&arg0->unk320);
            DestroyListMenu(arg0->unk158, NULL, &arg0->unk374);
            ListMenuItems_Delete(arg0->unk154);
            ov44_0222F7BC(arg0);
        }
        return arg1;
    }
    s32 var_r4 = ListMenu_ProcessInput(arg0->unk158);
    if (ov44_0222C500(arg0) != 0) {
        var_r4 = -2;
    }
    switch (var_r4) {
        case -1:
            return arg1;
        case -2:
            arg0->unk348 = 19;
            ov44_0222F7BC(arg0);
            PlaySE(1501);
        break;
        default:
        PlaySE(1501);
        if ((var_r4 == 19) && (temp_r6 != 0)) {
            if (temp_r6 == 1) {
                ov44_0222F510(arg0, 104, 0);
            } else if (temp_r6 == 3) {
                ov44_0222F510(arg0, 106, 0);
            } else {
                ov44_0222F510(arg0, 105, 0);
            }
            arg0->unk348 = 29;
        }
        else if ((var_r4 == 15) && (sp0 < 2)) {
            ov44_0222F510(arg0, 89, 0);
            arg0->unk348 = 29;
        }
        else if ((var_r4 == 1) && (sp0 < 2)) {
            ov44_0222F510(arg0, 103, 0);
            arg0->unk348 = 29;
        }
        else if ((var_r4 == 0) || (var_r4 == 1) || (var_r4 == 29)) {
            sub_0200E5D4(&arg0->unk320, 0);
            RemoveWindow(&arg0->unk320);
            DestroyListMenu(arg0->unk158, NULL, &arg0->unk374);
            ListMenuItems_Delete(arg0->unk154);
            ov44_0222D8B0(arg0, var_r4);
            arg0->unk348 = 38;
            return arg1;
        }
        else {
            ov44_0222F818(arg0, var_r4);
            arg0->unk348 = 19;
            ov44_0222F7BC(arg0);
        }
        s32 var_r6 = 1;
        if ((ov44_02229F44(var_r4) != 0) && (arg0->unk348 == 19)) {
            arg0->unk348 = 68;
            ov44_0223197C(arg0, -1, var_r4);
            ov44_0222A1B4(var_r4);
            ov44_0222F818(arg0, var_r4);
            arg0->unk34C = ov44_02229F74(var_r4);
            var_r6 = 0;
        } else {
            ov44_0223197C(arg0, -1, var_r4);
            ov44_0222A1B4(var_r4);
        }
        if (arg0->unk348 == 19) {
            ov44_022319BC(arg0, var_r6);
        }
    } 

    sub_0200E5D4(&arg0->unk320, 0);
    RemoveWindow(&arg0->unk320);
    DestroyListMenu(arg0->unk158, NULL, &arg0->unk374);
    ListMenuItems_Delete(arg0->unk154);
    return arg1;
}

s32 ov44_0222D824(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (sub_020393C8() != 0) {
        ov44_0222B9A0(arg0);
        return arg1;
    }
    ov44_02232018(&arg0->unk38C);
    if (ov44_0222C500(arg0) != 0) {
        arg0->unk348 = 19;
    }
    if (sub_020392A0() != 0) {
        sub_020398D4(0, 1);
        sub_020378E4(0);
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, HEAP_ID_53);
        arg0->unk384 = 0;
        sub_02034354(arg0->unk160, 0);
        sub_02034434();
        sub_0203476C(sub_0203769C());
        arg1 = 2;
    }
    return arg1;
}

s32 ov44_0222D8B0(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    ListMenuTemplate listMenuTemplate;
    test* listMenuItems;
    s32 listLen;
    switch (arg1) {
    case 0:
        arg0->unk37C = 1;
        listMenuItems = ov44_022366BC;
        listLen = NELEMS(ov44_022366BC);
        listMenuTemplate = ov44_02235508;
        break;
    case 1:
        arg0->unk37C = 0;
        listMenuItems = ov44_022366DC;
        listLen = NELEMS(ov44_022366DC);
        listMenuTemplate = ov44_02235508;
        break;
    case 29:
        listMenuItems = ov44_0223669C;
        listLen = NELEMS(ov44_0223669C);
        listMenuTemplate = ov44_02235528;
        arg0->unk37C = 2;
        break;
    }
    arg0->unk154 = ListMenuItems_New(listLen, HEAP_ID_53);
    for (s32 i = 0; i < listLen; i++) {
        if (listMenuItems[i].strno != 71) {
            ListMenuItems_AppendFromMsgData(arg0->unk154, arg0->unk168, listMenuItems[i].strno, listMenuItems[i].value);
        } else {
            BufferWiFiPlazaActivityName(arg0->unk164, 0, i);
            ReadMsgDataIntoString(arg0->unk168, listMenuItems[i].strno, arg0->unk170);
            StringExpandPlaceholders(arg0->unk164, arg0->unk178, arg0->unk170);
            ListMenuItems_AddItem(arg0->unk154, arg0->unk178, listMenuItems[i].value);
        }
    }
    if (WindowIsInUse(&arg0->unk320) != 0) {
        sub_0200E5D4(&arg0->unk320, 0);
        RemoveWindow(&arg0->unk320);
    }
    u32 temp_r0 = listLen * 2;
    AddWindowParameterized(arg0->unk15C, &arg0->unk320, 2, 16, 9, 15, temp_r0, 13, (59 - temp_r0));
    DrawFrameAndWindow1(&arg0->unk320, 1, 473, 11);
    listMenuTemplate.items = arg0->unk154;
    listMenuTemplate.window = &arg0->unk320;
    arg0->unk158 = ListMenuInit(&listMenuTemplate, 0, (&arg0->unk376)[arg0->unk37C], HEAP_ID_53);
    ScheduleWindowCopyToVram(&arg0->unk320);
    return 1;
}

s32 ov44_0222DA64(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    ov44_02232018(&arg0->unk38C);
    if (sub_020393C8() != 0) {
        sub_0200E5D4(&arg0->unk320, 0);
        RemoveWindow(&arg0->unk320);
        DestroyListMenu(arg0->unk158, 0, &arg0->unk376 + arg0->unk37C);
        ListMenuItems_Delete(arg0->unk154);
        ov44_0222F7BC(arg0);
        ov44_0222B9A0(arg0);
        return arg1;
    }
    if (ov44_0222C500(arg0) != 0) {
        arg0->unk348 = 19;
        sub_0200E5D4(&arg0->unk320, 0);
        RemoveWindow(&arg0->unk320);
        DestroyListMenu(arg0->unk158, 0, &arg0->unk376 + arg0->unk37C);
        ListMenuItems_Delete(arg0->unk154);
        ov44_0222F7BC(arg0);
        return arg1;
    }
    if (TextPrinterCheckActive(arg0->unk180) != 0) {
        return arg1;
    }
    s32 temp_r0 = ListMenu_ProcessInput(arg0->unk158);
    switch (temp_r0) {
        case -1:
            return arg1;
        case -2:
            PlaySE(1501);
            arg0->unk348 = 36;
            break;
        default:
            PlaySE(1501);
            ov44_0222F818(arg0, temp_r0);
            arg0->unk348 = 19;
            ov44_0222F7BC(arg0);
    }
    if (arg0->unk348 == 19) {
        s32 var_r7 = 1;
        if (ov44_02229F44(temp_r0) != 0) {
            arg0->unk348 = 68;
            ov44_0223197C(arg0, -1, temp_r0);
            ov44_0222A1B4(temp_r0);
            ov44_0222F818(arg0, temp_r0);
            arg0->unk34C = ov44_02229F74(temp_r0);
            var_r7 = 0;
        } else {
            ov44_0223197C(arg0, -1, temp_r0);
            sub_02039B7C();
        }
        ov44_022319BC(arg0, var_r7);
    }
    sub_0200E5D4(&arg0->unk320, 0);
    RemoveWindow(&arg0->unk320);
    DestroyListMenu(arg0->unk158, 0, &arg0->unk376 + arg0->unk37C);
    ListMenuItems_Delete(arg0->unk154);
    return arg1;
}

s32 ov44_0222DC18(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    u8 temp_r5 = ov44_02231C70(&arg0->unk38C);
    UnkStruct_ov44_02232B74* temp_r0 = ov44_0222AAEC(arg0, 0);
    GF_ASSERT(temp_r0 != 0);
    UnkStruct_ov44_02232B74* temp_r0_2 = ov44_0222AAEC(arg0, temp_r5);
    if (temp_r0_2 == 0) {
        ov44_0222F780(arg0, temp_r5 - 1);
        ov44_0222F510(arg0, 16, 0);
        arg0->unk348 = 29;
        return arg1;
    }
    ov44_02231F14(&arg0->unk38C, temp_r0_2, ov44_02231F04(temp_r0));
    u32 temp_r7 = temp_r5 - 1;
    u16 temp_r5_2 = ov44_02229F00(arg0, ov44_02231958(arg0, temp_r7));
    arg0->unk37E = temp_r5_2;
    ov44_0222F780(arg0, temp_r7);
    s32 var_r1 = 0;

    if (temp_r5_2 == 15) {
        var_r1 = 3;
    }
    else if (temp_r5_2 == 19) {
        var_r1 = 4;
    }
    else if (temp_r5_2 == 18) {
        var_r1 = 83;
    }
    else if (temp_r5_2 == 21) {
        var_r1 = 5;
    }
    else if (temp_r5_2 == 20) {
        var_r1 = 84;
    }
    else if (temp_r5_2 == 23) {
        BufferWiFiPlazaActivityName(arg0->unk164, 1, 0);
        var_r1 = 6;
    }
    else if (temp_r5_2 == 22) {
        var_r1 = 85;
    }
    else if (temp_r5_2 == 25) {
        BufferWiFiPlazaActivityName(arg0->unk164, 1, 1);
        var_r1 = 6;
    }
    else if (temp_r5_2 == 24) {
        var_r1 = 85;
    }
    else if (temp_r5_2 == 27) {
        BufferWiFiPlazaActivityName(arg0->unk164, 1, 2);
        var_r1 = 6;
    }
    else if (temp_r5_2 == 26) {
        var_r1 = 85;
    }
    else {
        if (ov44_02229FB4(temp_r5_2) != 0) {
            var_r1 = 2;
        } else if (temp_r5_2 == 8) {
            var_r1 = 82;
        } else if (ov44_0222A020(temp_r5_2) != 0) {
            var_r1 = 81;
        } else if (temp_r5_2 == 1) {
            var_r1 = 86;
        } else {
            var_r1 = 7;
        }
    }
    ov44_0222F510(arg0, var_r1, 0);
    arg0->unk348 = 43;
    return arg1;
}

s32 ov44_0222DD64(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    s32 i;
    if (sub_020393C8() != 0) {
        ov44_0222B9A0(arg0);
        return arg1;
    }
    s32 temp_r0 = ov44_02231C70(&arg0->unk38C);
    UnkStruct_ov44_02232B74* temp_r6 = ov44_0222AAEC(arg0, temp_r0);
    if (ov44_0222C500(arg0) != 0) {
        ov44_02231F88(&arg0->unk38C, temp_r6);
        ov44_0222F7BC(arg0);
        arg0->unk348 = 19;
        return arg1;
    }
    if (temp_r6 == 0) {
        ov44_02231F88(&arg0->unk38C, temp_r6);
        DestroyListMenu(arg0->unk158, 0, 0);
        ov44_0222F780(arg0, temp_r0 - 1);
        ov44_0222F510(arg0, 16, 0);
        arg0->unk348 = 29;
        return arg1;
    }
    if (arg0->unk37E != ov44_02229F00(arg0, ov44_02231958(arg0, temp_r0 - 1))) {
        ov44_02231F88(&arg0->unk38C, temp_r6);
        ov44_0222F780(arg0, temp_r0 - 1);
        ov44_0222F510(arg0, 16, 0);
        arg0->unk348 = 29;
        return arg1;
    }
    if (TextPrinterCheckActive(arg0->unk180) != 0) {
        return arg1;
    }
    s32 sp1C = 2;
    ListMenuTemplate listMenuTemplate = ov44_022354C8;
    test* listMenuItems = ov44_022353A0;
    UnkStruct_ov44_02231958* r6 = ov44_02231958(arg0, ov44_02231C70(&arg0->unk38C) - 1);
    u16 temp_r4 = ov44_02229F00(arg0, r6);
    u8 r1 = r6->unk21;
    arg0->unk380 = r1;
    u16 val = r1 + arg0->unk4->unk0.unk21;
    arg0->unk154 = ListMenuItems_New(2, HEAP_ID_53);
    

    for (i = 0; i < NELEMS(ov44_022353A0); i++) {
        if (i == 0) {
            if ((ov44_0222A048(temp_r4) != 0) || (temp_r4 == 0) || (temp_r4 == 28) || (temp_r4 >= 29)) {
                listMenuTemplate.maxShowed--;
                listMenuTemplate.totalItems--;
                sp1C -= 1;
            } else if (temp_r4 == 16) {
                if (val == 2) {
                    ListMenuItems_AppendFromMsgData(arg0->unk154, arg0->unk168, 36, listMenuItems[i].value);
                } else {
                    listMenuTemplate.maxShowed--;
                    listMenuTemplate.totalItems--;
                    sp1C -= 1;
                }
            } else {
                ListMenuItems_AppendFromMsgData(arg0->unk154, arg0->unk168, listMenuItems[i].strno, listMenuItems[i].value);
            }
        } else {
            ListMenuItems_AppendFromMsgData(arg0->unk154, arg0->unk168, listMenuItems[i].strno, listMenuItems[i].value);
        }
    }
    AddWindowParameterized(arg0->unk15C, &arg0->unk320, 2, 16, (((3 - sp1C) * 2) + 11), 15, (sp1C * 2), 13, 59);
    DrawFrameAndWindow1(&arg0->unk320, 1, 473, 11);
    listMenuTemplate.items = arg0->unk154; //itemPrintFunc
    listMenuTemplate.window = &arg0->unk320;
    arg0->unk158 = ListMenuInit(&listMenuTemplate, 0, 0, HEAP_ID_53);
    ScheduleWindowCopyToVram(&arg0->unk320);
    arg0->unk348 = 44;
    return arg1;
}

s32 ov44_0222DFEC(UnkStruct_ov44_02231958* arg0) {
    for (s32 i = 0; i < 6; i++) {
        if (arg0->unk0[i] == 495) {
            return 0;
        }
        if (arg0->unk0[i] > 495) {
            return 0;
        }
        if (arg0->unkC[i] > 536) {
            return 0;
        }
    }
    return 1;
}

s32 ov44_0222E02C(UnkStruct_ov44_022319EC* arg0) {
    return 3;
}

void ov44_0222E030(void) {
    ov00_021E70B8(0);
    if (ov44_0222E074() == 0) {
        IsNighttime();
        Sound_SetScene(0);
        Sound_SetSceneAndPlayBGM(11, 1063, 1);
        return;
    }
    u16 temp_r4 = GF_GetCurrentPlayingBGM();
    GF_GetCurrentPlayingBGM();
    GF_SetVolumeBySeqNo(temp_r4, GF_GetVolumeBySeqNo());
}

s32 ov44_0222E074(void) {
    u16 temp_r0 = GF_GetCurrentPlayingBGM();
    if ((temp_r0 != 1063) && (temp_r0 != 1063)) {
        return 0;
    }
    return 1;
}

s32 ov44_0222E090(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    s32 var_r6 = 0;
    s32 var_r7 = ListMenu_ProcessInput(arg0->unk158);
    if (ov44_0222C500(arg0) != 0) {
        var_r7 = -2;
    }
    s32 temp_r0 = ov44_02231C70(&arg0->unk38C);
    UnkStruct_ov44_02232B74* sp14 = ov44_0222AAEC(arg0, temp_r0);
    if (sub_020393C8() != 0) {
        sub_0200E5D4(&arg0->unk320, 0);
        RemoveWindow(&arg0->unk320);
        DestroyListMenu(arg0->unk158, 0, NULL);
        ListMenuItems_Delete(arg0->unk154);
        if (sp14 != 0) {
            ov44_02231F88(&arg0->unk38C, sp14);
        }
        ov44_0222B9A0(arg0);
        return arg1;
    }
    switch (var_r7) {
        case -1:
            if (sp14 == 0) {
                DestroyListMenu(arg0->unk158, 0, NULL);
                ov44_0222F780(arg0, temp_r0 - 1);
                ov44_0222F510(arg0, 16, 0);
                arg0->unk348 = 29;
                var_r6 = 1;
                break;
            }
            else {
                UnkStruct_ov44_02231958* temp_r0_2 = ov44_02231958(arg0, temp_r0 - 1);
                u32 r0 = ov44_02229F00(arg0, temp_r0_2);
                u8 r6 = temp_r0_2->unk21;
                if ((arg0->unk37E != r0) || ((ov44_02229F44(r0) == 0) && (arg0->unk380 != r6))) {
                    ov44_0222F780(arg0, temp_r0 - 1);
                    ov44_0222F510(arg0, 16, 0);
                    arg0->unk348 = 29;
                    var_r6 = 1;
                    break;
                }
                return arg1;
            }
        case -2:
            PlaySE(1501);
            arg0->unk348 = 19;
            break;
        default:
        PlaySE(1501);
        if (var_r7 == 1) {
            arg0->unk348 = 19;
            arg0->unk382 = temp_r0;
            if (temp_r0 == 0) {
                break;
            } else {
                s32 sp10 = ov44_0222A168(arg0);
                s32 spC = ov44_0222E02C(arg0);
                UnkStruct_ov44_02231958* sp8 = ov44_02231958(arg0, temp_r0 - 1);
                u32 temp_r7 = ov44_02229F00(arg0, sp8);
                s32 sp4 = sp8->unk21;
                if ((arg0->unk37E != temp_r7) || ((ov44_02229F44(temp_r7) == 0) && (arg0->unk380 != sp4))) {
                    ov44_0222F780(arg0, temp_r0 - 1);
                    ov44_0222F510(arg0, 16, 0);
                    arg0->unk348 = 29;
                    var_r6 = 1;
                } else if (ov44_0222DFEC(sp8) == 0) {
                    ov44_0222F780(arg0, temp_r0 - 1);
                    ov44_0222F510(arg0, 16, 0);
                    arg0->unk348 = 29;
                    var_r6 = 1;
                } else if ((ov44_0222A090(temp_r7) != 0) && (sp10 < 2)) {
                    if (temp_r7 == 15) {
                        ov44_0222F510(arg0, 89, 0);
                    } else {
                        ov44_0222F510(arg0, 103, 0);
                    }
                    arg0->unk348 = 29;
                    var_r6 = 1;
                } else if ((temp_r7 == 19) && (spC != 0)) {
                    if (spC == 1) {
                        ov44_0222F510(arg0, 104, 0);
                    } else if (spC == 3) {
                        ov44_0222F510(arg0, 106, 0);
                    } else {
                        ov44_0222F510(arg0, 105, 0);
                    }
                    arg0->unk348 = 29;
                    var_r6 = 1;
                } else if ((ov44_02229F44(temp_r7) == 0) && (sp4 != arg0->unk4->unk0.unk21)) {
                    if (sp4 != 0) {
                        ov44_0222F510(arg0, 134, 0);
                    } else {
                        ov44_0222F510(arg0, 135, 0);
                    }
                    ov44_0222F818(arg0, ov44_0222A0B4(temp_r7));
                    arg0->unk348 = 63;
                    var_r6 = 1;
                } else {
                    u32 temp_r0_3 = ov44_0222A0B4(temp_r7);
                    if (temp_r0_3 != 29) {
                        if (ov44_02231974(temp_r0 - 1) == 6) {
                            if (ov44_0223197C(arg0, temp_r0 - 1, temp_r0_3) != 0) {
                                arg0->unk340 = 1800;
                                ov44_0222A1B4(temp_r0_3);
                                ov44_0222F818(arg0, temp_r0_3);
                                ov44_0222F780(arg0, temp_r0 - 1);
                                ov44_0222F510(arg0, 17, 0);
                                GF_ASSERT(arg0->unk18C == NULL);
                                arg0->unk18C = WaitingIcon_New(&arg0->unk2D0, 482);
                                if (temp_r0_3 != 1) {
                                    arg0->unk348 = 45;
                                    
                                } else {
                                    arg0->unk340 = 1800;
                                    arg0->unk348 = 21;
                                }
                                var_r6 = 1;
                            } else {
                                ov44_0222F780(arg0, temp_r0 - 1);
                                ov44_0222F510(arg0, 16, 0);
                                arg0->unk348 = 29;
                                var_r6 = 1;
                            }
                        } else {
                            ov44_0222F780(arg0, temp_r0 - 1);
                            ov44_0222F510(arg0, 16, 0);
                            arg0->unk348 = 29;
                            var_r6 = 1;
                        }
                    }
                }
            }
            break;
        }
        if (var_r7 == 2) {
            arg0->unk348 = 54;
            break;
        }
    }
    if (var_r6 == 0) {
        ov44_0222F7BC(arg0);
    }
    sub_0200E5D4(&arg0->unk320, 0);
    RemoveWindow(&arg0->unk320);
    DestroyListMenu(arg0->unk158, 0, NULL);
    ListMenuItems_Delete(arg0->unk154);
    if (sp14 != 0) {
        ov44_02231F88(&arg0->unk38C, sp14);
    }
    return arg1;
}

s32 ov44_0222E45C(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    arg0->unk340 -= 1;
    if (arg0->unk340 < 0) {
        arg0->unk348 = 70;
    } else if (sub_020390C4() == 3) {
        ov44_0222F780(arg0, ov00_021E6EBC());
        ov44_0222F510(arg0, 18, 0);
        arg0->unk348 = 27;
    } else if (sub_020390C4() == 5) {
        ov44_0222F780(arg0, ov00_021E6EBC());
        ov44_0222F510(arg0, 15, 0);
        arg0->unk348 = 27;
    } else if ((sub_020390C4() == 4) || (sub_02039264() != 0)) {
        ov44_0222F780(arg0, ov00_021E6EBC());
        ov44_0222F510(arg0, 18, 0);
        arg0->unk348 = 27;
    } else if (sub_020393C8() != 0) {
        ov44_0222B9A0(arg0);
    } else if (sub_020390C4() == 1) {
        u32 temp_r6 = ov44_02229F00(arg0, &arg0->unk4->unk0);
        if (ov44_02229F44(temp_r6) == 0) {
            arg0->unk348 = 49;
            ov44_0222F7BC(arg0);
            sub_02034354(arg0->unk160, 0);
            arg0->unk35C = 30;
        } else if (sub_020373B4(0) == 1) {
            ov44_0222F7BC(arg0);
            sub_02034354(arg0->unk160, 0);
            sub_020398D4(0, 1);
            ov44_0222A1B4(temp_r6);
            arg0->unk34C = ov44_02229F74(temp_r6);
            sub_020378E4(0);
            BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, HEAP_ID_53);
            arg0->unk384 = 0;
            arg1 = 2;
        } else if (sub_02037880() == 1) {
            ov44_0222F780(arg0, ov00_021E6EBC());
            ov44_0222F510(arg0, 18, 0);
            arg0->unk348 = 27;
        }
    }
    return arg1;
}

s32 ov44_0222E5D8(s32 arg0, s32 arg1) {
    return arg1;
}

s32 ov44_0222E5DC(s32 arg0, s32 arg1) {
    return arg1;
}

s32 ov44_0222E5E0(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (sub_020393C8() != 0) {
        ov44_0222B9A0(arg0);
        return arg1;
    }
    ov44_0222F8F0(arg0);
    ov44_0222F818(arg0, 16);
    sub_02039358();
    arg0->unk370 = -1;
    arg0->unk348 = 19;
    ov44_022319EC(arg0);
    ov44_0222F7BC(arg0);
    return arg1;
}

s32 ov44_0222E62C(s32 arg0, s32 arg1) {
    return arg1;
}

s32 ov44_0222E630(s32 arg0, s32 arg1) {
    return arg1;
}

s32 ov44_0222E634(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    UnkStruct_ov44_02231958* temp_r7 = ov44_02231958(arg0, ov00_021E6EBC());
    u32 temp_r4 = ov44_02229F00(arg0, &arg0->unk4->unk0);
    u32 temp_r0 = ov44_02229F00(arg0, temp_r7);

    s32 r1 = arg0->unk4->unk0.unk21;
    if (temp_r4 == 12 && temp_r0 == 5) {
        arg0->unk348 = 49;
    }
    else if (temp_r4 == 13 && temp_r0 == 6) {
        arg0->unk348 = 49;
    }
    else if (temp_r4 == 14 && temp_r0 == 7) {
        arg0->unk348 = 49;
    }
    else if (temp_r4 == 9 && temp_r0 == 2) {
        arg0->unk348 = 49;
    }
    else if (temp_r4 == 10 && temp_r0 == 3) {
        arg0->unk348 = 49;
    }
    else if (temp_r4 == 11 && temp_r0 == 4) {
        arg0->unk348 = 49;
    }
    else if (temp_r4 == 15 && temp_r0 == 8) {
        arg0->unk348 = 49;
    }
    else if (temp_r4 == 19 && temp_r0 == 18) {
        arg0->unk348 = 49;
    }
    else if (temp_r4 == 21 && temp_r0 == 20) {
        arg0->unk348 = 49;
    }
    else if (temp_r4 == 23 && temp_r0 == 22) {
        arg0->unk348 = 49;
    }
    else if (temp_r4 == 25 && temp_r0 == 24) {
        arg0->unk348 = 49;
    }
    else if (temp_r4 == 27 && temp_r0 == 26) {
        arg0->unk348 = 49;
    }
    else if (temp_r4 == 16 && temp_r0 == 1 && r1 > 0) {
        arg0->unk348 = 22;
        return arg1;
    }
    else {
        if (sub_020393C8() != 0) {
            ov44_0222B9A0(arg0);
            return arg1;
        }
        ov44_0222F780(arg0, ov00_021E6EBC());
        ov44_0222F510(arg0, 18, 0);
        arg0->unk348 = 29;
        ov44_0222F818(arg0, 16);
        sub_02039358();
        return arg1;
    }
    ov44_0222F780(arg0, ov00_021E6EBC());
    ov44_0222F510(arg0, 11, 0);
    sub_02034354(arg0->unk160, 0);
    arg0->unk348 = 49;
    arg0->unk35C = 30;
    return arg1;
}

s32 ov44_0222E7C4(UnkStruct_ov44_022319EC* arg0) {
    if (sub_020390C4() >= 3) {
        ov44_0222F780(arg0, arg0->unk370);
        ov44_0222F510(arg0, 18, 0);
        arg0->unk348 = 27;
    }
    else if (sub_020393C8() != 0) {
        ov44_0222B9A0(arg0);
    }
    else {
        UnkStruct_ov44_02231958* temp_r6 = ov44_02231958(arg0, ov00_021E6EBC());
        u32 temp_r4 = ov44_02229F00(arg0, &arg0->unk4->unk0);
        u32 temp_r0 = ov44_02229F00(arg0, temp_r6);
        if ((temp_r4 != ov44_0222A0B4(temp_r0)) && (temp_r4 != temp_r0)) {
            ov44_0222F780(arg0, ov00_021E6EBC());
            ov44_0222F510(arg0, 16, 0);
            arg0->unk348 = 27;

        }
        else {
            return 0;
        }
    }
    arg0->unk384 = 0;
    return 1;
}

s32 ov44_0222E860(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if ((sub_020390C4() >= 4) || (sub_02039264() != 0) || (sub_020373B4(0) == 0)) {
        if (arg0->unk384 != 0) {
            ov44_0222F510(arg0, 101, 0);
        } else {
            ov44_0222F780(arg0, arg0->unk370);
            ov44_0222F510(arg0, 18, 0);
        }
        arg0->unk384 = 0;
        arg0->unk348 = 27;
    } else if (ov44_0222E7C4(arg0) == 0) {
        u32 temp_r1 = arg0->unk35C;
        if (temp_r1 == 0) {
            if (sub_02037B38(13) != 0) {
                sub_02037BEC();
                sub_02037AC0(14);
                arg0->unk384 = 0;
                arg0->unk348 = 50;
            }
        } else {
            arg0->unk35C = temp_r1 - 1;
            if (arg0->unk35C == 0) {
                sub_02037AC0(13);
            }
        }
    }
    return arg1;
}

s32 ov44_0222E908(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if ((ov44_0222E7C4(arg0) == 0) && (sub_02037B38(14) != 0)) {
        u16 r1 = ov44_02229F00(arg0, &arg0->unk4->unk0);
        if (sub_02037C0C(sub_0203769C(), &r1) != 0) {
            arg0->unk348 = 51;
        }
    }
    return arg1;
}

s32 ov44_0222E948(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    s32 var_r0;
    if (sub_0203769C() == 0) {
        var_r0 = 1;
    } else {
        var_r0 = 0;
    }
    u16* temp_r4 = sub_02037C44(var_r0);
    if ((ov44_0222E7C4(arg0) == 0) && (temp_r4 != NULL)) {
        u16 temp_r6 = ov44_02229F00(arg0, &arg0->unk4->unk0);
        u16 r0 = ov44_0222A0B4(temp_r6);
        if ((temp_r4[0] == r0) || (temp_r4[0] == temp_r6)) {
            sub_02037AC0(15);
            arg0->unk348 = 52;
        } else {
            ov44_0222F780(arg0, ov00_021E6EBC());
            ov44_0222F510(arg0, 16, 0);
            arg0->unk348 = 27;
        }
    }
    return arg1;
}

s32 ov44_0222E9C4(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if ((ov44_0222E7C4(arg0) == 0) && (sub_02037B38(15) != 0)) {
        sub_02034434();
        sub_020398D4(1, 1);
        sub_02037AC0(18);
        arg0->unk348 = 53;
    }
    return arg1;
}

s32 ov44_0222E9FC(UnkStruct_ov44_022319EC* arg0) {
    if (TextPrinterCheckActive(arg0->unk180) == 0) {
        return 1;
    }
    if (WindowIsInUse(&arg0->unk2D0) == 0) {
        return 1;
    }
    return 0;
}

s32 ov44_0222EA2C(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    ListMenuTemplate spC; // Unknown struct of size 0x20
    if ((ov44_0222E7C4(arg0) == 0) && (sub_02037B38(18) != 0) && (ov44_0222E9FC(arg0) == 1)) {
        sub_0203A1C4(arg0->unk160, &spC, HEAP_ID_53);
        ov44_0222F7BC(arg0);
        u32 temp_r0 = ov44_02229F00(arg0, ov44_02231958(arg0, ov00_021E6EBC()));
        u32 var_r1 = temp_r0;
        if ((var_r1 == 15) || (var_r1 == 8)) {
            var_r1 = 8;
            arg0->unk34C = 7;
        } else if (temp_r0 - 18 <= 1) {
            var_r1 = 18;
            arg0->unk34C = 11;
        } else if (var_r1 - 20 <= 1) {
            var_r1 = 20;
            arg0->unk34C = 12;
        } else if (var_r1 - 22 <= 1) {
            var_r1 = 22;
            arg0->unk34C = 13;
        } else if (var_r1 - 24 <= 1) {
            var_r1 = 24;
            arg0->unk34C = 14;
        } else if (var_r1 - 26 <= 1) {
            var_r1 = 26;
            arg0->unk34C = 15;
        } else if (var_r1 == 9 || var_r1 == 2) {
            var_r1 = 2;
            arg0->unk34C = 1;
        } else if (var_r1 == 10 || var_r1 == 3) {
            var_r1 = 3;
            arg0->unk34C = 2;
        } else if (var_r1 == 11 || var_r1 == 4) {
            var_r1 = 4;
            arg0->unk34C = 3;
        } else if (var_r1 == 12 || var_r1 == 5) {
            var_r1 = 5;
            arg0->unk34C = 4;
        } else if (var_r1 == 13 || var_r1 == 6) {
            var_r1 = 6;
            arg0->unk34C = 5;
        } else if (var_r1 == 14 || var_r1 == 7) {
            var_r1 = 7;
            arg0->unk34C = 6;
        } 
        ov44_0222F818(arg0, var_r1);
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, HEAP_ID_53);
        arg1 = 2;
    }
    return arg1;
}

s32 ov44_0222EB90(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    BgClearTilemapBufferAndCommit(arg0->unk15C, 3);
    arg0->unk364 = 0;
    arg0->unk348 = ov44_0222ADD0();
    return arg1;
}

s32 ov44_0222EBB8(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    arg0->unk348 = 55;
    return arg1;
}

s32 ov44_0222EBC4(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->unk180) != 0) {
        return arg1;
    }
    if (ov44_0222C4EC(arg0) != 0) {
        arg0->unk348 = 56;
    }
    if ((3 & gSystem.newKeys) || (System_GetTouchNew() == 1)) {
        arg0->unk348 = 56;
    }
    return arg1;
}

s32 ov44_0222EC14(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    ov44_0222F7BC(arg0);
    arg0->unk348 = 19;
    return arg1;
}

s32 ov44_0222EC2C(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->unk180) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            arg0->unk184 = Std_CreateYesNoMenu(arg0->unk15C, &ov44_0223538C, 473, 11, HEAP_ID_53);
        } else {
            arg0->unk188 = ov44_02231A14(arg0->unk15C, &ov44_02235374, 0);
        }
        arg0->unk348 = 58;
    }
    return arg1;
}

s32 ov44_0222EC98(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
        s32 var_r5 = Handle2dMenuInput_DeleteOnFinish(arg0->unk184, HEAP_ID_53);
        if (TextPrinterCheckActive(arg0->unk180) != 0) {
            return arg1;
        }
        if (ov44_0222C500(arg0) != 0) {
            if (var_r5 == -1) {
                Clear2dMenuWindowAndDelete(arg0->unk184, HEAP_ID_53);
            }
            var_r5 = -2;
        }
        if (var_r5 == -1) {
            return arg1;
        }
        if (var_r5 == 0) {
            ov44_0222F510(arg0, 26, 1);
            arg0->unk348 = 59;
            arg0->unk35C = 1;
        } else {
            ov44_0222F7BC(arg0);
            arg0->unk348 = 19;
            ov44_022319EC(arg0);
        }
    }
    else {
        YesNoResponse var_r5_2 = YesNoPrompt_HandleInput(arg0->unk188);
        if (TextPrinterCheckActive(arg0->unk180) != 0) {
            return arg1;
        }
        if (ov44_0222C500(arg0) != 0) {
            var_r5_2 = YESNORESPONSE_NO;
        }
        if (var_r5_2 == YESNORESPONSE_YES) {
            YesNoPrompt_Destroy(arg0->unk188);
            ov44_0222F510(arg0, 26, 1);
            arg0->unk348 = 59;
            arg0->unk35C = 1;
        }
        else if (var_r5_2 == YESNORESPONSE_NO) {
            YesNoPrompt_Destroy(arg0->unk188);
            ov44_0222F7BC(arg0);
            arg0->unk348 = 19;
            ov44_022319EC(arg0);
        }
        else {
            return arg1;
        }
    }
    return arg1;
}

s32 ov44_0222EDB8(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->unk180) != 0) {
        return arg1;
    }
    if (arg0->unk35C == 1) {
        arg0->unk35C = 0;
        sub_02039330();
    }
    if (sub_02037D78() == 0) {
        sub_0202C46C(arg0->unk0);
        ov44_0222F510(arg0, 27, 1);
        arg0->unk348 = 60;
        arg0->unk35C = 30;
    }
    return arg1;
}

s32 ov44_0222EE10(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->unk180) != 0) {
        return arg1;
    }
    arg0->unk35C--;
    if (arg0->unk35C == 0) {
        arg0->unk34C = 8;
        arg0->unk348 = 34;
        ov44_0222F7BC(arg0);
    }
    return arg1;
}

s32 ov44_0222EE54(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (sub_02037B38(16) == 0) {
        return arg1;
    }
    sub_020398D4(0, 1);
    if (TextPrinterCheckActive(arg0->unk180) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            arg0->unk184 = Std_CreateYesNoMenu(arg0->unk15C, &ov44_0223538C, 473, 11, HEAP_ID_53);
        } else {
            arg0->unk188 = ov44_02231A28(arg0->unk15C, &ov44_02235374, 0);
        }
        arg0->unk348 = 62;
    }
    return arg1;
}

s32 ov44_0222EED4(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (TextPrinterCheckActive(arg0->unk180) != 0) {
        return arg1;
    }
    if ((sub_02039274() != 0) || (sub_02039264() != 0) || (sub_020390C4() >= 3)) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            Clear2dMenuWindowAndDelete(arg0->unk184, HEAP_ID_53);
        } else {
            YesNoPrompt_Destroy(arg0->unk188);
        }
        ov44_0222F510(arg0, 101, 0);
        arg0->unk348 = 28;
    }
    else {
        if (sub_020393C8() != 0) {
            if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
                Clear2dMenuWindowAndDelete(arg0->unk184, HEAP_ID_53);
            } else {
                YesNoPrompt_Destroy(arg0->unk188);
            }
            ov44_0222B9A0(arg0);
        }
        else {
            if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
                s32 temp_r0 = Handle2dMenuInput_DeleteOnFinish(arg0->unk184, HEAP_ID_53);
                if (temp_r0 == -1) {
                    return arg1;
                }
                else if (temp_r0 == 0) {
                    ov44_0222F510(arg0, 138, 0);
                    GF_ASSERT(arg0->unk18C == 0);
                    arg0->unk18C = WaitingIcon_New(&arg0->unk2D0, 482);
                    arg0->unk384 = 1;
                    arg0->unk348 = 49;
                    arg0->unk35C = 30;
                } else {
                    ov44_0222F7BC(arg0);
                    sub_020343E4();
                    sub_020393B4();
                    arg0->unk35C = 20;
                    arg0->unk348 = 65;
                }
            }
            else {
                s32 temp_r0_2 = YesNoPrompt_HandleInput(arg0->unk188);
                if (temp_r0_2 == 1) {
                    YesNoPrompt_Destroy(arg0->unk188);
                    ov44_0222F510(arg0, 138, 0);
                    GF_ASSERT(arg0->unk18C == 0);
                    arg0->unk18C = WaitingIcon_New(&arg0->unk2D0, 482);
                    arg0->unk384 = 1;
                    arg0->unk348 = 49;
                    arg0->unk35C = 30;
                }
                else if (temp_r0_2 == 2) {
                    YesNoPrompt_Destroy(arg0->unk188);
                    ov44_0222F7BC(arg0);
                    sub_020343E4();
                    sub_020393B4();
                    arg0->unk35C = 20;
                    arg0->unk348 = 65;
                }
                else {
                    return arg1;
                }
            }
            if (arg0->unk384 == 0) {
                arg0->unk4 = (UnkStruct_ov44_0223197C*)sub_020398C8(); //TODO: Update overlay_44_02235340
                arg0->unk4->unk0.unk21 = arg0->unk4->unk0.unk22;
            }
        }
    }
    return arg1;
}

s32 ov44_0222F0AC(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    u32 temp_r0 = ov44_02231C70(&arg0->unk38C);
    if (ov44_0222AAEC(arg0, temp_r0) == 0) {
        ov44_0222F780(arg0, temp_r0 - 1);
        ov44_0222F510(arg0, 16, 0);
        arg0->unk348 = 29;
        return arg1;
    }
    UnkStruct_ov44_02231958* temp_r0_2 = ov44_02231958(arg0, temp_r0 - 1);
    u32 r0 = ov44_02229F00(arg0, temp_r0_2);
    u8 r7 = temp_r0_2->unk21;
    if ((arg0->unk37E != r0) || (arg0->unk380 != r7)) {
        ov44_0222F780(arg0, temp_r0 - 1);
        ov44_0222F510(arg0, 16, 0);
        arg0->unk348 = 29;
        return arg1;
    }
    if (TextPrinterCheckActive(arg0->unk180) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            arg0->unk184 = Std_CreateYesNoMenu(arg0->unk15C, &ov44_0223538C, 473, 11, HEAP_ID_53);
        } else {
            arg0->unk188 = ov44_02231A14(arg0->unk15C, &ov44_02235374, 0);
        }
        arg0->unk348 = 64;
    }
    return arg1;
}

s32 ov44_0222F194(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    s32 temp_r0 = ov44_02231C70(&arg0->unk38C);
    if (ov44_0222AAEC(arg0, temp_r0) == 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            Clear2dMenuWindowAndDelete(arg0->unk184, HEAP_ID_53);
        } else {
            YesNoPrompt_Destroy(arg0->unk188);
        }
        ov44_0222F780(arg0, temp_r0 - 1);
        ov44_0222F510(arg0, 16, 0);
        arg0->unk348 = 29;
        return arg1;
    }
    UnkStruct_ov44_02231958* temp_r0_2 = ov44_02231958(arg0, temp_r0 - 1);
    u32 r0 = ov44_02229F00(arg0, temp_r0_2);
    u8 r7 = temp_r0_2->unk21;
    if ((arg0->unk37E != r0) || (arg0->unk380 != r7)) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            Clear2dMenuWindowAndDelete(arg0->unk184, HEAP_ID_53);
        } else {
            YesNoPrompt_Destroy(arg0->unk188);
        }
        ov44_0222F780(arg0, temp_r0 - 1);
        ov44_0222F510(arg0, 16, 0);
        arg0->unk348 = 29;
        return arg1;
    }
    if (sub_020393C8() != 0) {
        if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            Clear2dMenuWindowAndDelete(arg0->unk184, HEAP_ID_53);
        } else {
            YesNoPrompt_Destroy(arg0->unk188);
        }
        ov44_0222B9A0(arg0);
    }
    else {
        if (sub_020390C4() >= 3) {
            if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
                Clear2dMenuWindowAndDelete(arg0->unk184, HEAP_ID_53);
            } else {
                YesNoPrompt_Destroy(arg0->unk188);
            }
            ov44_0222F510(arg0, 18, 0);
            sub_02039358();
            arg0->unk348 = 29;
            return arg1;
        }
        else if ((REG_POWCNT_ADDR / 2048 & reg_GX_POWCNT) >> 15 == 1) {
            s32 menuInputResult = Handle2dMenuInput_DeleteOnFinish(arg0->unk184, HEAP_ID_53);
            if (menuInputResult == -1) {
                return arg1;
            }
            if (menuInputResult == 0) {
                u32 temp_r0 = ov44_0222A0B4(arg0->unk37E);
                if (temp_r0 != 29) {
                    if (ov44_02231974(arg0->unk382 - 1) == 6) {
                        ov44_0222F89C(arg0);
                        if (ov44_0223197C(arg0, arg0->unk382 - 1, temp_r0) != 0) {
                            arg0->unk340 = 1800;
                            ov44_0222A1B4(temp_r0);
                            sub_020378E4(0);
                            ov44_0222F818(arg0, temp_r0);
                            ov44_0222F780(arg0, arg0->unk382 - 1);
                            ov44_0222F510(arg0, 17, 0);
                            GF_ASSERT(arg0->unk18C == 0);
                            arg0->unk18C = WaitingIcon_New(&arg0->unk2D0, 482);
                            if (temp_r0 != 1) {
                                arg0->unk348 = 45;
                                return arg1;
                            }
                            arg0->unk340 = 1800;
                            arg0->unk348 = 21;
                            return arg1;
                        }
                    }
                }
            }
            ov44_0222F7BC(arg0);
            ov44_0222F818(arg0, 16);
            ov44_0222F8F0(arg0);
            ov44_0222C120(arg0);
            arg0->unk348 = 19;
            return arg1;
        }
        YesNoResponse yesNoResponse = YesNoPrompt_HandleInput(arg0->unk188);
        if (yesNoResponse == 1) {
            YesNoPrompt_Destroy(arg0->unk188);
            u32 temp_r0 = ov44_0222A0B4(arg0->unk37E);
            if (temp_r0 != 29) {
                if (ov44_02231974(arg0->unk382 - 1) == 6) {
                    ov44_0222F89C(arg0);
                    if (ov44_0223197C(arg0, arg0->unk382 - 1, temp_r0) != 0) {
                        arg0->unk340 = 1800;
                        ov44_0222A1B4(temp_r0);
                        sub_020378E4(0);
                        ov44_0222F818(arg0, temp_r0);
                        ov44_0222F780(arg0, arg0->unk382 - 1);
                        ov44_0222F510(arg0, 17, 0);
                        GF_ASSERT(arg0->unk18C == 0);
                        arg0->unk18C = WaitingIcon_New(&arg0->unk2D0, 482);
                        if (temp_r0 != 1) {
                            arg0->unk348 = 45;
                            return arg1;
                        }
                        arg0->unk340 = 1800;
                        arg0->unk348 = 21;
                        return arg1;
                    }
                }
            }
        }
        else if (yesNoResponse == 2) {
            YesNoPrompt_Destroy(arg0->unk188);
            ov44_0222F7BC(arg0);
            ov44_0222F818(arg0, 16);
            ov44_0222F8F0(arg0);
            ov44_0222C120(arg0);
            arg0->unk348 = 19;
            return arg1;
        }
        else {
            return arg1;
        }
    }
    return arg1;
}

s32 ov44_0222F4E0(s32 arg0, s32 arg1) {
    if (sub_02037D78() == 0) {
        BeginNormalPaletteFade(0, 0, 0, 0, 6, 1, HEAP_ID_53);
        arg1 = 2;
    }
    return arg1;
}

void ov44_0222F510(UnkStruct_ov44_022319EC* arg0, s32 arg1, s32 arg2) {
    u32 textSpeed_int;
    if (arg2 != 0) {
        textSpeed_int = 1;
    } else {
        Options* options = Save_PlayerData_GetOptionsAddr(arg0->unk160);
        textSpeed_int = Options_GetTextFrameDelay(options);
    }
    u8 textSpeed = textSpeed_int;
    ov44_0222F910(arg0);
    if (WindowIsInUse(&arg0->unk300) != 0) {
        ClearFrameAndWindow2(&arg0->unk300, 0);
        RemoveWindow(&arg0->unk300);
    }
    if (WindowIsInUse(&arg0->unk2D0) != 0) {
        ClearFrameAndWindow2(&arg0->unk2D0, 0);
        RemoveWindow(&arg0->unk2D0);
    }
    s32 temp_r0 = arg0->unk180;
    if ((temp_r0 != 8) && (TextPrinterCheckActive(temp_r0) != 0)) {
        RemoveTextPrinter(arg0->unk180);
        arg0->unk180 = 8;
    }
    AddWindowParameterized(arg0->unk15C, &arg0->unk2D0, 2, 2, 19, 27, 4, 12, 301);
    if (arg2 != 0) {
        ReadMsgDataIntoString(arg0->unk16C, arg1, arg0->unk170);
    } else {
        ReadMsgDataIntoString(arg0->unk168, arg1, arg0->unk170);
    }
    StringExpandPlaceholders(arg0->unk164, arg0->unk174, arg0->unk170);
    FillWindowPixelBuffer(&arg0->unk2D0, 15);
    DrawFrameAndWindow2(&arg0->unk2D0, 1, 482, 10);
    TextFlags_SetCanABSpeedUpPrint(1);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    arg0->unk180 = AddTextPrinterParameterized(&arg0->unk2D0, 1, arg0->unk174, 0, 0, textSpeed, 0);
    ScheduleWindowCopyToVram(&arg0->unk2D0);
}

void ov44_0222F66C(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    ov44_0222F910(arg0);
    if (WindowIsInUse(&arg0->unk300) != 0) {
        ClearFrameAndWindow2(&arg0->unk300, 0);
        RemoveWindow(&arg0->unk300);
    }
    if (WindowIsInUse(&arg0->unk2D0) != 0) {
        ClearFrameAndWindow2(&arg0->unk2D0, 0);
        RemoveWindow(&arg0->unk2D0);
    }
    s32 temp_r0 = arg0->unk180;
    if ((temp_r0 != 8) && (TextPrinterCheckActive(temp_r0) != 0)) {
        RemoveTextPrinter(arg0->unk180);
        arg0->unk180 = 8;
    }
    AddWindowParameterized(arg0->unk15C, &arg0->unk300, 2, 4, 4, 23, 16, 12, 105);
    ReadMsgDataIntoString(arg0->unk16C, arg1, arg0->unk170);
    StringExpandPlaceholders(arg0->unk164, arg0->unk174, arg0->unk170);
    FillWindowPixelBuffer(&arg0->unk300, 15);
    DrawFrameAndWindow1(&arg0->unk300, 1, 473, 11);
    arg0->unk180 = AddTextPrinterParameterized(&arg0->unk300, 1, arg0->unk174, 0, 0, 255, 0);
    ScheduleWindowCopyToVram(&arg0->unk300);
}

void ov44_0222F780(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (arg1 != -1) {
        PlayerProfile* playerProfile = PlayerProfile_New(HEAP_ID_53);
        Save_Profile_PlayerName_Set(playerProfile, sub_0202C254(arg0->unk0, arg1));
        BufferPlayersName(arg0->unk164, 0, playerProfile);
        Heap_Free(playerProfile);
    }
}

void ov44_0222F7BC(UnkStruct_ov44_022319EC* arg0) {
    s32 temp_r0;

    ov44_0222F910(arg0);
    temp_r0 = arg0->unk180;
    if ((temp_r0 != 8) && (TextPrinterCheckActive(temp_r0) != 0)) {
        RemoveTextPrinter(arg0->unk180);
        arg0->unk180 = 8;
    }
    if (WindowIsInUse(&arg0->unk2D0) != 0) {
        ClearFrameAndWindow2(&arg0->unk2D0, 0);
        RemoveWindow(&arg0->unk2D0);
    }
}

void ov44_0222F818(UnkStruct_ov44_022319EC* arg0, u32 arg1) {
    if (arg0->unk4 != NULL) {
        ov44_0222F830(arg0, arg1);
        ov00_021E6D60(arg0->unk4, 36);
    }
}

void ov44_0222F830(UnkStruct_ov44_022319EC* arg0, u32 arg1) {
    if (arg0->unk4 != NULL) {
        u32 r0 = ov44_02229F00(arg0, &arg0->unk4->unk0);
        if (r0 != arg1) {
            arg0->unk4->unk0.unk1B = arg1;
            if ((ov44_0222A020(arg1) == 0) && (arg1 != 8) && (arg1 != 20) && (arg1 != 22) && (arg1 != 24) && (arg1 != 26) && (arg1 != 18)) {
                if (arg1 == 1) {
                    GF_SndStartFadeOutBGM(0, 30);
                } else if (arg1 == 16) {
                    ov00_021E70B8(0);
                    if (ov44_0222E074() == 1) {
                        GF_SndStartFadeInBGM(120, 30, 1);
                    }
                }
            }
        }
        ov44_0222C120(arg0);
    }
}

u8 ov44_0222F89C(UnkStruct_ov44_022319EC* arg0) {
    arg0->unk4->unk0.unk21 = 1 - arg0->unk4->unk0.unk21;
    ov00_021E6D60(arg0->unk4, 36);
    return arg0->unk4->unk0.unk21;
}

u8 ov44_0222F8C0(UnkStruct_ov44_022319EC* arg0) {
    arg0->unk4->unk0.unk22 = 1 - arg0->unk4->unk0.unk22;
    arg0->unk4->unk0.unk21 = arg0->unk4->unk0.unk22;
    ov00_021E6D60(arg0->unk4, 36);
    return arg0->unk4->unk0.unk22;
}

u8 ov44_0222F8F0(UnkStruct_ov44_022319EC* arg0) {
    arg0->unk4->unk0.unk21 = arg0->unk4->unk0.unk22;
    ov00_021E6D60(arg0->unk4, 36);
    return arg0->unk4->unk0.unk22;
}

void ov44_0222F910(UnkStruct_ov44_022319EC* arg0) {
    if (arg0->unk18C != 0) {
        sub_0200F478();
        arg0->unk18C = 0;
        if (WindowIsInUse(&arg0->unk2D0) != 0) {
            ClearFrameAndWindow2(&arg0->unk2D0, 0);
            RemoveWindow(&arg0->unk2D0);
        }
    }
}

void ov44_0222F950(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    ov44_0222FC0C(arg0);
    ov44_02231D8C(&arg0->unk38C);
    ov44_02230060(arg0, arg1);
}

void ov44_0222F970(UnkStruct_ov44_022319EC* arg0, s32 arg1, s32 arg2) {
    if (arg1 == ov44_0222FC00(arg0)) {
        ov44_0222F950(arg0, arg2);
    }
}

s32 ov44_0222F98C(UnkStruct_ov44_022319EC* arg0) {
    if (arg0->unkB1C.unk4C != 0) {
        return 1;
    }
    return 0;
}

void ov44_0222F9A0(UnkStruct_ov44_022319EC* arg0, NARC* arg1, enum HeapID arg2) {
    memset(&arg0->unkB1C, 0, sizeof(UnkStruct_ov44_02231800));
    GfGfx_EngineBTogglePlanes(1, 1);
    GfGfx_EngineBTogglePlanes(2, 1);
    GfGfx_EngineBTogglePlanes(4, 1);
    GfGfx_EngineBTogglePlanes(8, 0);
    arg0->unkB1C.unk4C = TouchHitboxController_Create(ov44_022354E8, 8, (TouchHitboxControllerCallback)ov44_0222FC84, arg0, arg2);
    arg0->unkB1C.unk0 = MessageFormat_New(arg2);
    ov44_0222FCBC(arg0, arg1, arg2);
    arg0->unkB1C.unk78 = 1;
    ov44_02230090(arg0);
    ov44_022300C8(arg0);
}

void ov44_0222FA28(UnkStruct_ov44_022319EC* arg0) {
    if (ov44_0222F98C(arg0) != 0) {
        ov44_0222FF30(arg0);
        TouchHitboxController_Destroy(arg0->unkB1C.unk4C);
        arg0->unkB1C.unk4C = NULL;
        MessageFormat_Delete(arg0->unkB1C.unk0);
        arg0->unkB1C.unk0 = NULL;
        GfGfx_EngineBTogglePlanes(1, 1);
        GfGfx_EngineBTogglePlanes(2, 0);
        GfGfx_EngineBTogglePlanes(4, 0);
        GfGfx_EngineBTogglePlanes(8, 0);
    }
}

u8 ov44_0222FA80(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    u32 var_r5 = ov44_02231C8C(&arg0->unk38C);
    u32 temp_r0 = ov44_02231CE8(&arg0->unk38C);
    if ((temp_r0 >= 3) && (temp_r0 <= 6)) {
        var_r5 = temp_r0;
    }
    if ((var_r5 >= 3) && (var_r5 <= 6)) {
        if (arg0->unkB1C.unk70 != var_r5 - 3) {
            arg0->unkB1C.unk70 = var_r5 - 3;
            PlaySE(1505);
            ov44_02230090(arg0);
            arg0->unkB1C.unk75 = 1;
            arg0->unkB1C.unk78 = 1;
        }
    }
    if (arg0->unkB1C.unk73 == 1) {
        arg0->unkB1C.unk73 = 2;
        arg0->unkB1C.unk74 = 0;
        ov44_02230234(arg0, arg1);
        ov44_02231720(arg0);
    }
    if (arg0->unkB1C.unk73 == 0) {
        TouchHitboxController_IsTriggered(arg0->unkB1C.unk4C);
        ov44_022313C8(arg0);
        if (arg0->unkB1C.unk75 == 1) {
            ov44_022300C8(arg0);
            arg0->unkB1C.unk75 = 0;
        }
    }
    if ((u8) (arg0->unkB1C.unk73 + 254) <= 1) {
        u32 temp_r0_2 = ov44_02231788(arg0);
        if (ov44_0222FBC8(arg0, temp_r0_2) == 1) {
            arg0->unkB1C.unk77 = arg0->unkB1C.unk71;
            arg0->unkB1C.unk71 = 0;
            arg0->unkB1C.unk72 = 0;
            arg0->unkB1C.unk75 = 1;
            arg0->unkB1C.unk78 = 1;
            arg0->unkB1C.unk73 = 0;
            ov44_02231754(arg0);
            PlaySE(1501);
        } else if (temp_r0_2 != 0) {
            PlaySE(1501);
            ov44_0222FC3C(arg0, temp_r0_2);
            ov44_02230234(arg0, arg1);
        }
    }
    return arg0->unkB1C.unk71;
}

void ov44_0222FBA0(UnkStruct_ov44_022319EC* arg0) {
    if ((arg0->unkB1C.unk73 == 0) && (arg0->unkB1C.unk75 == 1)) {
        ov44_022300C8(arg0);
        arg0->unkB1C.unk75 = 0;
    }
}

s32 ov44_0222FBC8(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (arg0->unkB1C.unk73 == 3) {
        return 0;
    }
    if ((240 & gSystem.heldKeys) || (1027 & gSystem.newKeys) || (arg1 == 2)) {
        return 1;
    }
    return 0;
}

u8 ov44_0222FC00(UnkStruct_ov44_022319EC* arg0) {
    return arg0->unkB1C.unk71;
}

void ov44_0222FC0C(UnkStruct_ov44_022319EC* arg0) {
    if (ov44_0222F98C(arg0) == 1) {
        arg0->unkB1C.unk71 = 0;
        arg0->unkB1C.unk72 = 0;
        arg0->unkB1C.unk73 = 0;
        arg0->unkB1C.unk75 = 1;
        arg0->unkB1C.unk78 = 1;
        ov44_02231754(arg0);
    }
}

void ov44_0222FC3C(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    u32 var_r1;
    if (ov44_02229EE0(arg0) == 1) {
        var_r1 = 7;
    } else {
        var_r1 = 2;
    }
    if (arg1 == 3) {
        arg0->unkB1C.unk74 = (arg0->unkB1C.unk74 + 1) % var_r1;
        return;
    }
    s8* ptr = &arg0->unkB1C.unk74;
    ptr[0]--;
    if (arg0->unkB1C.unk74 < 0) {
        ptr[0] += var_r1;
    }
}

void ov44_0222FC84(s32 arg0, enum HeapID arg1, UnkStruct_ov44_022319EC* arg2) {
    s32 temp_r0 = arg2->unkB1C.unk70 * 8 + arg0;
    if ((arg2->unkB1C.unk50[temp_r0] == 2) && (arg2->unkB1C.unk71 == 0) && (arg1 == 0)) {
        arg2->unkB1C.unk71 = temp_r0 + 1;
        PlaySE(1501);
    }
}

void ov44_0222FCBC(UnkStruct_ov44_022319EC* arg0, NARC* arg1, enum HeapID arg2) {
    s32 i;
    s32 j;
    BgClearTilemapBufferAndCommit(arg0->unk15C, 4);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg1, 17, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 128, arg2);
    GfGfxLoader_GXLoadPalFromOpenNarc(arg1, 20, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_4_OFFSET, 160, arg2);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg1, 16, arg0->unk15C, GF_BG_LYR_SUB_0, 0, 0, 0, arg2);
    GfGfxLoader_LoadCharDataFromOpenNarc(arg1, 19, arg0->unk15C, GF_BG_LYR_SUB_2, 48, 0, 0, arg2);
    GfGfxLoader_LoadScrnDataFromOpenNarc(arg1, 18, arg0->unk15C, GF_BG_LYR_SUB_0, 0, 0, 0, arg2);
    arg0->unkB1C.unk4 = GfGfxLoader_GetScrnDataFromOpenNarc(arg1, 21, 0, &arg0->unkB1C.unk8, arg2);
    ov44_0222FFB4(arg0->unkB1C.unk8, 48);
    
    for (i = 0; i < 7; i++) {
        arg0->unkB1C.unkC[i] = GfGfxLoader_GetScrnDataFromOpenNarc(arg1, i + 22, 0, &arg0->unkB1C.unk28[i], arg2);
        ov44_0222FFB4(arg0->unkB1C.unk28[i], 48);
    }
    
    arg0->unkB1C.unk44 = GfGfxLoader_GetScrnDataFromOpenNarc(arg1, 29, 0, &arg0->unkB1C.unk48, arg2);
    ov44_0222FFB4(arg0->unkB1C.unk48, 48);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, arg2);

    s32 sp20;
    s32 r5;
    for (i = 0; i < 8; i++) {
        InitWindow(&arg0->unkB1C.unk7C[i]);
        r5 = i / 4;
        sp20 = i % 4;

        AddWindowParameterized(arg0->unk15C, &arg0->unkB1C.unk7C[i], 5, r5 * 16 + 4, sp20 * 6 + 1, 9, 3, 15, 1 + (i * 27));
        FillWindowPixelBuffer(&arg0->unkB1C.unk7C[i], 0);
        ScheduleWindowCopyToVram(&arg0->unkB1C.unk7C[i]);
        
        for (j = 0; j < 2; j++) {
            InitWindow(&arg0->unkB1C.unkFC[i][j]);
            AddWindowParameterized(arg0->unk15C, &arg0->unkB1C.unkFC[i][j], 5, r5 * 16 + 1 + j * 12, sp20 * 6 + 1, 2, 3, 13, 6 * (j + i*2) + 218);
            FillWindowPixelBuffer(&arg0->unkB1C.unkFC[i][j], 0);
            ScheduleWindowCopyToVram(&arg0->unkB1C.unkFC[i][j]);
        }
    }
    AddWindowParameterized(arg0->unk15C, &arg0->unkB1C.unk1FC, 7, 1, 1, 30, 21, 15, 1);
    FillWindowPixelBuffer(&arg0->unkB1C.unk1FC, 0);
    ScheduleWindowCopyToVram(&arg0->unkB1C.unk1FC);
    ov44_02231420(arg0, arg1, arg2);
}

void ov44_0222FF30(UnkStruct_ov44_022319EC* arg0) {
    s32 i;
    ov44_022316B0(arg0);
    for (i = 0; i < 8; i++) {
        RemoveWindow(&arg0->unkB1C.unk7C[i]);
        for (s32 j = 0; j < 2; j++) {
            RemoveWindow(&arg0->unkB1C.unkFC[i][j]);
        }
    }
    RemoveWindow(&arg0->unkB1C.unk1FC);
    Heap_Free(arg0->unkB1C.unk4);
    for (i = 0; i < 7; i++) {
        Heap_Free(arg0->unkB1C.unkC[i]);
    }
    Heap_Free(arg0->unkB1C.unk44);
}

void ov44_0222FFB4(NNSG2dScreenData* arg0, s32 arg1) {
    s32 i;
    s32 j;
    u16* ip = (u16*)&arg0->rawData[0];
    s32 width = arg0->screenWidth / 8;
    s32 height = arg0->screenHeight / 8;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            ip[i * width + j] += arg1;
        }
    }
}

void ov44_0222FFF4(UnkStruct_ov44_022319EC* arg0, s32 arg1, u32 arg2) {
    GF_ASSERT(arg1 != 0);
    GF_ASSERT(arg2 != 0);
    GF_ASSERT(arg2 < 3);
    arg0->unkB1C.unk50[arg1 - 1] = arg2;
    arg0->unkB1C.unk75 = 1;
}

void ov44_02230028(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    GF_ASSERT(arg1 != 0);
    arg0->unkB1C.unk50[arg1 - 1] = 0;
    arg0->unkB1C.unk75 = 1;
}

u8 ov44_02230048(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    GF_ASSERT(arg1 != 0);
    return arg0->unkB1C.unk50[arg1 - 1];
}

void ov44_02230060(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    if (arg0->unkB1C.unk73 != 0) {
        ov44_02230234(arg0, arg1);
        return;
    }
    arg0->unkB1C.unk78 = 1;
    ov44_022300C8(arg0);
}

void ov44_02230080(UnkStruct_ov44_022319EC* arg0) {
    arg0->unkB1C.unk78 = 1;
    arg0->unkB1C.unk75 = 1;
}

void ov44_02230090(UnkStruct_ov44_022319EC* arg0) {
    BgTilemapRectChangePalette(arg0->unk15C, 4, 0, 0, 32, 24, arg0->unkB1C.unk70);
    ScheduleBgTilemapBufferTransfer(arg0->unk15C, 4);
}

void ov44_022300C8(UnkStruct_ov44_022319EC* arg0) {
    s32 sp1C;
    s32 sp18;
    if (arg0->unkB1C.unk78 == 1) {
        arg0->unkB1C.unk78 = 0;
        sp18 = 1;
        FillBgTilemapRect(arg0->unk15C, 6, 0, 0, 0, 32, 24, 0);
    } else {
        sp18 = 0;
    }
    for (s32 i = 0; i < 8; i++) {
        s32 temp_r4 = i + (arg0->unkB1C.unk70 * 8) + 1;
        if (arg0->unkB1C.unk50[temp_r4 - 1] != 0) {
            if (sub_0202C090(arg0->unk0, temp_r4 - 1, 8) == 0) {
                sp1C = 1;
            } else {
                sp1C = 0;
            }
        } else {
            sp1C = 2;
        }
        s32 r2 = i / 4;
        s32 temp_ip = i % 4;
        u32 var_r5;
        if (temp_r4 == arg0->unkB1C.unk71) {
            var_r5 = arg0->unkB1C.unk72;
        } else {
            var_r5 = 0;
        }
        s32 var_r1;
        if ((temp_r4 == arg0->unkB1C.unk71) || (temp_r4 == arg0->unkB1C.unk77)) {
            var_r1 = 1;
        } else {
            var_r1 = 0;
        }
        if (sp18 == 1 || var_r1 == 1) {
            ov44_02231148(arg0,  r2 << 4,  (6 * temp_ip), sp1C, var_r5);
            if (sp1C != 2) {
                ov44_022312B8(arg0, &arg0->unkB1C.unk7C[i], temp_r4, var_r5, i);
                ov44_02231344(arg0, &arg0->unkB1C.unkFC[i][0], temp_r4, var_r5, i);
            } else {
                FillWindowPixelBuffer(&arg0->unkB1C.unk7C[i], 0);
                ScheduleWindowCopyToVram(&arg0->unkB1C.unk7C[i]);
                for (s32 j = 0; j < 2; j++) {
                    FillWindowPixelBuffer(&arg0->unkB1C.unkFC[i][j], 0);
                    ScheduleWindowCopyToVram(&arg0->unkB1C.unkFC[i][j]);
                }
            }
        }
    }
    ScheduleBgTilemapBufferTransfer(arg0->unk15C, 6);
    GfGfx_EngineBTogglePlanes(2, 1);
    GfGfx_EngineBTogglePlanes(8, 0);
}

void ov44_02230234(UnkStruct_ov44_022319EC* arg0, s32 arg1) {
    s32 var_r4;
    if (sub_0202C090(arg0->unk0, arg0->unkB1C.unk71 - 1, 8) == 1) {
        var_r4 = 7;
    } else {
        var_r4 = 8;
    }
    NNSG2dScreenData* temp_r3 = arg0->unkB1C.unk28[arg0->unkB1C.unk74];
    BG_LoadScreenTilemapData(arg0->unk15C, 6, &temp_r3->rawData[0], temp_r3->szByte);
    BgTilemapRectChangePalette(arg0->unk15C, 6, 0, 0, 32, 24, var_r4);
    BgClearTilemapBufferAndCommit(arg0->unk15C, 7);
    if (ov44_02229EE0(arg0) == 0) {
        ov44_02230FE8(arg0);
    }
    FillWindowPixelBuffer(&arg0->unkB1C.unk1FC, 0);
    ov44_02236680[arg0->unkB1C.unk74](arg0, arg1);
    ScheduleBgTilemapBufferTransfer(arg0->unk15C, 6);
    ScheduleWindowCopyToVram(&arg0->unkB1C.unk1FC);
    GfGfx_EngineBTogglePlanes(2, 0);
    GfGfx_EngineBTogglePlanes(8, 1);
}

void ov44_02230300(UnkStruct_ov44_022319EC* arg0) {
    s32 color;
    s32 temp_r4 = arg0->unkB1C.unk71 - 1;
    if (sub_0202C090(arg0->unk0, temp_r4, 8) == 0) {
        color = MAKE_TEXT_COLOR(5, 6, 0);
    } else {
        color = MAKE_TEXT_COLOR(3, 4, 0);
    }
    ov44_02231918(arg0, temp_r4);
    ReadMsgDataIntoString(arg0->unk168, 42, arg0->unk170);
    StringExpandPlaceholders(arg0->unkB1C.unk0, arg0->unk178, arg0->unk170);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 1, arg0->unk178, 32, 8, 255, color, 0);
    UnkStruct_ov44_02231958* sp10 = ov44_02231958(arg0, temp_r4);
    u32 temp_r0 = ov44_02229F00(arg0, sp10);
    ReadMsgDataIntoString(arg0->unk168, ov44_0223120C(temp_r0, &color), arg0->unk170);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk170, 104, 8, 255, color, 0);
    PlayerProfile* temp_r6 = PlayerProfile_New(HEAP_ID_53);
    Save_Profile_PlayerName_Set(temp_r6, sub_0202C298(arg0->unk0, temp_r4));
    BufferPlayersName(arg0->unkB1C.unk0, 0, temp_r6);
    Heap_Free(temp_r6);
    ReadMsgDataIntoString(arg0->unk168, 43, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 32, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ReadMsgDataIntoString(arg0->unk168, 150, arg0->unk170);
    StringExpandPlaceholders(arg0->unkB1C.unk0, arg0->unk178, arg0->unk170);

    u32 width = 232 - FontID_String_GetWidth(0, arg0->unk178, 0);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, width, 32, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ReadMsgDataIntoString(arg0->unk168, 44, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 56, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    BufferIntegerAsString(arg0->unkB1C.unk0, 0, sub_0202C090(arg0->unk0, temp_r4, 1), 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    ReadMsgDataIntoString(arg0->unk168, 45, arg0->unk170);
    StringExpandPlaceholders(arg0->unkB1C.unk0, arg0->unk178, arg0->unk170);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 120, 56, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    BufferIntegerAsString(arg0->unkB1C.unk0, 0, sub_0202C090(arg0->unk0, temp_r4, 2), 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    ReadMsgDataIntoString(arg0->unk168, 46, arg0->unk170);
    StringExpandPlaceholders(arg0->unkB1C.unk0, arg0->unk178, arg0->unk170);
    
    width = 232 - FontID_String_GetWidth(0, arg0->unk178, 0);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, width, 56, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ReadMsgDataIntoString(arg0->unk168, 47, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 80, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    BufferIntegerAsString(arg0->unkB1C.unk0, 0, sub_0202C090(arg0->unk0, temp_r4, 3), 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    ReadMsgDataIntoString(arg0->unk168, 48, arg0->unk170);
    StringExpandPlaceholders(arg0->unkB1C.unk0, arg0->unk178, arg0->unk170);

    width = 232 - FontID_String_GetWidth(0, arg0->unk178, 0);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, width, 80, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    if (ov44_02229EF8(arg0) == 1) {
        ReadMsgDataIntoString(arg0->unk168, 49, arg0->unk178);
        AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 104, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
        BufferIntegerAsString(arg0->unkB1C.unk0, 0, sub_0202C090(arg0->unk0, temp_r4, 9), 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
        ReadMsgDataIntoString(arg0->unk168, 50, arg0->unk170);
        StringExpandPlaceholders(arg0->unkB1C.unk0, arg0->unk178, arg0->unk170);

        width = 232 - FontID_String_GetWidth(0, arg0->unk178, 0);
        AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, width, 104, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    }
    ReadMsgDataIntoString(arg0->unk168, 51, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 128, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    s32 temp_r0_2 = sub_0202C090(arg0->unk0, temp_r4, 6);
    if (temp_r0_2 != 0) {
        BufferIntegerAsString(arg0->unkB1C.unk0, 2, temp_r0_2, 2, PRINTING_MODE_LEFT_ALIGN, TRUE);
        BufferIntegerAsString(arg0->unkB1C.unk0, 0, sub_0202C090(arg0->unk0, temp_r4, 4), 4, PRINTING_MODE_LEFT_ALIGN, TRUE);
        BufferMonthNameAbbr(arg0->unkB1C.unk0, 1, sub_0202C090(arg0->unk0, temp_r4, 5));
        ReadMsgDataIntoString(arg0->unk168, 74, arg0->unk170);
        StringExpandPlaceholders(arg0->unkB1C.unk0, arg0->unk178, arg0->unk170);

        width = 232 - FontID_String_GetWidth(0, arg0->unk178, 0);
        AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, width, 128, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    }
    ov44_0222AC54(arg0->unk15C, &arg0->unkA88[132], 6, 2, 2, temp_r0);

    s32 var_r1;
    if (sp10->unk21 != 0) {
        var_r1 = 8;
    } else {
        var_r1 = 1;
    }
    ov44_0222ABDC(arg0->unk15C, &arg0->unkA88[132], 6, 28, 2, var_r1, 0);
}

void ov44_022307E0(UnkStruct_ov44_022319EC* arg0) {
    s32 temp_r4 = arg0->unkB1C.unk71 - 1;
    ReadMsgDataIntoString(arg0->unk168, 52, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 0, 255, MAKE_TEXT_COLOR(15, 14, 0), 0);
    ov44_02231084(arg0, arg0->unk178, 0, temp_r4);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 24, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231054(arg0, 65, 113, temp_r4, 120, 24);
    ReadMsgDataIntoString(arg0->unk168, 59, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 48, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231054(arg0, 65, 112, temp_r4, 120, 48);
}

void ov44_022308B0(UnkStruct_ov44_022319EC* arg0) {
    s32 temp_r4 = arg0->unkB1C.unk71 - 1;
    ReadMsgDataIntoString(arg0->unk168, 53, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 0, 255, MAKE_TEXT_COLOR(15, 14, 0), 0);
    ReadMsgDataIntoString(arg0->unk168, 60, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 24, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ReadMsgDataIntoString(arg0->unk168, 62, arg0->unk178);
    
    u32 width = 232 - FontID_String_GetWidth(0, arg0->unk178, 0);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, width, 24, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231084(arg0, arg0->unk178, 1, temp_r4);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 48, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231054(arg0, 65, 115, temp_r4, 104, 48);
    ov44_02231054(arg0, 66, 117, temp_r4, 212, 48);
    ReadMsgDataIntoString(arg0->unk168, 59, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 64, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231054(arg0, 65, 114, temp_r4, 104, 64);
    ov44_02231054(arg0, 66, 116, temp_r4, 212, 64);
    ReadMsgDataIntoString(arg0->unk168, 61, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 88, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ReadMsgDataIntoString(arg0->unk168, 62, arg0->unk178);

    width = 232 - FontID_String_GetWidth(0, arg0->unk178, 0);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, width, 88, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231084(arg0, arg0->unk178, 2, temp_r4);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 112, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231054(arg0, 65, 119, temp_r4, 104, 112);
    ov44_02231054(arg0, 66, 121, temp_r4, 212, 112);
    ReadMsgDataIntoString(arg0->unk168, 59, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 128, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231054(arg0, 65, 118, temp_r4, 104, 128);
    ov44_02231054(arg0, 66, 120, temp_r4, 212, 128);
}

void ov44_02230B2C(UnkStruct_ov44_022319EC* arg0) {
    s32 temp_r4 = arg0->unkB1C.unk71 - 1;
    ReadMsgDataIntoString(arg0->unk168, 54, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 0, 255, MAKE_TEXT_COLOR(15, 14, 0), 0);
    ReadMsgDataIntoString(arg0->unk168, 63, arg0->unk178);
    
    u32 width = 180 -(FontID_String_GetWidth(0, arg0->unk178, 0) + 1) / 2;
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, width, 24, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231084(arg0, arg0->unk178, 3, temp_r4);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 48, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231054(arg0, 68, 135, temp_r4, 104, 48);
    ov44_02231054(arg0, 67, 136, temp_r4, 212, 48);
    ReadMsgDataIntoString(arg0->unk168, 59, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 80, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231054(arg0, 68, 134, temp_r4, 104, 80);
    ov44_02231054(arg0, 67, 138, temp_r4, 212, 80);
}

void ov44_02230C68(UnkStruct_ov44_022319EC* arg0, enum HeapID arg1) {
    FrontierSave* frontierSave = Save_Frontier_GetStatic(arg0->unk160);
    s32 temp_r4 = arg0->unkB1C.unk71 - 1;
    ReadMsgDataIntoString(arg0->unk168, 55, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 0, 255, MAKE_TEXT_COLOR(15, 14, 0), 0);
    String* temp_r0 = GetSpeciesName(FrontierSave_GetStat(frontierSave, 124, temp_r4), arg1);
    u32 width = 33 - (FontID_String_GetWidth(0, temp_r0, 0) + 1) / 2;
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, temp_r0, width, 24, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    String_Delete(temp_r0);
    ov44_02231084(arg0, arg0->unk178, 4, temp_r4);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 48, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231054(arg0, 65, 123, temp_r4, 108, 48);
    ReadMsgDataIntoString(arg0->unk168, 59, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 80, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231054(arg0, 65, 122, temp_r4, 108, 80);
}

void ov44_02230D8C(UnkStruct_ov44_022319EC* arg0) {
    s32 temp_r4 = arg0->unkB1C.unk71 - 1;
    ReadMsgDataIntoString(arg0->unk168, 56, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 0, 255, MAKE_TEXT_COLOR(15, 14, 0), 0);
    ov44_02231084(arg0, arg0->unk178, 5, temp_r4);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 24, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231054(arg0, 69, 143, temp_r4, 112, 24);
    ReadMsgDataIntoString(arg0->unk168, 59, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 48, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_02231054(arg0, 69, 142, temp_r4, 112, 48);
}

void ov44_02230E5C(UnkStruct_ov44_022319EC* arg0) {
    UnkStruct_021D2230* temp_r6 = sub_0202C6F4(arg0->unk160);
    s32 temp_r4 = arg0->unkB1C.unk71 - 1;
    ReadMsgDataIntoString(arg0->unk168, 70, arg0->unk178);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 0, 255, MAKE_TEXT_COLOR(15, 14, 0), 0);
    BufferWiFiPlazaActivityName(arg0->unkB1C.unk0, 0, 0);
    ReadMsgDataIntoString(arg0->unk168, 71, arg0->unk170);
    StringExpandPlaceholders(arg0->unkB1C.unk0, arg0->unk178, arg0->unk170);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 24, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_022310C8(arg0, 72, sub_0202C090(temp_r6, temp_r4, 10), 164, 24);
    BufferWiFiPlazaActivityName(arg0->unkB1C.unk0, 0, 1);
    ReadMsgDataIntoString(arg0->unk168, 71, arg0->unk170);
    StringExpandPlaceholders(arg0->unkB1C.unk0, arg0->unk178, arg0->unk170);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 48, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_022310C8(arg0, 72, sub_0202C090(temp_r6, temp_r4, 11), 164, 48);
    BufferWiFiPlazaActivityName(arg0->unkB1C.unk0, 0, 2);
    ReadMsgDataIntoString(arg0->unk168, 71, arg0->unk170);
    StringExpandPlaceholders(arg0->unkB1C.unk0, arg0->unk178, arg0->unk170);
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, 8, 72, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ov44_022310C8(arg0, 72, sub_0202C090(temp_r6, temp_r4, 12), 164, 72);
}

void ov44_02230FE8(UnkStruct_ov44_022319EC* arg0) {
    for (s32 i = 0; i < 5; i++) {
        CopyToBgTilemapRect(arg0->unk15C, 6, i + 26, 0, 1, 1, &arg0->unkB1C.unk48->rawData[0], 0, 0, arg0->unkB1C.unk48->screenWidth / 8, arg0->unkB1C.unk48->screenHeight / 8);
    }
    BgTilemapRectChangePalette(arg0->unk15C, 6, 26, 0, 5, 1, 6);
}

void ov44_02231054(UnkStruct_ov44_022319EC* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    ov44_022310C8(arg0, arg1, FrontierSave_GetStat(Save_Frontier_GetStatic(arg0->unk160), arg2, arg3), arg4, arg5);
}

void ov44_02231084(UnkStruct_ov44_022319EC* arg0, String* arg1, u32 arg2, s32 arg3) {

    GF_ASSERT(arg2 < 6);
    s32 var_r1;
    if (FrontierSave_GetStat(Save_Frontier_GetStatic(arg0->unk160), ov44_022353D0[arg2], arg3) == 0) {
        var_r1 = 57;
    } else {
        var_r1 = 58;
    }
    ReadMsgDataIntoString(arg0->unk168, var_r1, arg1);
}

void ov44_022310C8(UnkStruct_ov44_022319EC* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    BufferIntegerAsString(arg0->unkB1C.unk0, 0, arg2, 4, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    ReadMsgDataIntoString(arg0->unk168, arg1, arg0->unk170);
    StringExpandPlaceholders(arg0->unkB1C.unk0, arg0->unk178, arg0->unk170);
    u32 width = arg3 - (FontID_String_GetWidth(0, arg0->unk178, 0) + 1) / 2;
    AddTextPrinterParameterizedWithColor(&arg0->unkB1C.unk1FC, 0, arg0->unk178, width, arg4, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
}

void ov44_02231148(UnkStruct_ov44_022319EC* arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {
    GF_ASSERT(arg4 < 4);
    if (arg4 == 3) {
        arg4 = 1;
    }
    if (arg4 < 2) {
        CopyToBgTilemapRect(arg0->unk15C, 6, arg1, arg2, 16, 6, &arg0->unkB1C.unk8->rawData, (u16)(arg4 << 4), (u16)(arg3 * 6), 32, 18);
    } else {
        s64 offset = 18 << 4;
        CopyToBgTilemapRect(arg0->unk15C, 6, arg1, arg2, 16, 6, &arg0->unkB1C.unk8->rawData + offset, 0, (u16)(arg3 * 6), 16, 18);
    }
    BgTilemapRectChangePalette(arg0->unk15C, 6, arg1, arg2, 16, 6, arg3 + 4);
}

s32 ov44_0223120C(u32 arg0, s32* color) {
    *color = MAKE_TEXT_COLOR(1, 2, 0);
    s32 ret = 0;
    switch (arg0) {
    case 1:
        ret = 36;
        *color = MAKE_TEXT_COLOR(2, 14, 0);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        ret = 27;
        *color = MAKE_TEXT_COLOR(2, 14, 0);
        break;
    case 9:
        ret = 96;
        break;
    case 10:
        ret = 97;
        break;
    case 11:
        ret = 95;
        break;
    case 12:
        ret = 99;
        break;
    case 13:
        ret = 100;
        break;
    case 14:
        ret = 98;
        break;
    case 8:
        ret = 29;
        *color = MAKE_TEXT_COLOR(2, 14, 0);
        break;
    case 15:
        ret = 28;
        break;
    case 18:
    case 19:
        ret = 92;
        break;
    case 20:
        ret = 33;
        *color = MAKE_TEXT_COLOR(2, 14, 0);
        break;    
    case 21:
        ret = 32;
        break;
    case 22:
    case 24:
    case 26:
        ret = 35;
        *color = MAKE_TEXT_COLOR(2, 14, 0);
        break;
    case 23:
    case 25:
    case 27:
        ret = 34;
        break;
    case 16:
        ret = 79;
        break;
    default:
        ret = 92;
    }
    return ret;
}

void ov44_022312B8(UnkStruct_ov44_022319EC* arg0, Window* arg1, s32 arg2, s32 arg3, s32 arg4) {    
    FillWindowPixelBuffer(arg1, 0);
    u8 temp_r7 = ov44_02235364[arg3];
    s32 sp14 = arg2 - 1;
    s32 color;
    if (sub_0202C090(arg0->unk0, sp14, 8) == 0) {
        color = MAKE_TEXT_COLOR(5, 6, 0);
    } else {
        color = MAKE_TEXT_COLOR(3, 4, 0);
    }
    ov44_02231918(arg0, sp14);
    ReadMsgDataIntoString(arg0->unk168, 42, arg0->unk170);
    StringExpandPlaceholders(arg0->unkB1C.unk0, arg0->unk178, arg0->unk170);
    AddTextPrinterParameterizedWithColor(arg1, 1, arg0->unk178, 0, temp_r7, 255, color, 0);
    ScheduleWindowCopyToVram(arg1);
}

void ov44_02231344(UnkStruct_ov44_022319EC* arg0, Window* arg1, s32 arg2, u32 arg3, u32 arg4) {
    s32 var_r0;
    UnkStruct_ov44_02231958* spC = ov44_02231958(arg0, arg2 - 1);
    u32 sp8 = ov44_02229F00(arg0, spC);
    u32 temp_r7 = ov44_02235364[arg3];
    for (s32 i = 0; i < 2; i ++) {
        FillWindowPixelBuffer(&arg1[i], 0);
        if (i == 0) {
            ov44_0222AD34(&arg1[i], &arg0->unkA88[132], 0, temp_r7, sp8);
        } else {
            if (spC->unk21 != 0) {
                var_r0 = 8;
            } else {
                var_r0 = 1;
            }
            ov44_0222ACE8(&arg1[i], &arg0->unkA88[132], 0, temp_r7, var_r0, 0);
        }
        ScheduleWindowCopyToVram(&arg1[i]);
    }
}

void ov44_022313C8(UnkStruct_ov44_022319EC* arg0) {
    if (arg0->unkB1C.unk71 != 0) {
        arg0->unkB1C.unk76++;
        if (arg0->unkB1C.unk76 >= ov44_02235368[arg0->unkB1C.unk72]) {
            arg0->unkB1C.unk76 = 0;
            arg0->unkB1C.unk72++;
            arg0->unkB1C.unk75 = 1;
            if (arg0->unkB1C.unk72 >= 4) {
                arg0->unkB1C.unk76 = 0;
                arg0->unkB1C.unk72 = 0;
                arg0->unkB1C.unk73 = 1;
            }
        }
    }
}

void ov44_02231420(UnkStruct_ov44_022319EC* arg0, NARC* arg1, enum HeapID arg2) {
    SpriteTemplate spriteTemplateList[3];
    spriteTemplateList = ov44_02235570;

    GfGfx_EngineBTogglePlanes(16, 0);
    arg0->unkB1C.unk20C[0] = AddCharResObjFromOpenNarc(arg0->unk2BC[0], arg1, 43, FALSE, 30, 2, arg2);
    arg0->unkB1C.unk20C[1] = AddPlttResObjFromOpenNarc(arg0->unk2BC[1], arg1, 44, FALSE, 30, 2, 8, arg2);
    arg0->unkB1C.unk20C[2] = AddCellOrAnimResObjFromOpenNarc(arg0->unk2BC[2], arg1, 41, FALSE, 30, GF_GFX_RES_TYPE_CELL, arg2);
    arg0->unkB1C.unk20C[3] = AddCellOrAnimResObjFromOpenNarc(arg0->unk2BC[3], arg1, 42, FALSE, 30, GF_GFX_RES_TYPE_ANIM, arg2);
    GF_ASSERT(sub_0200AE18(arg0->unkB1C.unk20C[0]) != 0);
    GF_ASSERT(sub_0200B00C(arg0->unkB1C.unk20C[1]) != 0);
    sub_0200A740(arg0->unkB1C.unk20C[0]);
    sub_0200A740(arg0->unkB1C.unk20C[1]);
    
    SpriteResourcesHeader spriteResourcesHeader;
    CreateSpriteResourcesHeader(&spriteResourcesHeader, 30, 30, 30, 30, -1, -1, 0, 0, arg0->unk2BC[0], arg0->unk2BC[1], arg0->unk2BC[2], arg0->unk2BC[3], 0, 0);
    for (s32 i = 0; i < 3; i++) {
        spriteTemplateList[i].spriteList = arg0->unk190;
        spriteTemplateList[i].header = &spriteResourcesHeader;
        spriteTemplateList[i].heapID = arg2;
        arg0->unkB1C.unk21C[i] = Sprite_CreateAffine(&spriteTemplateList[i]);
        Sprite_SetAnimCtrlSeq(arg0->unkB1C.unk21C[i], _0223535C[i]);
    }
    FontID_Alloc(2, arg2);
    Window window;
    InitWindow(&window);
    AddTextWindowTopLeftCorner(arg0->unk15C, &window, 8, 2, 0, 0);
    String* string = String_New(64, arg2);
    ReadMsgDataIntoString(arg0->unk168, 73, string);
    AddTextPrinterParameterizedWithColor(&window, 2, string, FontID_String_GetCenterAlignmentX(2, string, 0, 64), 0, 255, MAKE_TEXT_COLOR(1, 2, 0), 0);
    String_Delete(string);
    UnkStruct_02013910* temp_r0_4 = sub_02013910(&window, arg2);
    GF_ASSERT(sub_02021AC8(sub_02013948(temp_r0_4, NNS_G2D_VRAM_TYPE_2DSUB), TRUE, NNS_G2D_VRAM_TYPE_2DSUB, &arg0->unkB1C.unk228) != 0);
    
    TextOBJTemplate textObjTemplate;
    textObjTemplate.fontSystem = arg0->unk2CC;
    textObjTemplate.window = &window;
    textObjTemplate.spriteList = arg0->unk190;
    textObjTemplate.plttResourceProxy = SpriteTransfer_GetPaletteProxy(arg0->unkB1C.unk20C[1], NULL);
    textObjTemplate.sprite = arg0->unkB1C.unk21C[1]; 
    textObjTemplate.offset = arg0->unkB1C.unk228.offset;
    textObjTemplate.x = -32;
    textObjTemplate.y = -8;
    textObjTemplate.unk_20 = 0;
    textObjTemplate.unk_24 = 0;
    textObjTemplate.vram = 2;
    textObjTemplate.heapID = arg2;
    arg0->unkB1C.unk234 = TextOBJ_Create(&textObjTemplate, temp_r0_4);
    sub_02013938(temp_r0_4);
    RemoveWindow(&window);
    FontID_Release(2);
    arg0->unkB1C.unk244 = TouchHitboxController_Create(&ov44_02235394, 3, (TouchHitboxControllerCallback)ov44_022317F0, arg0, arg2);
}

void ov44_022316B0(UnkStruct_ov44_022319EC* arg0) {
    TouchHitboxController_Destroy(arg0->unkB1C.unk244);
    TextOBJ_Destroy(arg0->unkB1C.unk234);
    sub_02021B5C(&arg0->unkB1C.unk228);

    for (s32 i = 0; i < 3; i++) {
        Sprite_Delete(arg0->unkB1C.unk21C[i]);
    }
    sub_0200AEB0(arg0->unkB1C.unk20C[0]);
    sub_0200B0A8(arg0->unkB1C.unk20C[1]);

    for (s32 i = 0; i < 4; i++) {
        DestroySingle2DGfxResObj(arg0->unk2BC[i], arg0->unkB1C.unk20C[i]);
    }
}

void ov44_02231720(UnkStruct_ov44_022319EC* arg0) {
    arg0->unkB1C.unk238 = 1;
    GfGfx_EngineBTogglePlanes(16, 1);
    arg0->unkB1C.unk23C = 0;
    for (s32 i = 0; i < 3; i++) {
        ov44_02231800(&arg0->unkB1C, i);
    }
}

void ov44_02231754(UnkStruct_ov44_022319EC* arg0) {
    if (arg0->unkB1C.unk238 == 2) {
        Sprite_SetDrawFlag(arg0->unkB1C.unk21C[1], 1);
        TextOBJ_SetSpritesDrawFlag(arg0->unkB1C.unk234, 1);
    }
    arg0->unkB1C.unk238 = 0;
    GfGfx_EngineBTogglePlanes(16, 0);
}

s32 ov44_02231788(UnkStruct_ov44_022319EC* arg0) {
    if (arg0->unkB1C.unk238 == 0) {
        return 0;
    }
    arg0->unkB1C.unk23C = 3;
    arg0->unkB1C.unk240 = 255;
    TouchHitboxController_IsTriggered(arg0->unkB1C.unk244);

    for (s32 i = 0; i < 3; i ++){
        if ((arg0->unkB1C.unk238 != 2 || i != 1) && ov44_0223183C(&arg0->unkB1C, i, arg0->unkB1C.unk23C, arg0->unkB1C.unk240) == 1) {
            return i + 1;
        }
    } 
    return 0;
}

void ov44_022317F0(u32 arg0, u32 arg1, UnkStruct_ov44_022319EC* arg2) {
    arg2->unkB1C.unk23C = arg0;
    arg2->unkB1C.unk240 = arg1;
}

void ov44_02231800(UnkStruct_ov44_02231800* arg0, s32 arg1) {
    Sprite_SetAnimCtrlSeq(arg0->unk21C[arg1], _0223535C[arg1]);
    Sprite_SetAnimationFrame(arg0->unk21C[arg1], 4);
    if (arg1 == 1) {
        sub_020136B4(arg0->unk234, -32, -8);
    }
}

s32 ov44_0223183C(UnkStruct_ov44_02231800* arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp8 = 0;
    if (arg2 == arg1) {
        u32 temp_r0 = Sprite_GetAnimationFrame(arg0->unk21C[arg1]);
        if (arg3 == 0 || arg3 == 2) {
            if (arg3 == 0) {
                Sprite_SetAnimCtrlSeq(arg0->unk21C[arg1], ov44_02235360[arg1]);
            }
            if (temp_r0 < 3) {
                Sprite_UpdateAnim(arg0->unk21C[arg1], 2 * FX32_ONE);
                u16 temp_r4_2 = Sprite_GetAnimationFrame(arg0->unk21C[arg1]);
                if (arg1 == 1) {
                    sub_020136B4(arg0->unk234, -32, ov44_0223536C[temp_r4_2]);
                }
                if (temp_r4_2 >= 3) {
                    sp8 = 1;
                }
            }
        }
        else if (arg3 == 1 && temp_r0 < 3) {
            sp8 = 1;
        }
    } else {
        u16 temp_r7_2 = Sprite_GetAnimationNumber(arg0->unk21C[arg1]);
        u16 spC = Sprite_GetAnimationFrame(arg0->unk21C[arg1]);
        if (temp_r7_2 == ov44_02235360[arg1]) {
            Sprite_SetAnimCtrlSeq(arg0->unk21C[arg1], _0223535C[arg1]);
            Sprite_SetAnimationFrame(arg0->unk21C[arg1], 4 - spC);
        }
        Sprite_UpdateAnim(arg0->unk21C[arg1], 2 * FX32_ONE);
    }
    return sp8;
}