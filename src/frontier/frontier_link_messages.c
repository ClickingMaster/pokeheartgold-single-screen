#include "frontier/frontier_link_messages.h"

#include "global.h"

#include "frontier/overlay_80_022340E8.h"
#include "frontier/overlay_80_02238034.h"

#include "heap.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "frontier_save_access.h"
#include "unk_02035900.h"
#include "unk_0205BFF0.h"

// Link messages of the Battle Frontier facilities' script side in multi battles with a friend (wireless): for each
// facility a sender (packs the local data into the work's link buffer and sends it) and a receive callback
// (registered in the command table in asm/unk_02096910.s, entries 0x16 onwards). Every receive callback counts the
// messages it got, also its own echo, and ignores its own data; most only take the partner's data when the local
// player is not the parent (net id 0), whose data wins.
//   Battle Factory 0x16-0x1C, Battle Hall 0x21-0x26, Battle Castle 0x2A-0x30, Battle Arcade 0x41-0x46.
// The facilities' script work structs are defined by their own modules; the views below only name the fields the
// messages use.

#define LINK_MON_BUFFER_SIZE 0x200

// Partner Pokemon are sent as raw Pokemon structs through a large-data command (up to 0x200 bytes).
BOOL sub_02036FD8(int command, void *data, int size);
u32 ov80_02237B24(u8 type, int a1);
u32 ov80_02236DD4(u8 type);

// Battle Factory script work (view).
struct FactoryLinkWork {
    u8 filler_000[4];
    u8 type;                     // 0x004
    u8 filler_005[3];
    u16 unk_008;                 // 0x008
    u8 filler_00A[2];
    u16 unk_00C;                 // 0x00C
    u16 unk_00E;                 // 0x00E
    u8 filler_010[8];
    u16 trainerIds[14];          // 0x018
    u8 filler_034[0x220];
    u16 rentalSetIds[6];            // 0x254
    u8 rentalIVs[6];                // 0x260
    u8 filler_266[2];
    u32 rentalPersonalities[6];     // 0x268
    u8 filler_280[0x152];
    u16 opponentSetIds[4];       // 0x3D2
    u8 opponentIVs[4];           // 0x3DA
    u8 filler_3DE[2];
    u32 opponentPersonalities[4]; // 0x3E0
    u8 filler_3F0[0xE4];
    Party *party;                // 0x4D4
    u8 filler_4D8[0x10];
    u16 partySetIds[4];          // 0x4E8
    u8 filler_4F0[8];
    SaveData *saveData;          // 0x4F8
    u8 filler_4FC[8];
    u16 linkBuffer[30];          // 0x504
    u8 filler_540[0x3C];
    u8 unk_57C;                  // 0x57C
    u8 unk_57D;                  // 0x57D
    u16 unk_57E;                 // 0x57E
    u16 unk_580;                 // 0x580
    u16 unk_582;                 // 0x582
    u16 partnerSetIds[6];        // 0x584
    u8 partnerIVs[8];            // 0x590
    u32 partnerPersonalities[6]; // 0x598
    u8 filler_5B0[0x152];
    u8 linkMessageCount;         // 0x702
};

// Battle Hall script work (view).
struct HallLinkWork {
    u8 filler_000[0x18];
    u16 unk_018[20];                        // 0x018
    u8 filler_040[0x220];
    u8 partySlot;                           // 0x260
    u8 filler_261[7];
    u16 unk_268[20];                        // 0x268
    u8 filler_290[0x46C];
    SaveData *saveData;                     // 0x6FC
    u8 filler_700[0x2C];
    u16 linkBuffer[22];                     // 0x72C
    u8 filler_758[0x2C];
    u8 linkMonBuffer[LINK_MON_BUFFER_SIZE]; // 0x784
    u8 linkRecvMon[2][LINK_MON_BUFFER_SIZE]; // 0x984
    u8 filler_D84[4];
    u8 unk_D88;                             // 0xD88
    u8 filler_D89[3];
    Pokemon *partnerMon;                    // 0xD8C
    u8 linkMessageCount;                    // 0xD90
};

