#include "to54_atomic_w20_to54_overlay112_walker_helpers_private.h"

int ov112_021EE8BC(void *boxmon, void *mon, int exp) {
    BOOL leveled = FALSE;

    if (GetBoxMonData(boxmon, 0xA1, NULL) < 100) {
        int limit;
        if (exp < 0) {
            exp = 0;
        }
        limit = (int)CalcBoxMonExpToNextLevel(boxmon);
        if (exp >= limit) {
            exp = limit;
            leveled = TRUE;
        }
        {
            u32 value = GetBoxMonData(boxmon, 8, NULL);
            value += exp;
            CopyBoxPokemonToPokemon(boxmon, mon);
            SetMonData(mon, 8, &value);
        }
        CalcMonLevelAndStats(mon);
        if (leveled) {
            MonApplyFriendshipMod(mon, 0, 8);
        }
    }
    return leveled;
}

void ov112_021EE920(Ov112State *p) {
    U16_AT(p, 0x1F376) = 0;
    if (FLAG_AT(p, 0x9D78).bits.bit0 && sub_02032764(PTR_AT(p, 0x1E440)) == 0) {
        sub_0203276C(PTR_AT(p, 0x1E440));
        Bag_AddItem(PTR_AT(p, 0x1E434), 0xCF, 1, 0x9A);
        U16_AT(p, 0x1F376) = 1;
    }
}

void ov112_021EE970(void *boxmon, int exp) {
    u8 level;
    if (exp >= 5000) {
        level = GetBoxMonData(boxmon, 9, NULL);
        if (level < 255) {
            level++;
            SetBoxMonData(boxmon, 9, &level);
        }
    }
}

void ov112_021EE9A4(void *mon, void *profile, const Ov112GiftMon *gift, u32 nature, u32 mode) {
    void *name = PlayerProfile_GetPlayerName_NewString(profile, 0x9A);
    u32 id = PlayerProfile_GetTrainerID(profile);
    ov112_021EE9E4(mon, id, gift, nature, mode);
    SetMonData(mon, 0x91, name);
    String_Delete(name);
}

void ov112_021EE9E4(void *mon, u32 trainerId, const Ov112GiftMon *gift, u32 nature, u32 mode) {
    u8 form = gift->flags.form;
    u8 gender = 0;

    if (gift->flags.gender == 1) {
        gender = 0xFE;
    }
    {
        u32 personality = ChangePersonalityToNatureGenderAndAbility(trainerId, gift->species, (u8)nature, gender, nature & 1, mode);
        CreateMon(mon, gift->species, gift->level, 0x20, 1, personality, 1, trainerId);
    }
    SetMonData(mon, 6, &gift->moves[0]);
    SetMonData(mon, 0x36, &gift->moves[1]);
    SetMonData(mon, 0x37, &gift->moves[2]);
    SetMonData(mon, 0x38, &gift->moves[3]);
    SetMonData(mon, 0x39, &gift->unkA);
    SetMonData(mon, 0x70, &form);
}

void ov112_021EEA7C(Ov112State *p) {
    int i = 0;
    Ov112State *it = p;
    for (; i < 3; i++) {
        u16 item = U16_AT(it, 0x9DAC);
        if (item != 0) {
            Bag_AddItem(PTR_AT(p, 0x1E434), item, 1, 0x9A);
        }
        it = (Ov112State *)((u8 *)it + 4);
    }
    i = 0;
    it = p;
    for (; i < 10; i++) {
        u16 item = U16_AT(it, 0x9DB8);
        if (item != 0) {
            Bag_AddItem(PTR_AT(p, 0x1E434), item, 1, 0x9A);
        }
        it = (Ov112State *)((u8 *)it + 4);
    }
    if (((u32)U8_AT(p, 0xAABC) << 25) >> 31) {
        Bag_AddItem(PTR_AT(p, 0x1E434), U16_AT(p, 0xB002), 1, 0x9A);
    }
}
