#include "global.h"

#include "easy_chat.h"
#include "heap.h"
#include "location_gmm_dat.h"
#include "mail_message.h"
#include "player_data.h"
#include "pm_string.h"
#include "sav_system_info.h"
#include "save.h"
#include "save_misc_data.h"
#include "save_wifi_history.h"
#include "string_util.h"
#include "union_room_avatar.h"

// Player profile card built from the save (name, trainer ID, favorite Pokemon, greeting, country/region, CRC). The
// getters return safe defaults for out-of-range fields, i.e. for cards received from other players.
typedef struct FrontierCard {
    u16 name[8];
    u32 trainerId;
    u8 gender;
    u8 birthdayMonth;
    u8 avatar;
    u8 country;
    u8 region;
    u8 version;
    u8 unk_1A;
    u8 isEgg : 1;
    u8 form : 7;
    u16 species;
    u8 unk_1E[2];
    u16 greeting[40];
    u8 unk_70[12];
    u16 crc;
    u8 unk_7E[2];
} FrontierCard;

FrontierCard *FrontierCard_New(enum HeapID heapId);
void FrontierCard_Delete(FrontierCard *card);
void FrontierCard_InitFromSave(FrontierCard *card, SaveData *saveData);
String *FrontierCard_NewNameString(const u16 *name, enum HeapID heapId);
u32 FrontierCard_GetGender(FrontierCard *card);
u32 FrontierCard_GetSpecies(FrontierCard *card);
u32 FrontierCard_GetSanitizedForm(FrontierCard *card);
u32 FrontierCard_IsEgg(FrontierCard *card);
u32 FrontierCard_GetCountry(FrontierCard *card);
u32 FrontierCard_GetRegion(FrontierCard *card);
String *FrontierCard_GetGreeting(FrontierCard *card, MailMessage *message, enum HeapID heapId);
u32 FrontierCard_GetBirthdayMonth(FrontierCard *card);
u32 FrontierCard_GetAvatar(FrontierCard *card);
u8 SanitizeMonForm(u16 species, u8 form);

FrontierCard *FrontierCard_New(enum HeapID heapId) {
    FrontierCard *ret = Heap_Alloc(heapId, sizeof(FrontierCard));
    MI_CpuFill8(ret, 0, sizeof(FrontierCard));
    return ret;
}

void FrontierCard_Delete(FrontierCard *card) {
    Heap_Free(card);
}

void FrontierCard_InitFromSave(FrontierCard *card, SaveData *saveData) {
    PlayerProfile *profile = Save_PlayerData_GetProfile(saveData);
    SaveWiFiHistory *wifiHistory = Save_WiFiHistory_Get(saveData);
    Save_SysInfo_Get(saveData);
    SAVE_MISC_DATA *misc = (SAVE_MISC_DATA *)Save_Misc_Const_Get(saveData);
    OSOwnerInfo ownerInfo;
    int species;
    int form;
    int isEgg;
    int i;

    OS_GetOwnerInfo(&ownerInfo);
    SaveMisc_GetFavoriteMon(misc, &species, &form, &isEgg);
    MI_CpuFill8(card, 0, sizeof(FrontierCard));
    CopyU16StringArray(card->name, PlayerProfile_GetNamePtr(profile));
    card->trainerId = PlayerProfile_GetTrainerID(profile);
    card->gender = PlayerProfile_GetTrainerGender(profile);
    card->species = species;
    card->form = form;
    card->isEgg = isEgg;
    card->country = WifiHistory_GetPlayerCountry(wifiHistory);
    card->region = WiFiHistory_GetPlayerRegion(wifiHistory);
    for (i = 0; i < 40; i++) {
        card->greeting[i] = 0xFFFF;
    }
    SaveMisc_GetBattleGreeting(misc, (MailMessage *)card->greeting);
    card->birthdayMonth = ownerInfo.birthday.month;
    card->avatar = GetUnionRoomAvatarAttrBySprite(PlayerProfile_GetTrainerGender(profile), PlayerProfile_GetAvatar(profile), 0);
    card->version = GAME_VERSION;
    card->unk_1A = 2;
    card->crc = SaveArray_CalcCRC16(saveData, card, 0x7C);
}

String *FrontierCard_NewNameString(const u16 *name, enum HeapID heapId) {
    String *ret = String_New(15, heapId);
    CopyU16ArrayToStringN(ret, name, 15);
    return ret;
}

u32 FrontierCard_GetGender(FrontierCard *card) {
    if (card->gender != 0 && card->gender != 1) {
        return 0;
    }
    return card->gender;
}

u32 FrontierCard_GetSpecies(FrontierCard *card) {
    if (card->species >= 495) {
        return 0;
    }
    return card->species;
}

u32 FrontierCard_GetSanitizedForm(FrontierCard *card) {
    if (card->species >= 495) {
        return 0;
    }
    return SanitizeMonForm(card->species, card->form);
}

u32 FrontierCard_IsEgg(FrontierCard *card) {
    u32 ret = card->isEgg;
    if (ret > 1) {
        ret = 1;
    }
    return ret;
}

u32 FrontierCard_GetCountry(FrontierCard *card) {
    if (card->country >= 234) {
        return 0;
    }
    return card->country;
}

u32 FrontierCard_GetRegion(FrontierCard *card) {
    u32 country = card->country;
    u32 region;
    if (country >= 234) {
        return 0;
    }
    region = card->region;
    if (LocationGmmDatRegionCountGetByCountryMsgNo(country) < region) {
        region = 0;
    }
    return region;
}

// Easy chat greeting (validated into message, returns NULL), or the free-text greeting as a new String.
String *FrontierCard_GetGreeting(FrontierCard *card, MailMessage *message, enum HeapID heapId) {
    int invalid = 0;
    u32 category, msgNo;
    if (card->unk_1E[0] == 0) {
        message->msg_bank = card->greeting[0];
        message->msg_no = card->greeting[1];
        message->fields[0] = card->greeting[2];
        message->fields[1] = card->greeting[3];
        if (message->msg_bank >= 5) {
            invalid++;
        } else if (message->msg_no > 19) {
            invalid++;
        } else if ((message->fields[0] != 0xFFFF && !GetCategoryAndMsgNoByECWordIdx(message->fields[0], &category, &msgNo)) || (message->fields[1] != 0xFFFF && !GetCategoryAndMsgNoByECWordIdx(message->fields[1], &category, &msgNo))) {
            invalid++;
        }
        if (invalid > 0) {
            MailMsg_Init_WithBank(message, 4);
            message->msg_no = 0;
            message->fields[0] = GetECWordIndexByPair(0x11F, 99);
            message->fields[1] = 0xFFFF;
        }
        return NULL;
    } else {
        String *result = String_New(40, heapId);
        CopyU16ArrayToStringN(result, card->greeting, 40);
        return result;
    }
}

u32 FrontierCard_GetBirthdayMonth(FrontierCard *card) {
    u32 month = card->birthdayMonth;
    if (month < 1 || month > 12) {
        month = 1;
    }
    return month;
}

u32 FrontierCard_GetAvatar(FrontierCard *card) {
    u32 avatar = card->avatar;
    if (avatar > 15) {
        avatar = 0;
    }
    return avatar;
}
