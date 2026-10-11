#include "global.h"
#pragma require_prototypes off

extern void *Save_PlayerData_GetProfile(void *save);
extern void *SaveArray_Party_Get(void *save);
extern void TrainerHouseSet_SetZero(void *set);
extern int TrainerHouseSet_CheckHasData(void *set);
extern int TrainerHouseTrainer_Compare(void *a, void *b);
extern u16 GF_CalcCRC16(void *data, u32 size);
extern u32 PlayerProfile_GetTrainerID(void *profile);
extern u8 PlayerProfile_GetTrainerGender(void *profile);
extern u8 PlayerProfile_GetLanguage(void *profile);
extern u8 PlayerProfile_GetVersion(void *profile);
extern u8 PlayerProfile_GetAvatar(void *profile);
extern const u16 *PlayerProfile_GetNamePtr(void *profile);
extern void CopyU16StringArrayN(u16 *dst, const u16 *src, u32 count);
extern void *Save_Frontier_GetTowerMessage(void *save, int index);
extern void MailMsg_Copy(void *dst, void *src);
extern u8 *Save_TrainerHouse_Get(void *save);
extern void ov112_021F3608(void *save, u8 *out);
extern void ov112_021F3630(void *save, const u8 *in);

void ov112_021F336C(u8 *dst, void *save);
void ov112_021F33D8(void *dst, void *party);
int ov112_021F35A4(u8 *p);
int ov112_021F35C8(u8 *p, void *trainer, int *found);

void ov112_021F336C(u8 *dst, void *save) {
    void *profile = Save_PlayerData_GetProfile(save);
    int i;
    *(u32 *)dst = PlayerProfile_GetTrainerID(profile);
    dst[7] = PlayerProfile_GetTrainerGender(profile);
    dst[5] = PlayerProfile_GetLanguage(profile);
    dst[6] = PlayerProfile_GetVersion(profile);
    dst[4] = PlayerProfile_GetAvatar(profile);
    CopyU16StringArrayN((u16 *)(dst + 8), PlayerProfile_GetNamePtr(profile), 7);
    i = 0;
    dst += 0x18;
    for (; i < 3; i++, dst += 8) {
        Save_Frontier_GetTowerMessage(save, i);
        MailMsg_Copy(dst, Save_Frontier_GetTowerMessage(save, i));
    }
}
