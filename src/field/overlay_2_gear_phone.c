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
#include "gear_phone_ring_manager.h"

typedef struct PhoneCallTrigger {
    u8 callerId;
    u8 unk1;
    u16 callScriptID;
    u8 forcePickUp;
    u8 unk5;
} PhoneCallTrigger;

static const PhoneCallTrigger sPhoneCallTriggers[] = {
    // Hey, player, this is great news! A Pokémon hatched from that Egg!
    { PHONE_CONTACT_PROF__ELM, 0, PHONE_SCRIPT_013,  FALSE, 0 },
    // Hello, player? I discovered something called Pokérus
    { PHONE_CONTACT_PROF__ELM, 0, PHONE_SCRIPT_007,  FALSE, 0 },
    // Oh hey, this is the Bike Shop!
    { PHONE_CONTACT_BIKE_SHOP, 0, PHONE_SCRIPT_085,  TRUE,  0 },
    // Hey, this is Bill here. You know your PC boxes are full, right?
    { PHONE_CONTACT_BILL,      0, PHONE_SCRIPT_093,  TRUE,  0 },
    { PHONE_CONTACT_PROF__OAK, 0, PHONE_SCRIPT_NONE, FALSE, 0 },
    { PHONE_CONTACT_DAY_C_MAN, 0, PHONE_SCRIPT_NONE, FALSE, 0 },
    { PHONE_CONTACT_BAOBA,     0, PHONE_SCRIPT_NONE, FALSE, 0 },
    // This is Baoba, I've finally come up with the next test
    { PHONE_CONTACT_BAOBA,     0, PHONE_SCRIPT_142,  TRUE,  0 },
    // This is Baoba, I thought of a new way to play! It's called Object Arrangement!
    { PHONE_CONTACT_BAOBA,     0, PHONE_SCRIPT_143,  TRUE,  0 },
    // This is Baoba, we've come up with new objects
    { PHONE_CONTACT_BAOBA,     0, PHONE_SCRIPT_144,  TRUE,  0 },
    // This is Baoba, we've come up with so many objects!
    { PHONE_CONTACT_BAOBA,     0, PHONE_SCRIPT_145,  TRUE,  0 },
    // This is Baoba, I'm stuck in a time loop!
    { PHONE_CONTACT_BAOBA,     0, PHONE_SCRIPT_146,  TRUE,  0 },
    // Hi honey, I bought something with your money :3
    { PHONE_CONTACT_MOTHER,    0, PHONE_SCRIPT_027,  FALSE, 0 }
};

u32 GearPhoneRingManager_TryRandomCall(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId);
u32 GearPhoneRingManager_TryCallTrigger(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId);
static u8 GearPhoneRingManager_PickCallTrigger(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId);
void GearPhoneRingManager_SetRinging(GearPhoneRingManager *gearPhone, BOOL ringing);

#pragma force_active on

String *GetPhoneBookEntryName(GearPhoneRingManager *gearPhone, enum HeapID heapID) {
    String *str;
    if (!gearPhone->active || gearPhone->callerId >= NUM_PHONE_CONTACTS) {
        str = String_New(8, heapID);
    } else {
        int phoneMsg = GetPhoneMessageGmm(gearPhone->callerId);
        MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, phoneMsg, heapID);
        str = NewString_ReadMsgData(msgData, 0);
        DestroyMsgData(msgData);
    }
    return str;
}

void GearPhoneRingManager_SetCallerParams(GearPhoneRingManager *gearPhone, u8 callerId, u32 a2, u8 a3, u8 isScriptedCall, u8 callScriptID) {
    // a4 set to 2 when passed from script
    // a5 related to message id? when passed from script
    if (callerId >= NUM_PHONE_CONTACTS) {
        gearPhone->callerId = 0xFF;
        gearPhone->callerParams[0] = 0xFF;
        gearPhone->callerParams[1] = 0;
        GF_ASSERT(FALSE);
    } else {
        gearPhone->callerId = callerId;
        gearPhone->callerParams[0] = a2;
        gearPhone->callerParams[1] = a3;
    }

    gearPhone->isScriptedCall = isScriptedCall;
    gearPhone->callScriptID = callScriptID;
}

u8 GearPhoneRingManager_GetCallerParams(GearPhoneRingManager *gearPhone, Unk_PokegearSTRUCT_14 *params) {
    MI_CpuFill8(params, 0, 5);
    if (gearPhone->callerId >= NUM_PHONE_CONTACTS) {
        GF_ASSERT(FALSE);
        params->unk_0 = 0xFF;
        return 0xFF;
    }
    params->unk_0 = gearPhone->callerParams[0];
    params->unk_1 = gearPhone->callerParams[1];
    params->isScriptedCall = gearPhone->isScriptedCall;
    params->callScriptID = gearPhone->callScriptID;
    params->callTriggerID = gearPhone->callTriggerID;
    return gearPhone->callerId;
}

