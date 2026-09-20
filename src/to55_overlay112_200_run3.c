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
extern void *sub_0202D660(void *save, int index);
extern void MailMsg_Copy(void *dst, void *src);
extern u8 *Save_TrainerHouse_Get(void *save);
extern void ov112_021F3608(void *save, u8 *out);
extern void ov112_021F3630(void *save, const u8 *in);

void ov112_021F336C(u8 *dst, void *save);
void ov112_021F33D8(void *dst, void *party);
int ov112_021F35A4(u8 *p);
int ov112_021F35C8(u8 *p, void *trainer, int *found);

int ov112_021F35A4(u8 *p) {
    int i = 0;
    int result;
    result = i;
    for (; i < 10; i++, p += 0x180, result++) {
        if (!TrainerHouseSet_CheckHasData(p)) {
            break;
        }
    }
    return result;
}

int ov112_021F35C8(u8 *p, void *trainer, int *found) {
    int i;
    for (i = 0; i < 10; i++, p += 0x180) {
        if (TrainerHouseSet_CheckHasData(p) && TrainerHouseTrainer_Compare(p, trainer)) {
            *found = TRUE;
            return i;
        }
    }
    *found = FALSE;
    return 0;
}
