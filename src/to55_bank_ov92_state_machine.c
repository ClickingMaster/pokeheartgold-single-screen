#include "global.h"

#define M2C_UNK            void
#define M2C_FIELD(p, t, o) (*(t)((u8 *)(p) + (o)))

s32 IsPaletteFadeFinished();                    /* extern */
void *OverlayManager_GetData();                 /* extern */
M2C_UNK StopSE(s32, s32);                       /* extern */
M2C_UNK WifiConnection_StopVoiceChat();                        /* extern */
s32 PlazaGameEntry_NewMimeJrTop(void *, s32);                 /* extern */
M2C_UNK PlazaGameEntry_Free(s32);                     /* extern */
s32 PlazaGameEntry_IsDone(s32);                         /* extern */
s8 PlazaGameEntry_GetSelection(s32);                          /* extern */
s32 PlazaGameResults_NewMimeJrTop(void *, void *, s32);         /* extern */
M2C_UNK PlazaGameResults_Free(s32);                     /* extern */
s32 PlazaGameResults_IsDone(s32);                         /* extern */
s32 PlazaGameResults_GetContinue(s32);                         /* extern */
M2C_UNK PlazaGame_CalcRanks(void *, u8);              /* extern */
M2C_UNK PlazaGameSign_StartCountdown(s32);                     /* extern */
M2C_UNK PlazaGameSign_StartFinish(s32);                     /* extern */
s32 PlazaGameSign_IsDone(s32);                         /* extern */
M2C_UNK ov92_0225C5C4(void *);                  /* extern */
s32 ov92_0225CC6C(void *);                      /* extern */
M2C_UNK ov92_0225D1FC(void *);                  /* extern */
M2C_UNK ov92_0225D344(void *);                  /* extern */
s32 ov92_0225D868(s32);                         /* extern */
M2C_UNK ov92_0225D88C(void *, s32, s32, u32 *); /* extern */
s32 ov92_0225D8C4(void *);                      /* extern */
s32 ov92_0225D8E4();                            /* extern */
M2C_UNK ov92_0225DA40(void *);                  /* extern */
M2C_UNK ov92_0225E008(void *);                  /* extern */
M2C_UNK ov92_0225E070(void *);                  /* extern */
M2C_UNK ov92_0225E100(void *);                  /* extern */
M2C_UNK ov92_0225E1A8(void *, void *);          /* extern */
M2C_UNK ov92_0225E360(void *);                  /* extern */
s32 ov92_0225EB70(s32);                         /* extern */
M2C_UNK ov92_0225FC2C(void *);                  /* extern */
s32 ov92_0226077C(void *, s32);                 /* extern */
M2C_UNK ov92_0226156C(void *);                  /* extern */
M2C_UNK ov92_02261B18(void *);                  /* extern */
M2C_UNK Comm_SendCommand(s32, void *, s32);         /* extern */
M2C_UNK sub_02037AC0(s32);                      /* extern */
s32 sub_02037B38(s32);                          /* extern */

s32 ov92_0225CDF4(void *arg0, u32 *arg1);