// Battle Castle script work (view).
struct CastleLinkWork {
    u8 filler_000[4];
    SaveData *saveData;                     // 0x004
    u8 filler_008[8];
    u8 type;                                // 0x010
    u8 filler_011[3];
    u16 unk_014;                            // 0x014
    u16 unk_016;                            // 0x016
    u8 filler_018[0x10];
    Party *party;                           // 0x028
    u8 filler_02C[4];
    u16 trainerIds[14];                     // 0x030
    u8 filler_04C[0x220];
    u16 opponentSetIds[4];                  // 0x26C
    u8 opponentIVs[4];                      // 0x274
    u32 opponentPersonalities[4];           // 0x278
    u8 filler_288[0x138];
    u16 linkBuffer[40];                     // 0x3C0
    u8 linkMonBuffer[LINK_MON_BUFFER_SIZE]; // 0x410
    u8 linkRecvMon[2][LINK_MON_BUFFER_SIZE]; // 0x610
    u8 unk_A10;                             // 0xA10
    u8 unk_A11;                             // 0xA11
    u16 unk_A12;                            // 0xA12
    u8 filler_A14[2];
    u16 unk_A16;                            // 0xA16
    u8 unk_A18;                             // 0xA18
    u8 unk_A19;                             // 0xA19
    u8 linkMessageCount;                    // 0xA1A
    u8 unk_A1B;                             // 0xA1B
    u16 unk_A1C;                            // 0xA1C
    u8 filler_A1E[2];
    u16 *unk_A20;                           // 0xA20
};

// Battle Hall

void ov80_0222AEF8(int netId, int size, void *data, HallLinkWork *work) {
    work->linkMessageCount++;
    if (netId != Comm_GetLocalNetId()) {
        return;
    }
}

BOOL ov80_0222AF10(HallLinkWork *work) {
    Save_PlayerData_GetProfile(work->saveData);
    return Comm_SendCommand(0x22, work->linkBuffer, 0x2C) == TRUE;
}

void ov80_0222AF3C(int netId, int size, void *data, HallLinkWork *work) {
    work->linkMessageCount++;
    if (netId != Comm_GetLocalNetId()) {
        return;
    }
}

BOOL ov80_0222AF54(HallLinkWork *work) {
    u8 *p = (u8 *)work;
    u16 *buffer = work->linkBuffer;
    u16 *dst = buffer;
    int i;
    for (i = 0; i < 20; i++) {
        *dst++ = ((HallLinkWork *)p)->unk_018[0];
        p += 2;
    }
    return Comm_SendCommand(0x23, buffer, 0x2C) == TRUE;
}

void ov80_0222AF84(int netId, int size, const u16 *data, HallLinkWork *work) {
    u8 *p = (u8 *)work;
    int i;
    work->linkMessageCount++;
    if (netId != Comm_GetLocalNetId() && Comm_GetLocalNetId() != 0) {
        for (i = 0; i < 20; i++) {
            ((HallLinkWork *)p)->unk_018[0] = *data++;
            p += 2;
        }
    }
}

BOOL ov80_0222AFB8(HallLinkWork *work) {
    u8 *p = (u8 *)work;
    u16 *buffer = work->linkBuffer;
    int i = 0;
    u16 *dst = buffer;
    while (i < 20) {
        *dst++ = ((HallLinkWork *)p)->unk_268[0];
        p += 2;
        i++;
    }
    return Comm_SendCommand(0x24, buffer, 0x2C) == TRUE;
}

void ov80_0222AFEC(int netId, int size, const u16 *data, HallLinkWork *work) {
    u8 *p = (u8 *)work;
    int i;
    work->linkMessageCount++;
    if (netId != Comm_GetLocalNetId() && Comm_GetLocalNetId() != 0) {
        for (i = 0; i < 20; i++) {
            ((HallLinkWork *)p)->unk_268[0] = *data++;
            p += 2;
        }
    }
}

BOOL ov80_0222B024(HallLinkWork *work, u16 value) {
    work->linkBuffer[0] = value;
    return Comm_SendCommand(0x25, work->linkBuffer, 0x2C) == TRUE;
}

void ov80_0222B048(int netId, int size, const u16 *data, HallLinkWork *work) {
    work->linkMessageCount++;
    if (netId != Comm_GetLocalNetId()) {
        work->unk_D88 = *data;
    }
}

