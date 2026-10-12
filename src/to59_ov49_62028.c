#include "global.h"
#pragma require_prototypes off
typedef u32 M2C_UNK;
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))


struct _m2c_stack_ov49_02262028 {
    /* 0x00 */ char pad0[0x10];
    /* 0x10 */ s32 sp10;                            /* inferred */
    /* 0x14 */ s32 sp14;                            /* inferred */
    /* 0x18 */ s32 sp18;                            /* inferred */
    /* 0x1C */ s32 sp1C;                            /* inferred */
    /* 0x20 */ s32 sp20;                            /* inferred */
    /* 0x24 */ s32 sp24;                            /* inferred */
    /* 0x28 */ u32 sp28;                            /* inferred */
    /* 0x2C */ s32 sp2C;                            /* inferred */
    /* 0x30 */ s32 sp30;                            /* inferred */
    /* 0x34 */ M2C_UNK sp34;                        /* inferred */
    /* 0x34 */ char pad34[0x1C];
};                                                  /* size = 0x50 */

M2C_UNK PlaySE(s32);                                /* extern */
s32 ov45_0222A2C8();                                /* extern */
s32 ov45_0222A394(s32);                             /* extern */
M2C_UNK ov45_0222A404(s32);                         /* extern */
s32 ov45_0222A414(s32);                             /* extern */
M2C_UNK ov45_0222A43C(s32);                         /* extern */
M2C_UNK ov45_0222A450(s32, s32, s32);               /* extern */
M2C_UNK ov45_0222A480(s32, s32);                    /* extern */
M2C_UNK ov45_0222A498(s32, M2C_UNK *);              /* extern */
s32 ov45_0222A548(s32);                             /* extern */
M2C_UNK ov45_0222A5E8(s32, u8);                     /* extern */
M2C_UNK ov45_0222AB1C(s32);                         /* extern */
M2C_UNK ov45_0222AC14(s32, u8, u32, s32, s32, s32, s32, s32); /* extern */
s32 ov45_0222EC68(s32);                             /* extern */
M2C_UNK ov45_0222F1BC();                            /* extern */
s32 ov45_0222F218();                                /* extern */
s32 ov45_0222F274(u8);                              /* extern */
s32 ov45_0222F294(u8);                              /* extern */
s32 ov45_0222F2D4(u8);                              /* extern */
u32 ov45_0222F314(u8);                              /* extern */
s16 ov45_0222F3E8(u8);                              /* extern */
s32 ov45_0222F430();                                /* extern */
s32 ov45_0222F464();                                /* extern */
s32 WiFiPlazaRoom_GetLobby(s32);                             /* extern */
M2C_UNK WiFiPlazaRoom_PrintMessage(s32, s32);                    /* extern */
M2C_UNK ov49_0225A09C(s32, s32);                    /* extern */
s32 WiFiPlazaRoom_IsMessageFinished(s32);                             /* extern */
M2C_UNK ov49_0225A0BC(s32);                         /* extern */
M2C_UNK ov49_0225A0CC(s32);                         /* extern */
M2C_UNK WiFiPlazaRoom_CloseMessage(s32);                         /* extern */
M2C_UNK ov49_0225A174(s32, void *, s32, s32);       /* extern */
u32 ov49_0225A1D4(s32);                             /* extern */
M2C_UNK ov49_0225A1E4(s32, s32, s32);               /* extern */
M2C_UNK ov49_0225A294(s32);                         /* extern */
s32 WiFiPlazaRoom_GetYesNoResult(s32);                             /* extern */
M2C_UNK WiFiPlazaRoom_CloseYesNoMenu(s32);                         /* extern */
s32 WiFiPlazaRoom_GetMsgString(s32, s32, s32);                   /* extern */
M2C_UNK ov49_0225A37C(s32, u8, s32);                /* extern */
void *WiFiPlazaPlayerState_GetArg();                              /* extern */
M2C_UNK WiFiPlazaPlayerState_AllocWork(s32, s32);                    /* extern */
M2C_UNK WiFiPlazaPlayerState_FreeWork(s32);                         /* extern */
void *WiFiPlazaPlayerState_GetWork(s32);                           /* extern */
u32 WiFiPlazaPlayerState_GetStep(s32);                             /* extern */
M2C_UNK WiFiPlazaPlayerState_SetStep(s32, u8);                     /* extern */
M2C_UNK WiFiPlazaPlayerState_NextStep(s32);                         /* extern */
M2C_UNK ov49_02262BF8(void *, s32, s32, s32, s32);  /* extern */
M2C_UNK ov49_02262C20(void *, s32, s32, s32);       /* extern */
M2C_UNK ov49_02262C38(void *, s32, s32, s32);       /* extern */
M2C_UNK ov49_02262CA8(void *, s32);                 /* extern */
s32 ov49_02262CB4(void *, s32, void *, s32, s32);   /* extern */
M2C_UNK ov49_02262D70(void *, s32, u8, s32, s32);   /* extern */
M2C_UNK ov49_02262DB8(void *, s32);                 /* extern */
M2C_UNK ov49_02262DD4(void *, s16);                 /* extern */
M2C_UNK ov49_02262DF8(void *, s32, s32);            /* extern */
M2C_UNK ov49_02262E04(void *, s32, s32);            /* extern */
M2C_UNK CommPlayerInfo_Init(s32, s32);                     /* extern */
M2C_UNK CommPlayerInfo_Free();                             /* extern */
M2C_UNK CommPlayerInfo_SendLocal();                             /* extern */
M2C_UNK CommPlayerInfo_Accept();                             /* extern */
s32 CommPlayerInfo_FindReceived();                                 /* extern */
u32 CommPlayerInfo_CountAccepted();                                 /* extern */
M2C_UNK CommPlayerInfo_SetProfile();                             /* extern */
s32 Comm_CountConnected();                                 /* extern */
s32 Comm_GetLocalNetId();                                 /* extern */
M2C_UNK sub_020378E4(s32);                          /* extern */
M2C_UNK LinkSync_StartSync(u8);                           /* extern */
s32 LinkSync_IsSynced(u8);                               /* extern */
M2C_UNK LinkSync_ClearPackets();                             /* extern */
s32 LinkSync_SendPacket(s32, void *);                      /* extern */
s32 *LinkSync_GetPacket(s32);                             /* extern */
u32 sub_020390C4();                                 /* extern */
M2C_UNK sub_0203981C(u8);                           /* extern */
M2C_UNK sub_0203986C();                             /* extern */
s32 sub_0203988C();                                 /* extern */
M2C_UNK sub_020398D4(s32, s32);                     /* extern */
M2C_UNK sub_02039B38();                             /* extern */
extern M2C_UNK gSystem;

