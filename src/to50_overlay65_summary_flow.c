#include "to50_overlay65_summary_flow_private.h"

void ov65_0221E06C(void *work, u32 mode) {
    if (mode == 0) {
        PTR(work, 0xC) = PTR(work, 0x2224);
        U8(work, 0x1F) = Party_GetCount(PTR(PTR(work, 8), 8));
        PTR(work, 0x34) = NULL;
        sub_0208AD34((u8 *)work + 0xC, sub_02034818(Comm_GetLocalNetId()));
    } else {
        PTR(work, 0xC) = PTR(work, 0x2228);
        U8(work, 0x1F) = Party_GetCount(PTR(work, 0x2228));
        PTR(work, 0x34) = (u8 *)work + 0x2E20 + (Comm_GetLocalNetId() ^ 1) * 0x3EC;
        sub_0208AD34((u8 *)work + 0xC, sub_02034818(Comm_GetLocalNetId() ^ 1));
    }

    U8(work, 0x1D) = 1;
    U8(work, 0x20) = S32(work, 0x94) % 6;
    U8(work, 0x1E) = 1;
    U16(work, 0x24) = 0;
    PTR(work, 0x38) = sub_02088288(PTR(PTR(work, 8), 0x10));
    U32(work, 0x28) = U32(PTR(work, 8), 0x2C);
    U32(work, 0x10) = U32(PTR(work, 8), 0x18);
    PTR(work, 0x2C) = Save_SpecialRibbons_Get(PTR(PTR(work, 8), 0x10));
    sub_02089D40((u8 *)work + 0xC, _0221FD34);
    PTR(work, 0x4C) = OverlayManager_New(gOverlayTemplate_PokemonSummary, (u8 *)work + 0xC, 0x1A);
    U32(work, 0x48) = mode;
}

int ov65_0221E144(void *work) {
    FillBgTilemapRect(PTR(work, 0x180), 0, 0, 0, 0, 0x20, 0x18, 0);
    ov65_0221FB90((u8 *)work + 0x5B4, 0x19, 1, U32(work, 0x190), PTR(work, 0x184));
    PTR(work, 0x2220) = ov65_0221E194;
    return 0;
}

int ov65_0221E194(void *work) {
    switch (ov65_0221FC54(PTR(work, 0x180), (u8 *)work + 0x664, (u8 *)work + 0x670, (u8 *)work + 0x36C0, 1)) {
    case 1:
    case 3:
        FillBgTilemapRect(PTR(work, 0x180), 0, 0, 0, 0, 0x20, 0x18, 0);
        ov65_0221FB90((u8 *)work + 0x5B4, 0x1C, 1, U32(work, 0x190), PTR(work, 0x184));
        ov65_0221DE24(work, 0x18, 1);
        PTR(work, 0x2220) = ov65_0221E8EC;
        U32(work, 0x68) = 0;
        break;
    case 2:
    case 4:
        FillBgTilemapRect(PTR(work, 0x180), 0, 0, 0, 0, 0x20, 0x18, 0);
        ov65_0221FB90((u8 *)work + 0x594, 0xF, 1, U32(work, 0x190), PTR(work, 0x184));
        PTR(work, 0x2220) = ov65_0221E050;
        break;
    }
    return 0;
}

int ov65_0221E270(void *work) {
    FillBgTilemapRect(PTR(work, 0x180), 0, 0, 0, 0, 0x20, 0x18, 0);
    ov65_0221FB90((u8 *)work + 0x5B4, U32(work, 0x74), 1, U32(work, 0x190), PTR(work, 0x184));
    PTR(work, 0x2220) = ov65_0221E2C0;
    return 0;
}

int ov65_0221E2C0(void *work) {
    if ((U32(gSystem, 0x48) & 1) || U16(gSystem, 0x64) != 0) {
        FillBgTilemapRect(PTR(work, 0x180), 0, 0, 0, 0, 0x20, 0x18, 0);
        ov65_0221FB90((u8 *)work + 0x594, 0xF, 1, U32(work, 0x190), PTR(work, 0x184));
        PTR(work, 0x2220) = ov65_0221E050;
        ov65_0221EF0C(work);
    }
    return 0;
}
