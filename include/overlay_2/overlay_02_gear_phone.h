#ifndef POKEHEARTGOLD_OVERLAY_02_GEAR_PHONE_H
#define POKEHEARTGOLD_OVERLAY_02_GEAR_PHONE_H

#include "constants/phone_constants.h"

#include "field_system.h"
#include "gear_phone_ring_manager.h"

enum RadioMusicPlayingSeq {
    RADIO_MUSIC_SEQ_NONE = 0,
    RADIO_MUSIC_SEQ_MARCH,
    RADIO_MUSIC_SEQ_LULLABY,
    RADIO_MUSIC_SEQ_HOENN_SOUNDS,
    RADIO_MUSIC_SEQ_SINNOH_SOUNDS,
    RADIO_MUSIC_SEQ_POKEFLUTE,
    RADIO_MUSIC_SEQ_UNOWN,
};

String *GetPhoneBookEntryName(GearPhoneRingManager *gearPhone, enum HeapID heapID);
void GearPhoneRingManager_SetCallerParams(GearPhoneRingManager *phoneRingManager, u8 callerId, u32 a2, u8 a3, u8 a4, u8 a5);
u8 GearPhoneRingManager_GetCallerParams(GearPhoneRingManager *gearPhone, Unk_PokegearSTRUCT_14 *params);
BOOL GearPhoneRingManager_TryStartCall(GearPhoneRingManager *gearPhone);
enum RadioMusicPlayingSeq GetRadioMusicPlayingSeq(void);

#endif
