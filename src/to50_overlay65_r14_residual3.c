#include "to50_overlay65_r14_residual3_private.h"

#define U16_AT_LOCAL(p, o) (*(u16 *)((u8 *)(p) + (o)))
typedef struct WorkHeadR3 {
    u8 pad[0x2224];
    void *partyA;
    void *partyB;
} WorkHeadR3;

BOOL ov65_0221C5E0(void *work) {
    int i;

    ov65_0221F864(work);
    switch (S32_AT(work, 0x58)) {
    case 0:
        sub_02037AC0(0x50);
        sub_0201A728(2);
        ov65_0221F760(work);
        i = 0;
        if (Party_GetCount(PTR_AT(PTR_AT(work, 8), 8)) > 0) {
            do {
                Pokemon *mon = Party_GetMonByIndex(PTR_AT(PTR_AT(work, 8), 8), i);
                if (GetMonData(mon, 0xae, NULL) == 492 && GetMonData(mon, 0x70, NULL) != 0) {
                    Mon_UpdateShayminForm(mon, 0);
                }
                i++;
            } while (i < Party_GetCount(PTR_AT(PTR_AT(work, 8), 8)));
        }
        S32_AT(work, 0x58)
        ++;
        break;
    case 1:
        if (sub_02037B38(0x50)) {
            if (PTR_AT(PTR_AT(work, 8), 0x30) == NULL) {
                S32_AT(work, 0x58) = 6;
            } else {
                S32_AT(work, 0x58) = 2;
            }
            if (Comm_GetLocalNetId() == 0) {
                u16 value = (s32)LCRandom() % 60;
                ov65_0221DE10(Comm_GetLocalNetId(), 0x1f, value + 3);
            }
            ov65_0221DE8C(PTR_AT(work, 4));
            ov65_0221F850(work);
        }
        break;
    case 2:
        if (S32_AT(work, 0x22cc) != 0) {
            S32_AT(work, 0x58)
            ++;
        }
        break;
    case 3:
        S32_AT(work, 0x22cc)
        --;
        if (S32_AT(work, 0x22cc) == 0) {
            S32_AT(work, 0x58) = 4;
        }
        break;
    case 4:
        sub_02039EAC((u8 *)work + 0x36a8);
        S32_AT(work, 0x58)
        ++;
        break;
    case 5:
        if (sub_02039EB4(PTR_AT(work, 4), 2, (u8 *)work + 0x36a8)) {
            S32_AT(work, 0x58)
            ++;
        }
        break;
    case 6:
        sub_020378E4(1);
        sub_02037AC0(0x51);
        S32_AT(work, 0x58)
        ++;
        break;
    case 7:
        if (sub_02037B38(0x51)) {
            S32_AT(work, 0x58)
            ++;
        }
        break;
    case 8:
        S32_AT(work, 0x5c) = 0;
        S32_AT(work, 0x60) = 0;
        S32_AT(work, 0x64) = 0;
        if (Comm_GetLocalNetId() == 1) {
            ov65_0221DE64(Comm_GetLocalNetId(), PTR_AT(work, 0x2224), S32_AT(work, 0x5c));
            S32_AT(work, 0x5c)
            ++;
        }
        S32_AT(work, 0x58)
        ++;
        break;
    case 9:
        if (S32_AT(work, 0x60) != 0) {
            S32_AT(work, 0x58) = 10;
        }
        break;
    case 10:
        S32_AT(work, 0x58)
        ++;
        ov65_0221C1C4(work);
        break;
    case 11: {
        int j;
        u16 value;
        u8 *record;
        record = work;
        j = 0;
        value = j;
        do {
            j++;
            U16_AT_LOCAL(record, 0x69c) = value;
            record += 16;
        } while (j < 13);
        i = value;
    }
        if (Party_GetCount(((WorkHeadR3 *)work)->partyA) > 0) {
            Ov65MonSummary *summary = (Ov65MonSummary *)((u8 *)work + 0x69c);
            do {
                ov65_0221C9D8(Party_GetMonByIndex(((WorkHeadR3 *)work)->partyA, i), summary);
                summary++;
                i++;
            } while (i < Party_GetCount(PTR_AT(work, 0x2224)));
        }
        i = 0;
        if (Party_GetCount(((WorkHeadR3 *)work)->partyB) > 0) {
            do {
                ov65_0221C9D8(Party_GetMonByIndex(((WorkHeadR3 *)work)->partyB, i), (Ov65MonSummary *)((u8 *)work + 0x69c + (i + 6) * 16));
                i++;
            } while (i < Party_GetCount(PTR_AT(work, 0x2228)));
        }
        U16_AT_LOCAL(work, 0x75c) = 1;
        S32_AT(work, 0x58)
        ++;
        break;
    case 12:
        ov65_0221DEA0(sub_02034818(Comm_GetLocalNetId()), PTR_AT(work, 0x2230), (u8 *)work + 0x2234);
        S32_AT(work, 0x58)
        ++;
        break;
    case 13:
        if (S32_AT(work, 0x60) == 3) {
            S32_AT(work, 0x58)
            ++;
        }
        break;
    case 14:
        ov65_0221DF0C(Save_Chatot_Get(PTR_AT(work, 4)));
        S32_AT(work, 0x58)
        ++;
        break;
    case 15:
        if (S32_AT(work, 0x60) == 4) {
            S32_AT(work, 0x58)
            ++;
            GfGfx_EngineATogglePlanes(0x10, 0);
            ov65_0221C46C(PTR_AT(work, 0x2224), 0, work);
            ov65_0221C46C(PTR_AT(work, 0x2228), 6, work);
            for (i = 0; i < 2; i++, work = (u8 *)work + 4) {
                Sprite_SetDrawFlag(PTR_AT(work, 0x344), TRUE);
            }
        }
        break;
    case 16:
        StartBrightnessTransition(8, 0, -16, 0x1e, 1);
        ToggleBgLayer(1, 1);
        ToggleBgLayer(2, 1);
        ToggleBgLayer(3, 1);
        GfGfx_EngineATogglePlanes(0x10, 1);
        S32_AT(work, 0x58)
        ++;
        break;
    case 17:
        if (IsBrightnessTransitionActive(1)) {
            PTR_AT(work, 0x211c) = ov65_0221D57C(0, Party_GetMonByIndex(PTR_AT(work, 0x2224), 0), (u8 *)work + 0x7fc, (u8 *)work + 0x20fc);
            ov65_0221D674((u8 *)work + 0x444, 0, PTR_AT(work, 0x2224), 0, work);
            S32_AT(work, 0x58)
            ++;
        }
        break;
    case 18:
        StartBrightnessTransition(8, 0, -16, 0x17, 2);
        ToggleBgLayer(4, 1);
        ToggleBgLayer(5, 1);
        ToggleBgLayer(6, 1);
        GfGfx_EngineBTogglePlanes(0x10, 1);
        S32_AT(work, 0x58)
        ++;
        ov65_0221F780(work);
        break;
    case 19:
        if (IsBrightnessTransitionActive(2)) {
            ClearFrameAndWindow2((u8 *)work + 0x5b4, 1);
            FillBgTilemapRect(PTR_AT(work, 0x180), 0, 0, 0, 0, 0x20, 0x18, 0);
            ov65_0221FB90((u8 *)work + 0x594, 0xf, 1, PTR_AT(work, 0x190), PTR_AT(work, 0x184));
            sub_0201A738(2);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
