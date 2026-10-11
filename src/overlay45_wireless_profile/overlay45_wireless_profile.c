#include "private.h"

void ov45_0222B840(Ov45State *state) {
    if (((Ov45R15Flags *)&state->data[0x1fc])->bits.bit0 == 1 && ((Ov45R15Flags *)&state->data[0x1fc])->bits.bit7 == 0 && ov45_0222E96C() == 2) {
        s64 value = (u32)ov45_0222AD4C(state);
        if (value + *(s64 *)&state->data[0xe0] < *(s64 *)&state->data[0xd8]) {
            state->data[0x1fc] |= 0x80;
            *(u16 *)&state->data[0x200] = 0x384;
        }
    }
}

void WiFiPlazaProfile_InitFromSave(u8 *dst, void *save, int heapId) {
    void *profile;
    void *party;
    void *history;
    void *pokedex;
    void *rtc;
    void *string;
    int count;
    int i;
    int j;
    u8 *speciesDst;
    u8 *scoreDst;
    u8 *halfDst;
    profile = Save_PlayerData_GetProfile(save);
    party = SaveArray_Party_Get(save);
    pokedex = Save_Pokedex_Get(save);
    history = Save_WiFiHistory_Get(save);
    rtc = Save_SysInfo_RTC_Get(save);
    string = PlayerProfile_GetPlayerName_NewString(profile, heapId);
    CopyStringToU16Array(string, (u16 *)&dst[0x28], 8);
    CopyStringToU16Array(string, (u16 *)dst, 8);
    String_Delete(string);
    *(s32 *)&dst[0x20] = -1;
    *(u32 *)&dst[0x24] = PlayerProfile_GetTrainerID(profile);
    count = Party_GetCount(party);
    i = 0;
    speciesDst = dst;
    while (i < 6) {
        if (i < count) {
            void *mon = Party_GetMonByIndex(party, i);
            *(u16 *)&speciesDst[0x40] = GetMonData(mon, 5, NULL);
            dst[0x4c + i] = GetMonData(mon, 0x70, NULL);
            dst[0x52 + i] = GetMonData(mon, 0x4c, NULL);
        } else {
            *(u16 *)&speciesDst[0x40] = 0x1ef;
        }
        i++;
        speciesDst += 2;
    }
    dst[0x58] = PlayerProfile_GetTrainerGender(profile);
    dst[0x59] = PlayerProfile_GetLanguage(profile);
    *(u16 *)&dst[0x5a] = PlayerProfile_GetAvatar(profile);
    *(u16 *)&dst[0x5a] = ov45_0222CCE4(*(u16 *)&dst[0x5a]);
    *(u16 *)&dst[0x5c] = WifiHistory_GetPlayerCountry(history);
    dst[0x5e] = WiFiHistory_GetPlayerRegion(history);
    dst[0x5f] = Pokedex_GetNatDexFlag(pokedex);
    dst[0x60] = PlayerProfile_GetGameClearFlag(profile);
    dst[0x61] = 0xff;
    dst[0x63] = 0;
    dst[0x62] = GAME_VERSION;
    *(u64 *)&dst[0x64] = *(u64 *)&((u8 *)rtc)[0x24];
    scoreDst = dst;
    for (i = 0; i < 12; i++) {
        dst[0x6c + i] = 0x18;
        *(s32 *)&scoreDst[0x78] = -1;
        scoreDst += 4;
    }
    halfDst = dst;
    for (j = 0; j < 2; j++) {
        *(u16 *)&halfDst[0xa8] = 0;
        halfDst += 2;
    }
    *(s32 *)&dst[0xac] = -1;
    *(u32 *)&dst[0xb0] = 3;
    WiFiPlazaProfile_UpdateChecksum(dst, save);
}

void ov45_0222BA3C(Ov45State *state) {
    int i;
    u8 *p = state->data;
    int offset = 0x4bc;
    u8 *dst;
    if (WiFiPlazaProfile_IsChecksumValid(&p[0x108], *(void **)p) == 0) {
        *(u32 *)&p[0x52c] = 1;
        return;
    }
    i = 0;
    dst = p;
    do {
        p[0x174 + i] = ov45_0222CC50(&p[offset], i);
        *(u32 *)&dst[0x180] = ov45_0222CC7C(&p[offset], i);
        i++;
        dst += 4;
    } while (i < 12);
    MI_CpuCopy8(&p[0x118], &p[0x130], 16);
    ov45_0222E9F8(&p[0x128]);
    MI_CpuCopy8(&p[0x108], &p[0x130], 16);
    WiFiPlazaProfile_UpdateChecksum(&p[0x108], *(void **)p);
}
