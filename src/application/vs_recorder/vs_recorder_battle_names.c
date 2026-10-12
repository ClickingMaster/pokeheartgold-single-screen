#include "global.h"

#include "constants/pokemon.h"

#include "application/vs_recorder/vs_recorder_screens.h"
#include "battle/battle_setup.h"

#include "font.h"
#include "heap.h"
#include "party.h"
#include "pm_string.h"
#include "pokemon.h"

#pragma force_active on

// Before a battle video is played: trainer names without a terminator or with characters the font can't draw
// become the placeholder name, nicknames the font can't draw are replaced by the species name.
void VsRecorder_SanitizeBattleSetupNames(BattleRecord *record, BattleSetup *setup, enum HeapID heapID) {
    u8 *setupBytes = (u8 *)setup;

    struct {
        int partySize, count;
    } battlers;

    int i, j;
    String *str, *checked;
    int partyCount;
    u16 *nickname;
    u8 *trainer, *dst, *src;

    BattleRecord_GetBattlerAndPartyCount(record->header[0x26], &battlers.count, &battlers.partySize); // header type
    str = String_New(64, heapID);
    checked = String_New(64, heapID);
    nickname = Heap_Alloc(heapID, 128);

    i = 0;
    if (i < battlers.count) {
        trainer = setupBytes;
        dst = setupBytes + 0x3C; // setup->trainer[i].name
        src = dst;
        do {
            u32 cursor = (u32)trainer + 14; // last character of the name
            for (j = PLAYER_NAME_LENGTH; j > -1; j--, cursor -= 2) {
                if (*(const u16 *)(cursor + 0x3C) == EOS) {
                    break;
                }
            }
            if (j == -1) {
                VsRecorder_LoadPlaceholderTrainerName(checked, heapID);
                CopyStringToU16Array(checked, (u16 *)dst, PLAYER_NAME_LENGTH + 1);
            } else {
                String_SetEmpty(str);
                CopyU16ArrayToString(str, (const u16 *)src);
                if (!FontID_String_AllCharsValid((FontID)0, str, checked)) {
                    VsRecorder_LoadPlaceholderTrainerName(checked, heapID);
                    CopyStringToU16Array(checked, (u16 *)dst, PLAYER_NAME_LENGTH + 1);
                }
            }
            trainer += sizeof(Trainer);
            dst += sizeof(Trainer);
            src += sizeof(Trainer);
            i++;
        } while (i < battlers.count);
    }

    i = 0;
    if (i < battlers.count) {
        do {
            partyCount = Party_GetCount(*(Party **)(setupBytes + 4)); // setup->party[i]
            for (j = 0; j < partyCount; j++) {
                Pokemon *mon = Party_GetMonByIndex(*(Party **)(setupBytes + 4), j);
                if (!GetMonData(mon, MON_DATA_SPECIES_EXISTS, NULL)) {
                    break;
                }
                MIi_CpuClear16(0, nickname, 128);
                GetMonData(mon, MON_DATA_NICKNAME, nickname);
                String_SetEmpty(str);
                CopyU16ArrayToString(str, nickname);
                if (!FontID_String_AllCharsValid((FontID)0, str, checked)) {
                    SetMonData(mon, MON_DATA_SPECIES_NAME, NULL);
                }
            }
            setupBytes += 4;
            i++;
        } while (i < battlers.count);
    }

    String_Delete(str);
    String_Delete(checked);
    Heap_Free(nickname);
}