s32 ov92_0225CDF4(void *arg0, u32 *arg1) {
    s32 temp_r0;
    s32 temp_r0_4;
    s32 temp_r0_5;
    s32 temp_r0_6;
    s32 temp_r0_7;
    s32 temp_r0_8;
    s32 temp_r0_9;
    s32 temp_r2;
    s32 temp_r5;
    s32 temp_r6;
    s32 var_r5;
    s32 var_r6;
    u32 temp_r0_3;
    u32 temp_r1;
    void *temp_r4;
    void *var_r5_3;

    temp_r4 = OverlayManager_GetData(arg0);
    temp_r0 = ov92_0225D8E4();
    switch (temp_r0) { /* switch 1; irregular */
    case 1:            /* switch 1 */
        return 0;
    case 2: /* switch 1 */
        return 1;
    default:             /* switch 1 */
    case 0:              /* switch 1 */
        switch (*arg1) { /* switch 2; irregular */
        case 0:          /* switch 2 */
            M2C_FIELD(temp_r4, s32 *, 0x9C) = PlazaGameEntry_NewMimeJrTop(temp_r4 + 0x8C, 0x71);
            ov92_0225D88C(temp_r4, 1, 1, arg1);
            break;
        case 1: /* switch 2 */
            ov92_0225D88C(temp_r4, PlazaGameEntry_IsDone(M2C_FIELD(temp_r4, s32 *, 0x9C)), 2, arg1);
            break;
        case 2: /* switch 2 */
            M2C_FIELD(temp_r4, s8 *, 0x14) = PlazaGameEntry_GetSelection(M2C_FIELD(temp_r4, s32 *, 0x9C));
            PlazaGameEntry_Free(M2C_FIELD(temp_r4, s32 *, 0x9C));
            M2C_FIELD(temp_r4, s32 *, 0x9C) = 0;
            ov92_0225C5C4(temp_r4);
            ov92_0225D88C(temp_r4, 1, 3, arg1);
            break;
        case 3: /* switch 2 */
            temp_r5 = ov92_0225D868(0x1B);
            ov92_0225E1A8(temp_r4, M2C_FIELD(temp_r4, void **, 4));
            ov92_0225D88C(temp_r4, temp_r5, 4, arg1);
            break;
        case 4: /* switch 2 */
            ov92_0225D88C(temp_r4, IsPaletteFadeFinished(), 5, arg1);
            break;
        case 5: /* switch 2 */
            ov92_0225E070(M2C_FIELD(temp_r4, void **, 4));
            ov92_0226156C(M2C_FIELD(temp_r4, void **, 4));
            ov92_02261B18(M2C_FIELD(temp_r4, void **, 4));
            ov92_0225D88C(temp_r4, 1, 6, arg1);
            break;
        case 6: /* switch 2 */
            temp_r0_4 = ov92_0225CC6C(M2C_FIELD(temp_r4, void **, 4));
            if ((temp_r0_4 != 0) && (ov92_0225D8C4(temp_r4) == 1)) {
                M2C_FIELD(temp_r4, s32 *, 0x18) = (s32)M2C_FIELD(temp_r4, s32 *, 0x80);
                Comm_SendCommand(0x16, temp_r4 + 0x14, 8);
            }
            ov92_0225D88C(temp_r4, temp_r0_4, 7, arg1);
            break;
        case 7: /* switch 2 */
            ov92_0225D88C(temp_r4, M2C_FIELD(temp_r4, s32 *, 8), 8, arg1);
            break;
        case 8: /* switch 2 */
            PlazaGameSign_StartCountdown(M2C_FIELD(temp_r4, s32 *, 0x84));
            ov92_0225D88C(temp_r4, 1, 9, arg1);
            break;
        case 9: /* switch 2 */
            temp_r0_5 = PlazaGameSign_IsDone(M2C_FIELD(temp_r4, s32 *, 0x84));
            if (temp_r0_5 != 0) {
                ov92_0225E360(M2C_FIELD(temp_r4, void **, 4));
                ov92_0225E100(M2C_FIELD(temp_r4, void **, 4));
            }
            ov92_0225D88C(temp_r4, temp_r0_5, 0xA, arg1);
            break;
        case 10: /* switch 2 */
            var_r5 = M2C_FIELD(temp_r4, s32 *, 0xC);
            if ((var_r5 != 0) && ((s32)M2C_FIELD(M2C_FIELD(temp_r4, void **, 4), s32 *, 0x2AE4) < 0x4B0)) {
                var_r5 = 0;
            }
            if (var_r5 == 0) {
                if ((ov92_0225D8C4(temp_r4) == 1) && (ov92_0225EB70(M2C_FIELD(temp_r4, s32 *, 0)) == 0)) {
                    StopSE(0x589, 0);
                    Comm_SendCommand(0x17, NULL, 0);
                }
                ov92_0225FC2C(M2C_FIELD(temp_r4, void **, 4));
            }
            ov92_0225D88C(temp_r4, var_r5, 0xB, arg1);
            break;
        case 11: /* switch 2 */
            StopSE(0x58B, 0);
            StopSE(0x589, 0);
            PlazaGameSign_StartFinish(M2C_FIELD(temp_r4, s32 *, 0x84));
            ov92_0225D88C(temp_r4, 1, 0xC, arg1);
            break;
        case 12: /* switch 2 */
            ov92_0225D88C(temp_r4, PlazaGameSign_IsDone(M2C_FIELD(temp_r4, s32 *, 0x84)), 0xD, arg1);
            break;
        case 13: /* switch 2 */
            temp_r1 = 0x64 * ((s32)M2C_FIELD(M2C_FIELD(temp_r4, void **, 4), s32 *, 0x2AF0) / 10);
            M2C_FIELD(temp_r4, u32 *, 0x1C) = temp_r1;
            if (temp_r1 >= 0x186A0U) {
                M2C_FIELD(temp_r4, u32 *, 0x1C) = 0x186A0U;
            }
            if (M2C_FIELD(M2C_FIELD(temp_r4, void **, 4), s32 *, 0x2B94) == 0) {
                M2C_FIELD(temp_r4, u32 *, 0x1C) = 1U;
            }
            Comm_SendCommand(0x19, temp_r4 + 0x1C, 8);
            ov92_0225D88C(temp_r4, 1, 0xE, arg1);
            break;
        case 14: {
            s32 var_r1;
            s32 var_r3;
            s32 var_r0;
            void *var_r5_2;
            var_r1 = 0;
            var_r0 = var_r1;
            var_r3 = var_r1;
            var_r5_2 = temp_r4;
            do {
                temp_r2 = M2C_FIELD(var_r5_2, s32 *, 0x24);
                if (temp_r2 != 0) {
                    var_r0 += 1;
                }
                var_r3 += 1;
                var_r5_2 += 8;
            } while (var_r3 < 4);
            if (var_r0 == M2C_FIELD(M2C_FIELD(temp_r4, void **, 4), s32 *, 4)) {
                var_r1 = 1;
            }
            ov92_0225D88C(temp_r4, var_r1, 0xF, arg1);
            break;
        }
        case 15: /* switch 2 */
            ov92_0225D88C(temp_r4, ov92_0225D868(0x1A), 0x10, arg1);
            break;
        case 16: /* switch 2 */
            temp_r0_6 = IsPaletteFadeFinished();
            if (temp_r0_6 != 0) {
                ov92_0225D1FC(temp_r4);
            }
            ov92_0225D88C(temp_r4, temp_r0_6, 0x11, arg1);
            break;
        case 17: /* switch 2 */
            var_r6 = 0;
            var_r5_3 = temp_r4;
            do {
                temp_r0_7 = ov92_0226077C(temp_r4, var_r6);
                if (temp_r0_7 != 0xFF) {
                    temp_r0_8 = temp_r0_7 * 4;
                    M2C_FIELD(temp_r4 + temp_r0_8, s32 *, 0xA0) = (s32)M2C_FIELD(var_r5_3, s32 *, 0x24);
                    if ((u32)M2C_FIELD(temp_r4 + temp_r0_8, u32 *, 0xA0) <= 1U) {
                        M2C_FIELD(temp_r4 + temp_r0_8, s32 *, 0xA0) = 0;
                    }
                }
                var_r6 += 1;
                var_r5_3 += 8;
            } while (var_r6 < 4);
            PlazaGame_CalcRanks(temp_r4 + 0xA0, M2C_FIELD(temp_r4, u8 *, 0x94));
            M2C_FIELD(temp_r4, s32 *, 0xB8) = PlazaGameResults_NewMimeJrTop(temp_r4 + 0x8C, temp_r4 + 0xA0, 0x71);
            ov92_0225D88C(temp_r4, 1, 0x12, arg1);
            break;
        case 18: /* switch 2 */
            temp_r0_9 = PlazaGameResults_IsDone(M2C_FIELD(temp_r4, s32 *, 0xB8));
            if ((temp_r0_9 != 0) && (temp_r6 = PlazaGameResults_GetContinue(M2C_FIELD(temp_r4, s32 *, 0xB8)), PlazaGameResults_Free(M2C_FIELD(temp_r4, s32 *, 0xB8)), M2C_FIELD(temp_r4, s32 *, 0xB8) = 0, temp_r6 != 0)) {
                M2C_FIELD(temp_r4, s32 *, 0xCC) = 1;
                ov92_0225D344(temp_r4);
                ov92_0225D88C(temp_r4, temp_r0_9, 0, arg1);
                if (M2C_FIELD(M2C_FIELD(temp_r4, void **, 0x88), s32 *, 0x3C) != 0) {
                    WifiConnection_StopVoiceChat();
                }
            } else {
                ov92_0225D88C(temp_r4, temp_r0_9, 0x13, arg1);
            }
            break;
        case 19: /* switch 2 */
            sub_02037AC0(0x1B);
            if (M2C_FIELD(M2C_FIELD(temp_r4, void **, 0x88), s32 *, 0x3C) != 0) {
                WifiConnection_StopVoiceChat();
            }
            ov92_0225D88C(temp_r4, 1, 0x14, arg1);
            break;
        case 20: /* switch 2 */
        default: /* switch 2 */
            if (sub_02037B38(0x1B) == 1) {
                return 1;
            }
            return 0;
        }
        temp_r0_3 = *arg1;
        if (((s32)temp_r0_3 >= 3) && ((s32)temp_r0_3 < 0x10)) {
            ov92_0225E008(M2C_FIELD(temp_r4, void **, 4));
            ov92_0225DA40(temp_r4);
        }
        return 0;
        break;
    }
}
