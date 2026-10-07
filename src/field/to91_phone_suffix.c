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

typedef struct UnkStruct_02253C86 {
    u8 callerId;
    u8 unk1;
    u16 callScriptID;
    u8 forcePickUp;
    u8 unk5;
} UnkStruct_02253C86;

static const UnkStruct_02253C86 ov02_02253C84[] = {
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

static u8 ov02_02252218(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId);

#pragma force_active on

u32 ov02_022521C0(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId) {
    u8 callTriggerID = ov02_02252218(gearPhone, phoneBook, mapId);
    if (callTriggerID == 0xFF) {
        return FALSE;
    }
    GearPhoneRingManager_SetCallerParams(gearPhone, ov02_02253C84[callTriggerID].callerId, 0xFF, 0, 3, ov02_02253C84[callTriggerID].callScriptID);
    GearPhoneRingManager_StartRinging(gearPhone);
    gearPhone->callTriggerID = callTriggerID;
    return ov02_02253C84[callTriggerID].forcePickUp + 1;
}

// FIXME: This is a fakematch from decomp.me, it doesn't match locally without the label https://decomp.me/scratch/YdDak
static u8 ov02_02252218(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId) {
    u8 *ptr = Heap_AllocAtEnd(HEAP_ID_FIELD1, NUM_CALL_TRIGGERS);
    MI_CpuFill8(ptr, 0, NUM_CALL_TRIGGERS);

    int cnt = 0;
    for (int i = 0; i < NUM_CALL_TRIGGERS; i++) {
        if (PhoneCallPersistentState_CheckCallTriggerFlag(gearPhone->savingsData, i) == 0) {
            continue;
        }
        PhoneBookEntry *entry = &phoneBook->entries[ov02_02253C84[i].callerId];
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

void ov02_022522AC(GearPhoneRingManager *gearPhone, BOOL a1) {
    GearPhone_ToggleRinging(gearPhone, a1);
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
