#include "overlay_44_02232E9C.h"

#include "save_frontier.h"
#include "touch_hitbox_controller.h"
#include "bg_window.h"
#include "msgdata.h"
#include "text.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "unk_0200ACF0.h"
#include "unk_02013534.h"
#include "unk_0200A090.h"
#include "unk_0202C034.h"
#include "unk_02030A98.h"

extern u8 _0223535C[4];
extern u8 ov44_02235360[4];
extern u8 ov44_02235364[4];
extern u8 ov44_02235368[4];
extern s8 ov44_0223536C[8];
extern const TouchscreenHitbox ov44_02235394;
extern u32 ov44_022353D0[6];
extern SpriteTemplate ov44_02235570[3];
extern func_type_02236680 ov44_02236680[6];

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

s32 ov44_0223120C(u32 arg0, s32* arg1) {
    *arg1 = MAKE_TEXT_COLOR(1, 2, 0);
    s32 ret = 0;
    switch (arg0) {
    case 1:
        ret = 36;
        *arg1 = 0x20E00;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        ret = 27;
        *arg1 = 0x20E00;
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
        *arg1 = 0x20E00;
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
        *arg1 = 0x20E00;
        break;    
    case 21:
        ret = 32;
        break;
    case 22:
    case 24:
    case 26:
        ret = 35;
        *arg1 = 0x20E00;
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
    arg0->unkB1C.unk20C[0] = AddCharResObjFromOpenNarc(arg0->unk2BC[0], arg1, 43, 0, 30, 2, arg2);
    arg0->unkB1C.unk20C[1] = AddPlttResObjFromOpenNarc(arg0->unk2BC[1], arg1, 44, 0, 30, 2, 8, arg2);
    arg0->unkB1C.unk20C[2] = AddCellOrAnimResObjFromOpenNarc(arg0->unk2BC[2], arg1, 41, 0, 30, GF_GFX_RES_TYPE_CELL, arg2);
    arg0->unkB1C.unk20C[3] = AddCellOrAnimResObjFromOpenNarc(arg0->unk2BC[3], arg1, 42, 0, 30, GF_GFX_RES_TYPE_ANIM, arg2);
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
                Sprite_UpdateAnim(arg0->unk21C[arg1], 0x2000);
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
        Sprite_UpdateAnim(arg0->unk21C[arg1], 0x2000);
    }
    return sp8;
}