BOOL GearPhoneRingManager_TryStartCall(GearPhoneRingManager *gearPhone) {
    Location *position = LocalFieldData_GetCurrentPosition(Save_LocalFieldData_Get(gearPhone->saveData));

    if (!MapHeader_CanReceivePhoneCalls(position->mapId)) {
        return FALSE;
    }

    if (gearPhone->active || gearPhone->elapsedMinutes < gearPhone->callIntervalMinutes) {
        return FALSE;
    }

    PhoneBook *phoneBook = AllocAndReadPhoneBook(HEAP_ID_FIELD1);
    u32 var = GearPhoneRingManager_TryCallTrigger(gearPhone, phoneBook, position->mapId);
    if (var) {
        FreePhoneBook(phoneBook);
        if (var == 2) {
            gearPhone->sys->unkD2_7 = TRUE;
            StartMapSceneScript(gearPhone->sys, 0x7FF, 0);
            return TRUE;
        }
        GearPhoneRingManager_SetRinging(gearPhone, 1);
        return FALSE;
    }
    u32 rand = MTRandom() % 100;

    if (rand > gearPhone->randomCallChance) {
        FreePhoneBook(phoneBook);
        gearPhone->elapsedMinutes = 0;
        return FALSE;
    }

    var = GearPhoneRingManager_TryRandomCall(gearPhone, phoneBook, position->mapId);
    FreePhoneBook(phoneBook);

    if (var == 0) {
        gearPhone->elapsedMinutes = 0;
    }
    return FALSE;
}

#pragma force_active reset

u32 GearPhoneRingManager_TryRandomCall(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId) {
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
    i = SaveMisc_PhoneRecentCallers_Count(miscData, timeOfDay);
    if (i == numCandidates) {
        SaveMisc_PhoneRecentCallers_Clear(miscData, timeOfDay);
    }
    for (i = 0; i < numCandidates; i++) {
        if (mapId == phoneBook->entries[candidates[i]].mapId) {
            continue;
        }
        if (SaveMisc_PhoneRecentCallers_Contains(miscData, timeOfDay, candidates[i])) {
            continue;
        }
        if (candidates[i] == 8) {
            if (Phone_IsBuenasPasswordHour(hour)) {
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
        GearPhoneRingManager_SetRinging(gearPhone, TRUE);
    }
    Heap_Free(contacts);
    Heap_Free(candidates);
    return numCallers;
}

#pragma force_active on

u32 GearPhoneRingManager_TryCallTrigger(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId) {
    u8 callTriggerID = GearPhoneRingManager_PickCallTrigger(gearPhone, phoneBook, mapId);
    if (callTriggerID == 0xFF) {
        return FALSE;
    }
    GearPhoneRingManager_SetCallerParams(gearPhone, sPhoneCallTriggers[callTriggerID].callerId, 0xFF, 0, 3, sPhoneCallTriggers[callTriggerID].callScriptID);
    GearPhoneRingManager_StartRinging(gearPhone);
    gearPhone->callTriggerID = callTriggerID;
    return sPhoneCallTriggers[callTriggerID].forcePickUp + 1;
}

// FIXME: This is a fakematch from decomp.me, it doesn't match locally without the label https://decomp.me/scratch/YdDak
static u8 GearPhoneRingManager_PickCallTrigger(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId) {
    u8 *ptr = Heap_AllocAtEnd(HEAP_ID_FIELD1, NUM_CALL_TRIGGERS);
    MI_CpuFill8(ptr, 0, NUM_CALL_TRIGGERS);

    int cnt = 0;
    for (int i = 0; i < NUM_CALL_TRIGGERS; i++) {
        if (PhoneCallPersistentState_CheckCallTriggerFlag(gearPhone->savingsData, i) == 0) {
            continue;
        }
        PhoneBookEntry *entry = &phoneBook->entries[sPhoneCallTriggers[i].callerId];
        if (entry->id == PHONE_CONTACT_DAY_C_MAN) {
            if (SavePokegear_IsNumberRegistered(gearPhone->pokegearData, PHONE_CONTACT_DAY_C_MAN) != 0xFF) {
                // had to do this to match
                goto LABEL;
            } else {
                continue;
            }
        } else if (entry->mapId == mapId) {
            continue;
        }
LABEL:
        ptr[cnt++] = i;
    }

    if (cnt == 0) {
        Heap_Free(ptr);
        return 0xFF;
    }

    u8 ret = ptr[LCRandom() % cnt];
    Heap_Free(ptr);
    return ret;
}

void GearPhoneRingManager_SetRinging(GearPhoneRingManager *gearPhone, BOOL ringing) {
    GearPhone_ToggleRinging(gearPhone, ringing);
}

enum RadioMusicPlayingSeq GetRadioMusicPlayingSeq(void) {
    u16 seq = SndRadio_GetSeqNo();
    switch (seq) {
    case SEQ_GS_RADIO_MARCH:
    case SEQ_GS_P_RADIO_MARCH:
        return RADIO_MUSIC_SEQ_MARCH;
    case SEQ_GS_RADIO_KOMORIUTA:
    case SEQ_GS_P_RADIO_KOMORIUTA:
        return RADIO_MUSIC_SEQ_LULLABY;
    case SEQ_GS_RADIO_R_101:
        return RADIO_MUSIC_SEQ_HOENN_SOUNDS;
    case SEQ_GS_RADIO_R_201:
        return RADIO_MUSIC_SEQ_SINNOH_SOUNDS;
    case SEQ_GS_RADIO_UNKNOWN:
        return RADIO_MUSIC_SEQ_UNOWN;
    case SEQ_GS_HUE:
    case SEQ_GS_P_HUE:
        return RADIO_MUSIC_SEQ_POKEFLUTE;
    }
    return RADIO_MUSIC_SEQ_NONE;
}

#pragma force_active reset
