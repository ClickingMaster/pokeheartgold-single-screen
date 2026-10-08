#pragma force_active on
#include "global.h"
#include "field_system.h"
#include "player_data.h"
#include "game_stats.h"
#include "pokedex.h"
#include "save_trainer_card.h"
#include "sys_flags.h"
typedef union Card {
    u8 raw[0x66c];
    struct {
        u8 data[0x668];
        u16 checksum;
        u16 reserved;
    };
} Card;
u8 sub_020691E8(FieldSystem *fieldSystem);
void sub_020692A0(u32, u32, u32, u32, u8, Card *);
void sub_020692C4(u32, u8, const u16 *, u32, u32, u32, u32, Card *);
void sub_02055624(FieldSystem *, RTCDate *, RTCTime *);
void FieldSystem_GetGameClearTime(FieldSystem *, RTCDate *, RTCTime *);
void sub_02069308(u8, void *, const RTCDate *, const RTCDate *, const RTCTime *, u8, Card *);
void sub_020693AC(u32, u32, u32, u32, u8, const void *, Card *);
void sub_0206940C(PlayerProfile *, FieldSystem *, Card *);
void sub_02068FC8(u8 a0, u8 a1, u8 a2, u8 a3, FieldSystem *fieldSystem, Card *card);
static inline u32 SumB(GameStats *stats) { return GameStats_GetCapped(stats, 33) + (GameStats_GetCapped(stats, 26) + (GameStats_GetCapped(stats, 21) + (GameStats_GetCapped(stats, 25) + (GameStats_GetCapped(stats, 92) + GameStats_GetCapped(stats, 20))))); }
// @fn sub_02068FC8
void sub_02068FC8(u8 a0, u8 a1, u8 a2, u8 a3, FieldSystem *fieldSystem, Card *card) {
    SaveData *save;
    PlayerProfile *profile;
    GameStats *stats;
    void *igt;
    RTCDate date;
    RTCDate clearDate;
    RTCTime time;
    struct SaveTrainerCard *trainerCard;
    u32 i;
    u32 checksum;

    save = FieldSystem_GetSaveData(fieldSystem);
    profile = Save_PlayerData_GetProfile(save);
    stats = Save_GameStats_Get(save);
    memset(card, 0, sizeof(Card));
    card->raw[5] = a3;
    sub_020692A0(a0, GAME_VERSION, sub_020691E8(fieldSystem), a2, PlayerProfile_GetLanguage(profile), card);
    sub_020692C4(PlayerProfile_GetTrainerID_VisibleHalf(profile), PlayerProfile_GetTrainerGender(profile), PlayerProfile_GetNamePtr(profile), PlayerProfile_GetMoney(profile), Pokedex_CountDexOwned(Save_Pokedex_Get(fieldSystem->saveData)), Pokedex_IsEnabled(Save_Pokedex_Get(fieldSystem->saveData)), GameStats_GetScore(stats), card);
    igt = Save_PlayerData_GetIGTAddr(save);
    sub_02055624(fieldSystem, &date, &time);
    FieldSystem_GetGameClearTime(fieldSystem, &clearDate, &time);
    sub_02069308(CheckGameClearFlag(Save_VarsFlags_Get(fieldSystem->saveData)), igt, &date, &clearDate, &time, a1, card);
    trainerCard = Save_TrainerCard_Get(fieldSystem->saveData);
    sub_020693AC(SumB(stats),
        GameStats_GetCapped(stats, 22) + GameStats_GetCapped(stats, 27),
        GameStats_GetCapped(stats, 23) + GameStats_GetCapped(stats, 28),
        GameStats_GetCapped(stats, 20) + GameStats_GetCapped(stats, 25),
        TrainerCard_SignatureExists(trainerCard),
        TrainerCard_GetSignature(trainerCard),
        card);
    sub_0206940C(profile, fieldSystem, card);
    checksum = 0;
    for (i = 0; i < sizeof(Card); i++) {
        checksum ^= card->raw[i];
    }
    card->checksum = checksum;
    card->reserved = 0;
}