BOOL ov80_0222B070(HallLinkWork *work) {
    u32 size = SizeOfStructPokemon();
    Pokemon *mon = Party_GetMonByIndex(SaveArray_Party_Get(work->saveData), work->partySlot);
    MI_CpuCopy8(mon, work->linkMonBuffer, size);
    return sub_02036FD8(0x26, work->linkMonBuffer, LINK_MON_BUFFER_SIZE) == TRUE;
}

void BattleHallLink_ReceivePartnerMon(int netId, int size, const void *data, HallLinkWork *work) {
    work->linkMessageCount++;
    if (netId != Comm_GetLocalNetId()) {
        MI_CpuCopy8(data, work->partnerMon, SizeOfStructPokemon());
    }
}

void *BattleHallLink_GetRecvMonBuffer(int netId, HallLinkWork *work, int size) {
    GF_ASSERT(size <= LINK_MON_BUFFER_SIZE);
    return work->linkRecvMon[netId];
}

// Battle Factory

BOOL ov80_0222B108(FactoryLinkWork *work) {
    u16 *buffer = work->linkBuffer;
    Save_PlayerData_GetProfile(work->saveData);
    buffer[1] = work->unk_008;
    buffer[2] = work->unk_00C;
    buffer[3] = work->unk_00E;
    return Comm_SendCommand(0x16, buffer, 0x3C) == TRUE;
}

void ov80_0222B140(int netId, int size, const u16 *data, FactoryLinkWork *work) {
    work->linkMessageCount++;
    if (netId != Comm_GetLocalNetId()) {
        work->unk_580 = data[1];
        work->unk_582 = data[2];
        work->unk_57E = data[3];
    }
}

BOOL BattleFactoryLink_SendTrainerIds(FactoryLinkWork *work) {
    u8 *p = (u8 *)work;
    u16 *buffer = work->linkBuffer;
    u16 *dst = buffer;
    int i;
    for (i = 0; i < 14; i++) {
        *dst++ = ((FactoryLinkWork *)p)->trainerIds[0];
        p += 2;
    }
    return Comm_SendCommand(0x17, buffer, 0x3C) == TRUE;
}

void BattleFactoryLink_ReceiveTrainerIds(int netId, int size, const u16 *data, FactoryLinkWork *work) {
    u8 *p = (u8 *)work;
    int i;
    work->linkMessageCount++;
    if (netId != Comm_GetLocalNetId() && Comm_GetLocalNetId() != 0) {
        for (i = 0; i < 14; i++) {
            ((FactoryLinkWork *)p)->trainerIds[0] = *data++;
            p += 2;
        }
    }
}

// The message carries {setIds[6], ivs[6], personalities low halves[6], high halves[6]}.
BOOL BattleFactoryLink_SendRentalMons(FactoryLinkWork *work) {
    u8 *buffer = (u8 *)work->linkBuffer;
    {
        int i = 0;
        u8 *src = (u8 *)work;
        u8 *dst = buffer;
        for (; i < 6; i++) {
            *(u16 *)dst = ((FactoryLinkWork *)src)->partnerSetIds[0];
            src += 2;
            dst += 2;
        }
    }
    {
        u8 *dst;
        int i = 0;
        dst = buffer;
        for (; i < 6; i++) {
            *(u16 *)(dst + 12) = work->partnerIVs[i];
            dst += 2;
        }
    }
    {
        u8 *dst;
        int i;
        u8 *src = (u8 *)work;
        dst = buffer;
        i = 0;
        dst += 24;
        for (; i < 6; i++) {
            *(u16 *)dst = ((FactoryLinkWork *)src)->partnerPersonalities[0];
            *(u16 *)(dst + 12) = ((FactoryLinkWork *)src)->partnerPersonalities[0] >> 16;
            src += 4;
            dst += 2;
        }
    }
    return Comm_SendCommand(0x18, buffer, 0x3C) == TRUE;
}

typedef struct FactoryMonsMessage6 {
    u16 setIds[6];
    u16 ivs[6];
    u16 personalityLow[6];
    u16 personalityHigh[6];
} FactoryMonsMessage6;

