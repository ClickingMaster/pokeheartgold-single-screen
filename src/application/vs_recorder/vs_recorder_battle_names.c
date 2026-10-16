#include "global.h"

#include "constants/pokemon.h"

#include "application/vs_recorder/vs_recorder_screens.h"
#include "battle/battle_setup.h"

#include "font.h"
#include "heap.h"
#include "party.h"
#include "pm_string.h"
#include "pokemon.h"

// Before a battle video is played: trainer names without a terminator or with characters the font can't draw
// become the placeholder name, nicknames the font can't draw are replaced by the species name.
void VsRecorder_SanitizeBattleSetupNames(BattleRecord *record, BattleSetup *setup, enum HeapID heapID) {
    int battlerCount, partySize;
    int i, j;
    String *str, *checked;
    int partyCount;
    u16 *nickname;

    BattleRecord_GetBattlerAndPartyCount(record->header[0x26], &battlerCount, &partySize); // the video's battle type
    str = String_New(64, heapID);
    checked = String_New(64, heapID);
    nickname = Heap_Alloc(heapID, 128);

    for (i = 0; i < battlerCount; i++) {
        for (j = PLAYER_NAME_LENGTH; j > -1; j--) {
            if (setup->trainer[i].name[j] == EOS) {
                break;
            }
        }
        if (j == -1) {
            VsRecorder_LoadPlaceholderTrainerName(checked, heapID);
            CopyStringToU16Array(checked, setup->trainer[i].name, PLAYER_NAME_LENGTH + 1);
        } else {
            String_SetEmpty(str);
            CopyU16ArrayToString(str, setup->trainer[i].name);
            if (!FontID_String_AllCharsValid(0, str, checked)) {
                VsRecorder_LoadPlaceholderTrainerName(checked, heapID);
                CopyStringToU16Array(checked, setup->trainer[i].name, PLAYER_NAME_LENGTH + 1);
            }
        }
    }

    for (i = 0; i < battlerCount; i++) {
        partyCount = Party_GetCount(setup->party[i]);
        for (j = 0; j < partyCount; j++) {
            Pokemon *mon = Party_GetMonByIndex(setup->party[i], j);
            if (!GetMonData(mon, MON_DATA_SPECIES_EXISTS, NULL)) {
                break;
            }
            MIi_CpuClear16(0, nickname, 128);
            GetMonData(mon, MON_DATA_NICKNAME, nickname);
            String_SetEmpty(str);
            CopyU16ArrayToString(str, nickname);
            if (!FontID_String_AllCharsValid(0, str, checked)) {
                SetMonData(mon, MON_DATA_SPECIES_NAME, NULL);
            }
        }
    }

    String_Delete(str);
    String_Delete(checked);
    Heap_Free(nickname);
}
