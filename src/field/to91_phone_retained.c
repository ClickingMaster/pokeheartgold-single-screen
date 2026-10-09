#include "constants/maps.h"
#include "constants/phone_scripts.h"
#include "constants/sndseq.h"

#include "overlay_2/overlay_02_gear_phone.h"

#include "map_header.h"
#include "math_util.h"
#include "msgdata.h"
#include "phonebook_dat.h"
#include "sav_system_info.h"
#include "save_local_field_data.h"
#include "save_vars_flags.h"
#include "script_manager.h"
#include "sound_radio.h"
#include "sys_flags.h"
#include "sys_vars.h"
#include "unk_02092BE8.h"

#include "to91_phone_internal.h"

u32 ov02_02251FDC(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId) {
    u8 numContacts;
    u8 numCallers;
    int timeOfDay;
    PhoneContact *contacts;
    SaveVarsFlags *varsFlags;
    SAVE_MISC_DATA *miscData;
    int hour;
    u8 bugContest;
    u8 *candidates;
    u8 numCandidates;
    u8 i;
    u16 rand;

    varsFlags = Save_VarsFlags_Get(gearPhone->saveData);
    Save_PlayerData_GetProfile(gearPhone->saveData);
    miscData = Save_Misc_Get(gearPhone->saveData);
    numContacts = SavePokegear_FindEmptyPhonebookSlot(gearPhone->pokegearData);
    contacts = SavePokegear_AllocAndCopyPhonebook(gearPhone->pokegearData, HEAP_ID_FIELD1);
    candidates = Heap_AllocAtEnd(HEAP_ID_FIELD1, numContacts);
    MI_CpuFill8(candidates, 0xFF, numContacts);
    rand = LCRandom() % 1000;
    if (rand < 500) {
        timeOfDay = 0;
    } else if (rand < 800) {
        timeOfDay = 1;
    } else {
        timeOfDay = 2;
    }
    numCandidates = 0;
    for (i = 0; i < numContacts; i++) {
        u8 id = contacts[i].id;
        PhoneBookEntry *entry = &phoneBook->entries[id];
        if (timeOfDay == entry->unkF) {
            if (entry->type == 0 || entry->type == 12 || entry->type == 11 || entry->type == 10 || entry->type == 14) {
                candidates[numCandidates++] = id;
            }
        }
    }
    if (numCandidates == 0) {
        Heap_Free(contacts);
        Heap_Free(candidates);
        return 0;
    }
    MI_CpuFill8(contacts, 0xFF, numContacts);
    hour = (u8)Save_SysInfo_RTC_Get(gearPhone->saveData)->time.hour;
    bugContest = Save_VarsFlags_CheckBugContestFlag(varsFlags);
    numCallers = 0;
    i = sub_0202AAD4(miscData, timeOfDay);
    if (i == numCandidates) {
        sub_0202AA9C(miscData, timeOfDay);
    }
    for (i = 0; i < numCandidates; i++) {
        if (mapId == phoneBook->entries[candidates[i]].mapId) {
            continue;
        }
        if (sub_0202AA44(miscData, timeOfDay, candidates[i])) {
            continue;
        }
        if (candidates[i] == 8) {
            if (sub_02095FF8(hour)) {
                continue;
            }
            if (Save_VarsFlags_IsInRocketTakeover(varsFlags)) {
                continue;
            }
        }
        if (phoneBook->entries[candidates[i]].mapId == 0x60 && bugContest) {
            continue;
        }
        contacts[numCallers++].id = candidates[i];
    }
    if (numCallers != 0) {
        GearPhoneRingManager_SetCallerParams(gearPhone, contacts[(u8)(u16)(MTRandom() % (numCallers * 100) / 100)].id, timeOfDay, numCandidates, 0, 0);
        GearPhoneRingManager_StartRinging(gearPhone);
        ov02_022522AC(gearPhone, TRUE);
    }
    Heap_Free(contacts);
    Heap_Free(candidates);
    return numCallers;
}