void BattleFactoryLink_ReceiveRentalMons(int netId, int size, const u16 *data, FactoryLinkWork *work) {
    const FactoryMonsMessage6 *msg = (const FactoryMonsMessage6 *)data;
    int i;
    work->linkMessageCount++;
    if (netId == Comm_GetLocalNetId() || Comm_GetLocalNetId() == 0) {
        return;
    }
    for (i = 0; i < 6; i++) {
        work->rentalSetIds[i] = msg->setIds[i];
    }
    for (i = 0; i < 6; i++) {
        work->rentalIVs[i] = msg->ivs[i];
    }
    i = 0;
    data += 12;
    for (; i < 6; i++) {
        work->rentalPersonalities[0] = data[0];
        work->rentalPersonalities[0] |= data[6] << 16;
        data++;
        work = (FactoryLinkWork *)((u8 *)work + 4);
    }
}

// The message carries {setIds[4], ivs[4], personalities low halves[4], high halves[4]}.
BOOL BattleFactoryLink_SendOpponentMons(FactoryLinkWork *work) {
    u8 *buffer = (u8 *)work->linkBuffer;
    {
        int i = 0;
        u8 *src = (u8 *)work;
        u8 *dst = buffer;
        for (; i < 4; i++) {
            *(u16 *)dst = ((FactoryLinkWork *)src)->opponentSetIds[0];
            src += 2;
            dst += 2;
        }
    }
    {
        u8 *dst;
        int i = 0;
        dst = buffer;
        for (; i < 4; i++) {
            *(u16 *)(dst + 8) = work->opponentIVs[i];
            dst += 2;
        }
    }
    {
        u8 *dst;
        int i;
        u8 *src = (u8 *)work;
        dst = buffer;
        i = 0;
        dst += 16;
        for (; i < 4; i++) {
            *(u16 *)dst = ((FactoryLinkWork *)src)->opponentPersonalities[0];
            *(u16 *)(dst + 8) = ((FactoryLinkWork *)src)->opponentPersonalities[0] >> 16;
            src += 4;
            dst += 2;
        }
    }
    return Comm_SendCommand(0x19, buffer, 0x3C) == TRUE;
}

typedef struct FactoryMonsMessage4 {
    u16 setIds[4];
    u16 ivs[4];
    u16 personalityLow[4];
    u16 personalityHigh[4];
} FactoryMonsMessage4;

void BattleFactoryLink_ReceiveOpponentMons(int netId, int size, const u16 *data, FactoryLinkWork *work) {
    const FactoryMonsMessage4 *msg = (const FactoryMonsMessage4 *)data;
    int i;
    work->linkMessageCount++;
    if (netId == Comm_GetLocalNetId() || Comm_GetLocalNetId() == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        work->opponentSetIds[i] = msg->setIds[i];
    }
    for (i = 0; i < 4; i++) {
        work->opponentIVs[i] = msg->ivs[i];
    }
    i = 0;
    data += 8;
    for (; i < 4; i++) {
        work->opponentPersonalities[0] = data[0];
        work->opponentPersonalities[0] |= data[4] << 16;
        data++;
        work = (FactoryLinkWork *)((u8 *)work + 4);
    }
}

BOOL ov80_0222B3B0(FactoryLinkWork *work, u16 value) {
    work->linkBuffer[0] = value;
    return Comm_SendCommand(0x1A, work->linkBuffer, 0x3C) == TRUE;
}

void ov80_0222B3D4(int netId, int size, const u16 *data, FactoryLinkWork *work) {
    work->linkMessageCount++;
    if (netId != Comm_GetLocalNetId()) {
        work->unk_57C = *data;
    }
}

BOOL ov80_0222B3FC(FactoryLinkWork *work, u16 value) {
    work->linkBuffer[0] = value;
    return Comm_SendCommand(0x1B, work->linkBuffer, 0x3C) == TRUE;
}

void ov80_0222B420(int netId, int size, const u16 *data, FactoryLinkWork *work) {
    work->linkMessageCount++;
    if (netId != Comm_GetLocalNetId()) {
        work->unk_57D = *data;
    }
}

#pragma force_active on