s32 ov49_02262028(s32 arg0, s32 arg1, s32 arg2) {
    M2C_UNK sp34;
    s32 sp30;
    s32 sp2C;
    u32 sp28 __attribute__((aligned(8)));
    s32 sp24;
    s32 sp20;
    s32 sp1C;
    s32 sp18;
    s32 sp14;
    s32 sp10;
    s32 *temp_r0_8;
    s32 temp_r0_2;
    s32 temp_r0_9;
    s32 temp_r5;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r2;
    s32 var_r4;
    s32 var_r6;
    s32 var_r7;
    s32 var_r7_2;
    s8 temp_r0_11;
    u32 temp_r0;
    u32 temp_r0_10;
    u32 temp_r0_3;
    u32 temp_r0_4;
    u32 temp_r0_5;
    u32 temp_r0_6;
    u32 temp_r0_7;
    u32 temp_r5_2;
    u32 temp_r7;
    u32 temp_r7_2;
    void *temp_r4;
    void *temp_r6;

    sp10 = arg0;
    sp14 = arg2;
    temp_r6 = WiFiPlazaPlayerState_GetArg();
    temp_r4 = WiFiPlazaPlayerState_GetWork(arg0);
    sp30 = WiFiPlazaRoom_GetLobby(arg1);
    temp_r0 = WiFiPlazaPlayerState_GetStep(arg0);
    switch (temp_r0) {                              /* switch 1 */
    case 9:                                         /* switch 1 */
    case 10:                                        /* switch 1 */
    case 11:                                        /* switch 1 */
    case 12:                                        /* switch 1 */
    case 13:                                        /* switch 1 */
    case 14:                                        /* switch 1 */
    case 26:                                        /* switch 1 */
    case 27:                                        /* switch 1 */
        if ((u32) M2C_FIELD(temp_r4, u16 *, 8) > ov45_0222F314(M2C_FIELD(temp_r6, u8 *, 1))) {
            M2C_FIELD(temp_r6, s8 *, 0) = 4;
            WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
        } else if ((s32) M2C_FIELD(temp_r4, u16 *, 8) > Comm_CountConnected()) {
            M2C_FIELD(temp_r6, s8 *, 0) = 4;
            WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
        } else if (sub_0203988C() == 0) {
            M2C_FIELD(temp_r6, s8 *, 0) = 4;
            WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
        } else {
        case 8:                                     /* switch 1 */
            if ((ov45_0222F464() == 1) && (ov45_0222F314(M2C_FIELD(temp_r6, u8 *, 1)) <= 1U)) {
                M2C_FIELD(temp_r6, s8 *, 0) = 4;
                WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
            } else {
                temp_r0_2 = M2C_FIELD(temp_r4, s32 *, 0x10) - 1;
                M2C_FIELD(temp_r4, s32 *, 0x10) = temp_r0_2;
                if (temp_r0_2 < 0) {
                    M2C_FIELD(temp_r6, s8 *, 0) = 4;
                    WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
                } else {
                case 7:                             /* switch 1 */
                case 17:                            /* switch 1 */
                case 18:                            /* switch 1 */
                case 19:                            /* switch 1 */
                case 20:                            /* switch 1 */
                case 21:                            /* switch 1 */
                case 22:                            /* switch 1 */
                    temp_r0_3 = sub_020390C4();
                    switch (temp_r0_3) {            /* switch 2 */
                    case 0:                         /* switch 2 */
                    case 1:                         /* switch 2 */
                        break;
                    case 2:                         /* switch 2 */
                    case 3:                         /* switch 2 */
                    case 4:                         /* switch 2 */
                    case 5:                         /* switch 2 */
                        M2C_FIELD(temp_r6, s8 *, 0) = 4;
                        WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
                        break;
                    }
                }
            }
        }
        break;
    }
    temp_r0_4 = WiFiPlazaPlayerState_GetStep(sp10);
    switch (temp_r0_4) {                            /* switch 3 */
    case 0:                                         /* switch 3 */
        WiFiPlazaPlayerState_AllocWork(sp10, 0x44);
        WiFiPlazaPlayerState_NextStep(sp10);
    default:                                        /* switch 3 */
        break;
    case 1:                                         /* switch 3 */
        if (ov45_0222A414(sp30) != 0) {
            M2C_FIELD(temp_r6, s8 *, 0) = 7;
            WiFiPlazaPlayerState_SetStep(sp10, 0x1CU);
        } else if (ov45_0222A394(sp30) != 0) {
            M2C_FIELD(temp_r6, s8 *, 0) = 2;
            WiFiPlazaPlayerState_SetStep(sp10, 0x1CU);
        } else {
            PlaySE(0x5DD);
            WiFiPlazaPlayerState_NextStep(sp10);
        }
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 2:                                         /* switch 3 */
        ov49_0225A37C(arg1, M2C_FIELD(temp_r6, u8 *, 2), 0);
        ov49_02262BF8(temp_r4, sp10, arg1, 0, 3);
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 3:                                         /* switch 3 */
        ov49_02262C38(temp_r4, arg1, 3, 1);
        ov49_0225A174(arg1, temp_r4 + 0x18, 0, 0);
        WiFiPlazaPlayerState_SetStep(sp10, 4U);
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 4:                                         /* switch 3 */
        var_r7 = 0;
        temp_r0_5 = ov49_0225A1D4(arg1);
        switch (temp_r0_5) {                        /* switch 4; irregular */
        case -2:                                    /* switch 4 */
        case 2:                                     /* switch 4 */
            M2C_FIELD(temp_r6, s8 *, 0) = 5;
            WiFiPlazaPlayerState_SetStep(sp10, 0x1CU);
            var_r7 = 1;
            break;
        case 1:                                     /* switch 4 */
            WiFiPlazaPlayerState_SetStep(sp10, 5U);
            var_r7 = 1;
            break;
        case 0:                                     /* switch 4 */
            WiFiPlazaPlayerState_SetStep(sp10, 6U);
            var_r7 = 1;
            break;
        }
        if (var_r7 == 1) {
            ov49_0225A1E4(arg1, 0, 0);
            ov49_02262CA8(temp_r4, arg1);
        }
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 5:                                         /* switch 3 */
        ov49_0225A37C(arg1, M2C_FIELD(temp_r6, u8 *, 2), 0);
        ov49_02262BF8(temp_r4, sp10, arg1, M2C_FIELD(temp_r6, u8 *, 2) + 0x22, 2);
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 6:                                         /* switch 3 */
        if (ov45_0222A394(sp30) != 0) {
            M2C_FIELD(temp_r6, s8 *, 0) = 2;
            WiFiPlazaPlayerState_SetStep(sp10, 0x1CU);
        } else if (ov45_0222F274(M2C_FIELD(temp_r6, u8 *, 1)) == 1) {
            if ((ov45_0222F2D4(M2C_FIELD(temp_r6, u8 *, 1)) == 0) || (ov45_0222F294(M2C_FIELD(temp_r6, u8 *, 1)) == 0)) {
                M2C_FIELD(temp_r6, s8 *, 0) = 1;
                WiFiPlazaPlayerState_SetStep(sp10, 0x1CU);
            } else if (ov45_0222F3E8(M2C_FIELD(temp_r6, u8 *, 1)) == 0) {
                M2C_FIELD(temp_r6, s8 *, 0) = 7;
                WiFiPlazaPlayerState_SetStep(sp10, 0x1CU);
            } else {
                goto block_46;
            }
        } else {
block_46:
            M2C_FIELD(temp_r4, s16 *, 0) = ov45_0222F3E8(M2C_FIELD(temp_r6, u8 *, 1));
            M2C_FIELD(temp_r4, s32 *, 0x10) = 0x384;
            if ((s32) M2C_FIELD(temp_r4, s16 *, 0) > 0x14A) {
                var_r0 = WiFiPlazaRoom_GetMsgString(arg1, 0, 0xA);
            } else {
                M2C_FIELD(temp_r4, s32 *, 0x14) = 1;
                var_r0 = WiFiPlazaRoom_GetMsgString(arg1, 0, 0xB);
            }
            ov49_0225A09C(arg1, var_r0);
            ov49_0225A0BC(arg1);
            sub_0203981C(M2C_FIELD(temp_r6, u8 *, 1));
            if (ov45_0222F464() == 1) {
                ov45_0222AC14(sp30, M2C_FIELD(temp_r6, u8 *, 2), 1U, sp14, 0, 0, 0, 0);
                M2C_FIELD(temp_r4, u8 *, 7) = 1U;
            }
            ov49_02262D70(temp_r4 + 0x3C, arg1, M2C_FIELD(temp_r6, u8 *, 2), 1, (s32) M2C_FIELD(temp_r4, s16 *, 0));
            ov49_02262E04(temp_r4 + 0x3C, arg1, 1);
            WiFiPlazaPlayerState_SetStep(sp10, 7U);
        }
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 7:                                         /* switch 3 */
        sp2C = sub_0203988C();
        temp_r7 = ov45_0222F314(M2C_FIELD(temp_r6, u8 *, 1));
        M2C_FIELD(temp_r4, s16 *, 0) = ov45_0222F3E8(M2C_FIELD(temp_r6, u8 *, 1));
        ov49_02262DD4(temp_r4 + 0x3C, M2C_FIELD(temp_r4, s16 *, 0));
        if (((s32) M2C_FIELD(temp_r4, s16 *, 0) <= 0x14A) && (M2C_FIELD(temp_r4, s32 *, 0x14) == 0)) {
            ov49_0225A0CC(arg1);
            M2C_FIELD(temp_r4, s32 *, 0x14) = 1;
            ov49_0225A09C(arg1, WiFiPlazaRoom_GetMsgString(arg1, 0, 0xB));
            ov49_0225A0BC(arg1);
        }
        ov49_02262DF8(temp_r4 + 0x3C, arg1, 0);
        if (sp2C == 0) {
            M2C_FIELD(temp_r6, s8 *, 0) = 4;
            WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
        } else if (sp2C == 2) {
            M2C_FIELD(temp_r6, s8 *, 0) = 8;
            if (temp_r7 == 4) {
                var_r6 = 0x10;
            } else {
                var_r6 = 0x12;
            }
            ov49_0225A0CC(arg1);
            ov49_0225A09C(arg1, WiFiPlazaRoom_GetMsgString(arg1, 0, var_r6));
            ov49_0225A0BC(arg1);
            WiFiPlazaPlayerState_SetStep(sp10, 9U);
            M2C_FIELD(temp_r4, u16 *, 8) = (u16) temp_r7;
            ov49_02262DD4(temp_r4 + 0x3C, 0);
            ov49_02262DF8(temp_r4 + 0x3C, arg1, 0);
        } else if ((M2C_FIELD(temp_r4, s32 *, 0x14) == 0) && (2 & M2C_FIELD(&gSystem, s32 *, 0x48))) {
            PlaySE(0x5DC);
            if (ov45_0222F464() == 0) {
                M2C_FIELD(temp_r6, s8 *, 0) = 6;
                ov49_0225A0CC(arg1);
                WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
            } else {
                M2C_FIELD(temp_r6, s8 *, 0) = 6;
                ov49_0225A0CC(arg1);
                WiFiPlazaPlayerState_SetStep(sp10, 0x10U);
            }
        } else if (ov45_0222F464() == 1) {
            if (M2C_FIELD(temp_r4, u8 *, 7) != temp_r7) {
                M2C_FIELD(temp_r4, u8 *, 7) = (u8) temp_r7;
                if (temp_r7 != 4) {
                    ov45_0222AC14(sp30, M2C_FIELD(temp_r6, u8 *, 2), temp_r7, sp14, 0, 0, 0, 0);
                }
            }
            if ((M2C_FIELD(temp_r4, s16 *, 0) == 0) && (ov45_0222F314(M2C_FIELD(temp_r6, u8 *, 1)) <= 1U)) {
                M2C_FIELD(temp_r6, s8 *, 0) = 3;
                WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
            } else {
                goto block_75;
            }
        } else {
block_75:
            if ((ov45_0222F274(M2C_FIELD(temp_r6, u8 *, 1)) == 1) && (ov45_0222F294(M2C_FIELD(temp_r6, u8 *, 1)) == 0)) {
                M2C_FIELD(temp_r6, s8 *, 0) = 0;
                WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
            } else if (M2C_FIELD(temp_r4, s16 *, 0) == 0) {
                WiFiPlazaPlayerState_SetStep(sp10, 8U);
                ov49_0225A0CC(arg1);
                ov49_0225A09C(arg1, WiFiPlazaRoom_GetMsgString(arg1, 0, 0x12));
                ov49_0225A0BC(arg1);
            }
        }
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 8:                                         /* switch 3 */
        ov49_02262DD4(temp_r4 + 0x3C, 0);
        ov49_02262DF8(temp_r4 + 0x3C, arg1, 0);
        temp_r5 = sub_0203988C();
        temp_r7_2 = ov45_0222F314(M2C_FIELD(temp_r6, u8 *, 1));
        temp_r0_6 = sub_020390C4();
        switch (temp_r0_6) {
        case 3:
        case 4:
            M2C_FIELD(temp_r6, s8 *, 0) = 4;
            WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
            break;
        }
        if (temp_r5 == 0) {
            M2C_FIELD(temp_r6, s8 *, 0) = 4;
            WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
        } else if (temp_r5 == 2) {
            M2C_FIELD(temp_r6, s8 *, 0) = 8;
            WiFiPlazaPlayerState_SetStep(sp10, 9U);
            M2C_FIELD(temp_r4, u16 *, 8) = (u16) temp_r7_2;
        } else if ((ov45_0222F274(M2C_FIELD(temp_r6, u8 *, 1)) != 1) || (ov45_0222F294(M2C_FIELD(temp_r6, u8 *, 1)) != 0)) {

        } else {
            M2C_FIELD(temp_r6, s8 *, 0) = 0;
            WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
        }
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 9:                                         /* switch 3 */
        ov49_02262DD4(temp_r4 + 0x3C, 0);
        ov49_02262DF8(temp_r4 + 0x3C, arg1, 0);
        ov45_0222A5E8(WiFiPlazaRoom_GetLobby(arg1), M2C_FIELD(temp_r6, u8 *, 3));
        sub_02039B38();
        WiFiPlazaRoom_GetLobby(arg1);
        CommPlayerInfo_Init(ov45_0222A2C8(), 0);
        ov45_0222AB1C(sp30);
        CommPlayerInfo_SetProfile();
        sub_020378E4(0);
        ov49_02262C20(temp_r4, sp10, 0xA, 0x11);
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 10:                                        /* switch 3 */
        ov49_02262DD4(temp_r4 + 0x3C, 0);
        ov49_02262DF8(temp_r4 + 0x3C, arg1, 0);
        CommPlayerInfo_SendLocal();
        Comm_GetLocalNetId();
        CommPlayerInfo_Accept();
        WiFiPlazaPlayerState_SetStep(sp10, 0xBU);
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 11:                                        /* switch 3 */
        ov49_02262DD4(temp_r4 + 0x3C, 0);
        ov49_02262DF8(temp_r4 + 0x3C, arg1, 0);
        if (CommPlayerInfo_FindReceived() != 0xFF) {
            do {
                CommPlayerInfo_Accept();
            } while (CommPlayerInfo_FindReceived() != 0xFF);
        }
        temp_r5_2 = CommPlayerInfo_CountAccepted();
        if (temp_r5_2 >= ov45_0222F314(M2C_FIELD(temp_r6, u8 *, 1))) {
            if ((ov45_0222F464() == 1) && (M2C_FIELD(temp_r4, u16 *, 2) == 0)) {
                ov45_0222F1BC();
                M2C_FIELD(temp_r4, u16 *, 2) = 1U;
            }
            if (ov45_0222F218() == 1) {
                WiFiPlazaPlayerState_SetStep(sp10, 0xCU);
            }
        } else if (ov45_0222F218() == 1) {
            M2C_FIELD(temp_r6, s8 *, 0) = 4;
            WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
        }
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 12:                                        /* switch 3 */
        ov49_02262DD4(temp_r4 + 0x3C, 0);
        ov49_02262DF8(temp_r4 + 0x3C, arg1, 0);
        LinkSync_ClearPackets();
        ov45_0222A43C(sp30);
        ov49_02262C20(temp_r4, sp10, 0xD, 0xE);
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 13:                                        /* switch 3 */
        ov49_02262DD4(temp_r4 + 0x3C, 0);
        ov49_02262DF8(temp_r4 + 0x3C, arg1, 0);
        M2C_FIELD(temp_r4, s32 *, 0xC) = ov45_0222A548(sp30);
        if (LinkSync_SendPacket(Comm_GetLocalNetId(), temp_r4 + 0xC) != 1) {

        } else {
            WiFiPlazaPlayerState_SetStep(sp10, 0xEU);
        }
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 14:                                        /* switch 3 */
        ov49_02262DD4(temp_r4 + 0x3C, 0);
        ov49_02262DF8(temp_r4 + 0x3C, arg1, 0);
        sp18 = 0;
        temp_r0_7 = CommPlayerInfo_CountAccepted();
        sp28 = temp_r0_7;
        sp24 = Comm_GetLocalNetId();
        var_r7_2 = 0;
        sp20 = var_r7_2;
        sp1C = ov45_0222F430();
        var_r4 = var_r7_2;
        if ((s32) temp_r0_7 > 0) {
loop_108:
            if (sp24 != var_r4) {
                temp_r0_8 = LinkSync_GetPacket(var_r4);
                if (temp_r0_8 != 0) {
                    if (ov45_0222EC68(*temp_r0_8) == -1) {
                        sp18 = 1;
                        M2C_FIELD(temp_r6, s8 *, 0) = 4;
                        WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
                    } else {
                        ov45_0222A450(sp30, *temp_r0_8, var_r4);
                        if (*temp_r0_8 == sp1C) {
                            sp20 = 1;
                        }
                        var_r7_2 += 1;
                        goto block_118;
                    }
                } else {
                    goto block_118;
                }
            } else {
                ov45_0222A480(sp30, var_r4);
                if (ov45_0222F464() == 1) {
                    sp20 = 1;
                }
                var_r7_2 += 1;
block_118:
                var_r4 += 1;
                if (var_r4 < (s32) sp28) {
                    goto loop_108;
                }
            }
        }
        if ((sp18 == 0) && (var_r7_2 == sp28)) {
            if (sp20 == 1) {
                WiFiPlazaPlayerState_SetStep(sp10, 0xFU);
            } else {
                M2C_FIELD(temp_r6, s8 *, 0) = 4;
                WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
            }
        }
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 15:                                        /* switch 3 */
        sub_020398D4(0, 1);
        if (ov45_0222F464() == 1) {
            ov45_0222A498(sp30, &sp34);
            ov45_0222AC14(sp30, M2C_FIELD(temp_r6, u8 *, 2), ov45_0222F314(M2C_FIELD(temp_r6, u8 *, 1)), (s32) M2C_FIELD(&sp34, u8 *, 0), (s32) M2C_FIELD(&sp34, u8 *, 1), (s32) M2C_FIELD(&sp34, u8 *, 2), (s32) M2C_FIELD(&sp34, u8 *, 3), 1);
        }
        ov49_0225A0CC(arg1);
        ov49_02262C20(temp_r4, sp10, 0x1C, 0x12);
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 16:                                        /* switch 3 */
        WiFiPlazaRoom_PrintMessage(arg1, WiFiPlazaRoom_GetMsgString(arg1, 0, 0x1A));
        WiFiPlazaPlayerState_SetStep(sp10, 0x11U);
        ov49_02262DB8(temp_r4 + 0x3C, arg1);
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 17:                                        /* switch 3 */
        if ((ov49_02262CB4(temp_r4, arg1, temp_r6, sp10, sp14) == 0) && (WiFiPlazaRoom_IsMessageFinished(arg1) == 1)) {
            WiFiPlazaPlayerState_SetStep(sp10, 0x12U);
        }
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 18:                                        /* switch 3 */
        ov49_0225A294(arg1);
        WiFiPlazaPlayerState_SetStep(sp10, 0x13U);
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 19:                                        /* switch 3 */
        temp_r0_9 = WiFiPlazaRoom_GetYesNoResult(arg1);
        switch (temp_r0_9) {                        /* switch 5; irregular */
        case 0:                                     /* switch 5 */
            WiFiPlazaRoom_CloseYesNoMenu(arg1);
            WiFiPlazaPlayerState_SetStep(sp10, 0x17U);
            ov45_0222A404(sp30);
            break;
        case 1:                                     /* switch 5 */
            WiFiPlazaRoom_CloseYesNoMenu(arg1);
            WiFiPlazaPlayerState_SetStep(sp10, 0x14U);
            break;
        case 2:                                     /* switch 5 */
            if (ov49_02262CB4(temp_r4, arg1, temp_r6, sp10, sp14) != 1) {

            } else {
                WiFiPlazaRoom_CloseYesNoMenu(arg1);
            }
            break;
        }
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 20:                                        /* switch 3 */
        M2C_FIELD(temp_r4, s16 *, 0) = ov45_0222F3E8(M2C_FIELD(temp_r6, u8 *, 1));
        ov49_02262D70(temp_r4 + 0x3C, arg1, M2C_FIELD(temp_r6, u8 *, 2), 1, (s32) M2C_FIELD(temp_r4, s16 *, 0));
        ov49_02262E04(temp_r4 + 0x3C, arg1, 1);
        if ((s32) M2C_FIELD(temp_r4, s16 *, 0) > 0x14A) {
            var_r0_2 = WiFiPlazaRoom_GetMsgString(arg1, 0, 0xA);
        } else {
            M2C_FIELD(temp_r4, s32 *, 0x14) = 1;
            var_r0_2 = WiFiPlazaRoom_GetMsgString(arg1, 0, 0xB);
        }
        ov49_0225A09C(arg1, var_r0_2);
        ov49_0225A0BC(arg1);
        WiFiPlazaPlayerState_SetStep(sp10, 7U);
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 21:                                        /* switch 3 */
        temp_r0_10 = ov45_0222F314(M2C_FIELD(temp_r6, u8 *, 1));
        M2C_FIELD(temp_r6, s8 *, 0) = 8;
        if (temp_r0_10 == 4) {
            var_r2 = 0x10;
        } else {
            var_r2 = 0x12;
        }
        ov49_0225A09C(arg1, WiFiPlazaRoom_GetMsgString(arg1, 0, var_r2));
        ov49_0225A0BC(arg1);
        WiFiPlazaPlayerState_SetStep(sp10, 9U);
        ov49_02262D70(temp_r4 + 0x3C, arg1, M2C_FIELD(temp_r6, u8 *, 2), 1, 0);
        ov49_02262E04(temp_r4 + 0x3C, arg1, 1);
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 22:                                        /* switch 3 */
        ov49_0225A09C(arg1, WiFiPlazaRoom_GetMsgString(arg1, 0, 0x12));
        ov49_0225A0BC(arg1);
        ov49_02262D70(temp_r4 + 0x3C, arg1, M2C_FIELD(temp_r6, u8 *, 2), 1, 0);
        ov49_02262DD4(temp_r4 + 0x3C, 0);
        ov49_02262DF8(temp_r4 + 0x3C, arg1, 1);
        WiFiPlazaPlayerState_SetStep(sp10, 8U);
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 23:                                        /* switch 3 */
        CommPlayerInfo_Free();
        sub_0203986C();
        WiFiPlazaPlayerState_SetStep(sp10, 0x18U);
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 24:                                        /* switch 3 */
        if (sub_0203988C() == 0) {
            WiFiPlazaPlayerState_SetStep(sp10, 0x1CU);
        }
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 25:                                        /* switch 3 */
        if (WiFiPlazaRoom_IsMessageFinished(arg1) == 1) {
            WiFiPlazaPlayerState_SetStep(sp10, M2C_FIELD(temp_r4, u8 *, 4));
        }
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 26:                                        /* switch 3 */
        temp_r0_11 = M2C_FIELD(temp_r4, s8 *, 5);
        if ((s32) temp_r0_11 > 0) {
            M2C_FIELD(temp_r4, s8 *, 5) = (s8) (temp_r0_11 - 1);
        }
        if (M2C_FIELD(temp_r4, s8 *, 5) == 0) {
            LinkSync_StartSync(M2C_FIELD(temp_r4, u8 *, 6));
            WiFiPlazaPlayerState_SetStep(sp10, 0x1BU);
        }
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 27:                                        /* switch 3 */
        M2C_FIELD(temp_r4, u16 *, 0xA) = (u16) (M2C_FIELD(temp_r4, u16 *, 0xA) + 1);
        if ((u32) M2C_FIELD(temp_r4, u16 *, 0xA) >= 0x96U) {
            LinkSync_StartSync(M2C_FIELD(temp_r4, u8 *, 6));
            M2C_FIELD(temp_r4, u16 *, 0xA) = 0U;
        }
        if (LinkSync_IsSynced(M2C_FIELD(temp_r4, u8 *, 6)) != 0) {
            WiFiPlazaPlayerState_SetStep(sp10, M2C_FIELD(temp_r4, u8 *, 4));
        }
        /* Duplicate return node #164. Try simplifying control flow for better match */
        break;
    case 28:                                        /* switch 3 */
        WiFiPlazaRoom_CloseMessage(arg1);
        ov49_02262CA8(temp_r4, arg1);
        ov49_02262DB8(temp_r4 + 0x3C, arg1);
        WiFiPlazaRoom_CloseYesNoMenu(arg1);
        WiFiPlazaPlayerState_FreeWork(sp10);
        return 1;
     }
    return 0;
}