// Sends the set ids, attack IVs and personalities of the player's party: {setIds[n], ivs[n], low halves[n],
// high halves[n]} with n = the facility type's Pokemon count.
BOOL BattleFactoryLink_SendPartyMons(FactoryLinkWork *work) {
    u32 personalities[2];
    u8 ivs[4];
    int i;
    int pos;
    u16 *buffer;
    Pokemon *mon;
    int count;

    buffer = work->linkBuffer;
    pos = 0;

    count = ov80_02236DD4(work->type);
    for (i = 0; i < count; i++) {
        mon = Party_GetMonByIndex(work->party, i);
        ivs[i] = GetMonData(mon, MON_DATA_ATK_IV, NULL);
        personalities[i] = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
    }
    for (i = 0; i < count; i++) {
        buffer[i] = work->partySetIds[i];
    }
    pos += count;
    for (i = 0; i < count; i++) {
        buffer[pos + i] = ivs[i];
    }
    pos += count;
    for (i = 0; i < count; i++) {
        buffer[pos + i] = personalities[i];
        buffer[count + pos + i] = personalities[i] >> 16;
    }
    if (Comm_SendCommand(0x1C, buffer, 0x3C) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void BattleFactoryLink_ReceivePartyMons(int netId, int size, u16 *data, FactoryLinkWork *work) {
    int i;
    int count;
    int pos = 0;

    work->linkMessageCount++;
    if (netId == Comm_GetLocalNetId()) {
        return;
    }
    count = ov80_02236DD4(work->type);
    for (i = 0; i < count; i++) {
        work->partnerSetIds[i] = data[i];
    }
    pos += count;
    for (i = 0; i < count; i++) {
        work->partnerIVs[i] = data[pos + i];
    }
    pos += count;
    for (i = 0; i < count; i++) {
        work->partnerPersonalities[i] = data[pos + i];
        work->partnerPersonalities[i] |= data[count + pos + i] << 16;
    }
}

#pragma force_active reset

// Battle Castle

BOOL ov80_0222B5C8(CastleLinkWork *work) {
    FrontierSave *frontierSave;
    u32 stat;
    u32 substat;
    Save_Frontier_GetCastleFlags(work->saveData);
    work->linkBuffer[1] = work->unk_014;
    work->linkBuffer[2] = work->unk_016;
    frontierSave = Save_Frontier_GetStatic(work->saveData);
    stat = sub_0205C1F0(work->type);
    substat = sub_0205C268(sub_0205C1F0(work->type));
    work->linkBuffer[11] = FrontierSave_GetStat(frontierSave, stat, substat);
    return Comm_SendCommand(0x2A, work->linkBuffer, 0x28) == TRUE;
}

void ov80_0222B628(int netId, int size, const u16 *data, CastleLinkWork *work) {
    work->linkMessageCount++;
    if (netId != Comm_GetLocalNetId()) {
        work->unk_A16 = data[1];
        work->unk_A12 = data[2];
        work->unk_A1C = data[11];
    }
}

BOOL BattleCastleLink_SendTrainerIds(CastleLinkWork *work) {
    int i = 0;
    u8 *p = (u8 *)work;
    while (i < 14) {
        ((CastleLinkWork *)p)->linkBuffer[0] = ((CastleLinkWork *)p)->trainerIds[0];
        p += 2;
        i++;
    }
    return Comm_SendCommand(0x2B, work->linkBuffer, 0x28) == TRUE;
}

void BattleCastleLink_ReceiveTrainerIds(int netId, int size, const u16 *data, CastleLinkWork *work) {
    u8 *p = (u8 *)work;
    int i;
    work->linkMessageCount++;
    if (netId != Comm_GetLocalNetId() && Comm_GetLocalNetId() != 0) {
        for (i = 0; i < 14; i++) {
            ((CastleLinkWork *)p)->trainerIds[0] = *data++;
            p += 2;
        }
    }
}

BOOL ov80_0222B6C8(CastleLinkWork *work) {
    work->linkBuffer[0] = work->unk_A18;
    if (Comm_GetLocalNetId() == 0) {
        if (work->unk_A1B == 0) {
            work->unk_A1B = work->unk_A18;
        } else if (work->unk_A1B - 6 == 4 && work->unk_A18 != 4) {
            work->unk_A1B = work->unk_A18;
        }
    } else if (work->unk_A1B == 4 && work->unk_A18 != 4) {
        work->unk_A1B = work->unk_A18 + 6;
    }
    work->linkBuffer[1] = work->unk_A1B;
    return Comm_SendCommand(0x2C, work->linkBuffer, 0x28) == TRUE;
}

void ov80_0222B740(int netId, int size, void *data, CastleLinkWork *work) {
    const u16 *msg = data;

    work->linkMessageCount++;

    if (Comm_GetLocalNetId() == netId) {
        return;
    }

    work->unk_A19 = msg[0];

    if (Comm_GetLocalNetId() == 0) {
        if (work->unk_A1B != 0) {
            if (work->unk_A1B == 4) {
                if (work->unk_A19 != 4) {
                    work->unk_A18 = work->unk_A19 + 6;
                    work->unk_A1B = work->unk_A19 + 6;
                }
            }
        } else {
            work->unk_A1B = work->unk_A19 + 6;

            if (work->unk_A19 != 4) {
                *work->unk_A20 = 0xEEDD;
            }
        }
    } else {
        work->unk_A1B = msg[1];

        if (work->unk_A1B != 4) {
            *work->unk_A20 = 0xEEDD;
        }

        if (work->unk_A19 == 4) {
            if (work->unk_A18 != 0) {
                if (work->unk_A18 != 4) {
                    work->unk_A1B = work->unk_A18 + 6;
                }
            }
        }
    }
}

// The message carries {setIds[4], ivs[4], personalities low halves[4], high halves[4]}.
BOOL BattleCastleLink_SendOpponentMons(CastleLinkWork *work) {
    int i, offset, result, size;

    offset = 0;
    size = 40;

    for (i = 0; i < 4; i++) {
        work->linkBuffer[i] = work->opponentSetIds[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        work->linkBuffer[i + offset] = work->opponentIVs[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        work->linkBuffer[i + offset] = (work->opponentPersonalities[i] & 0xFFFF);
        work->linkBuffer[i + offset + 4] = ((work->opponentPersonalities[i] >> 16) & 0xFFFF);
    }

    offset += 4 * 2;

    if (Comm_SendCommand(0x2D, work->linkBuffer, size) == TRUE) {
        result = TRUE;
    } else {
        result = FALSE;
    }

    return result;
}

void BattleCastleLink_ReceiveOpponentMons(int netId, int size, void *data, CastleLinkWork *work) {
    int i, offset;
    const u16 *msg = data;

    offset = 0;
    work->linkMessageCount++;

    if (Comm_GetLocalNetId() == netId || Comm_GetLocalNetId() == 0) {
        return;
    }

    for (i = 0; i < 4; i++) {
        work->opponentSetIds[i] = msg[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        work->opponentIVs[i] = msg[i + offset];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        work->opponentPersonalities[i] = msg[i + offset];
        work->opponentPersonalities[i] |= (msg[i + offset + 4] << 16);
    }

    offset += 4 * 2;
}

BOOL ov80_0222B8D8(CastleLinkWork *work, u16 value) {
    work->linkBuffer[0] = value;
    return Comm_SendCommand(0x2E, work->linkBuffer, 0x28) == TRUE;
}

void ov80_0222B8F8(int netId, int size, const u16 *data, CastleLinkWork *work) {
    work->linkMessageCount++;
    if (netId != Comm_GetLocalNetId()) {
        work->unk_A10 = *data;
    }
}

BOOL ov80_0222B920(CastleLinkWork *work, u16 value) {
    work->linkBuffer[0] = value;
    return Comm_SendCommand(0x2F, work->linkBuffer, 0x28) == TRUE;
}

void ov80_0222B940(int netId, int size, const u16 *data, CastleLinkWork *work) {
    work->linkMessageCount++;
    if (netId != Comm_GetLocalNetId()) {
        work->unk_A11 = *data;
    }
}

BOOL BattleCastleLink_SendPartyMons(CastleLinkWork *work) {
    s32 count = ov80_02237B24(work->type, 0);
    u32 size = SizeOfStructPokemon();
    s32 i;
    i = 0;
    if (count > 0) {
        u32 offset = i;
        do {
            Pokemon *mon = Party_GetMonByIndex(work->party, i);
            MI_CpuCopy8(mon, work->linkMonBuffer + offset, size);
            i++;
            offset += size;
        } while (i < count);
    }
    return sub_02036FD8(0x30, work->linkMonBuffer, LINK_MON_BUFFER_SIZE) == TRUE;
}

#pragma force_active on

// Adds the partner's Pokemon to the party; the child then swaps them in front of its own.
void BattleCastleLink_ReceivePartyMons(int netId, int size, void *data, CastleLinkWork *work) {
    int i;
    int count;
    int monSize;
    Pokemon *mon;

    work->linkMessageCount++;
    if (netId == Comm_GetLocalNetId()) {
        return;
    }
    count = ov80_02237B24(work->type, 0);
    monSize = SizeOfStructPokemon();
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < count; i++) {
        MI_CpuCopy8((u8 *)data + i * monSize, mon, monSize);
        Party_AddMon(work->party, mon);
    }
    Heap_Free(mon);
    if (Comm_GetLocalNetId() != 0) {
        Party_SwapSlots(work->party, 0, 2);
        Party_SwapSlots(work->party, 1, 3);
    }
}

#pragma force_active reset

void *BattleCastleLink_GetRecvMonBuffer(int netId, CastleLinkWork *work, int size) {
    GF_ASSERT(size <= LINK_MON_BUFFER_SIZE);
    return work->linkRecvMon[netId];
}

// Battle Arcade (ArcadeContext)

BOOL BattleArcadeLink_SendWinStreak(ArcadeContext *ctx) {
    Save_Frontier_GetArcadeFlags(ctx->saveData);
    ctx->linkBuffer[1] = ctx->winStreak;
    ctx->linkBuffer[2] = ctx->unk1A;
    return Comm_SendCommand(0x41, ctx->linkBuffer, 0x28) == TRUE;
}

void BattleArcadeLink_ReceiveWinStreak(int netId, int size, const u16 *data, ArcadeContext *ctx) {
    ctx->linkMessageCount++;
    if (netId != Comm_GetLocalNetId()) {
        ctx->multiWinStreak = data[1];
        ctx->unkA76 = data[2];
    }
}

BOOL BattleArcadeLink_SendTrainerIds(ArcadeContext *ctx) {
    int i = 0;
    u8 *p = (u8 *)ctx;
    while (i < 14) {
        ((ArcadeContext *)p)->linkBuffer[0] = ((ArcadeContext *)p)->trainerIds[0];
        p += 2;
        i++;
    }
    return Comm_SendCommand(0x42, ctx->linkBuffer, 0x28) == TRUE;
}

void BattleArcadeLink_ReceiveTrainerIds(int netId, int size, const u16 *data, ArcadeContext *ctx) {
    u8 *p = (u8 *)ctx;
    int i;
    ctx->linkMessageCount++;
    if (netId != Comm_GetLocalNetId() && Comm_GetLocalNetId() != 0) {
        for (i = 0; i < 14; i++) {
            u16 value = *data;
            ((ArcadeContext *)p)->trainerIds[0] = value;
            data++;
            p += 2;
        }
    }
}

#pragma force_active on

// The message carries {setIds[4], ivs[4], personalities low halves[4], high halves[4]} of the opponents.
BOOL BattleArcadeLink_SendOpponentMons(ArcadeContext *ctx) {
    int i;
    u8 *cursor;
    i = 0;
    cursor = (u8 *)ctx;
    for (; i < 4; i++, cursor += 2) {
        ((ArcadeContext *)cursor)->linkBuffer[0] = ((ArcadeContext *)cursor)->unk314[0];
    }
    {
        u8 *second;
        int j = 0;
        second = (u8 *)ctx;
        for (; j < 4; j++, second += 2) {
            ((ArcadeContext *)second)->linkBuffer[4] = ctx->unk31C[j];
        }
    }
    {
        u8 *word;
        u8 *packet;
        i = 0;
        word = (u8 *)ctx;
        packet = (u8 *)ctx + 16;
        for (; i < 4; i++, word += 4, packet += 2) {
            ((ArcadeContext *)packet)->linkBuffer[0] = ((ArcadeContext *)word)->unk320[0];
            ((ArcadeContext *)packet)->linkBuffer[4] = ((ArcadeContext *)word)->unk320[0] >> 16;
        }
    }
    if (Comm_SendCommand(0x43, ctx->linkBuffer, 40) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

struct ArcadeMonsMessage {
    u16 setIds[4];
    u16 ivs[4];
    u16 personalityLow[4];
    u16 personalityHigh[4];
};

void BattleArcadeLink_ReceiveOpponentMons(int netId, int size, ArcadeMonsMessage *msg, ArcadeContext *ctx) {
    int i;
    ctx->linkMessageCount++;
    if (netId == Comm_GetLocalNetId()) {
        return;
    }
    if (Comm_GetLocalNetId() == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        ctx->unk314[i] = msg->setIds[i];
    }
    for (i = 0; i < 4; i++) {
        ctx->unk31C[i] = msg->ivs[i];
    }
    {
        u8 *cursor;
        i = 0;
        cursor = (u8 *)msg + 16;
        for (; i < 4; i++, cursor += 2) {
            ctx->unk320[i] = *(u16 *)cursor;
            ctx->unk320[i] |= (u32) * (u16 *)(cursor + 8) << 16;
        }
    }
}

#pragma force_active reset

BOOL ov80_0222BC48(ArcadeContext *ctx, u16 value) {
    ctx->linkBuffer[0] = value;
    return Comm_SendCommand(0x44, ctx->linkBuffer, 0x28) == TRUE;
}

void ov80_0222BC6C(int netId, int size, const u16 *data, ArcadeContext *ctx) {
    ctx->linkMessageCount++;
    if (netId != Comm_GetLocalNetId()) {
        ctx->unkA74 = *data;
    }
}

BOOL ov80_0222BC94(ArcadeContext *ctx, u16 value) {
    ctx->linkBuffer[0] = value;
    return Comm_SendCommand(0x45, ctx->linkBuffer, 0x28) == TRUE;
}

void ov80_0222BCB8(int netId, int size, const u16 *data, ArcadeContext *ctx) {
    ctx->linkMessageCount++;
    if (netId != Comm_GetLocalNetId()) {
        ctx->unkA75 = *data;
    }
}

BOOL BattleArcadeLink_SendPartyMons(ArcadeContext *ctx) {
    s32 count = BattleArcade_GetMonCount(ctx->type, 0);
    u32 size = SizeOfStructPokemon();
    s32 i;
    i = 0;
    if (count > 0) {
        u32 offset = i;
        do {
            Pokemon *mon = Party_GetMonByIndex(ctx->playerParty, i);
            MI_CpuCopy8(mon, ctx->linkMonBuffer + offset, size);
            i++;
            offset += size;
        } while (i < count);
    }
    return sub_02036FD8(0x46, ctx->linkMonBuffer, LINK_MON_BUFFER_SIZE) == TRUE;
}

#pragma force_active on

// Adds the partner's Pokemon to the party; the child then swaps them in front of its own.
void BattleArcadeLink_ReceivePartyMons(int netId, int size, void *data, ArcadeContext *ctx) {
    int i;
    int count;
    int monSize;
    Pokemon *mon;

    ctx->linkMessageCount++;
    if (netId == Comm_GetLocalNetId()) {
        return;
    }
    count = BattleArcade_GetMonCount(ctx->type, 0);
    monSize = SizeOfStructPokemon();
    mon = AllocMonZeroed(HEAP_ID_FIELD2);
    for (i = 0; i < count; i++) {
        MI_CpuCopy8((u8 *)data + i * monSize, mon, monSize);
        Party_AddMon(ctx->playerParty, mon);
    }
    Heap_Free(mon);
    if (Comm_GetLocalNetId() != 0) {
        Party_SwapSlots(ctx->playerParty, 0, 2);
        Party_SwapSlots(ctx->playerParty, 1, 3);
    }
}

#pragma force_active reset

void *BattleArcadeLink_GetRecvMonBuffer(int netId, ArcadeContext *ctx, int size) {
    GF_ASSERT(size <= LINK_MON_BUFFER_SIZE);
    return ctx->linkRecvMon + netId * LINK_MON_BUFFER_SIZE;
}
