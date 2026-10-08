#pragma force_active on
#include "global.h"
#include "mail_message.h"
#include "player_data.h"
#include "pm_string.h"

typedef struct Ov34Entry {
    String *name;
    String *message;
    String *text;
    u32 unkC;
    u32 gender;
    MailMessage mailMsg;
} Ov34Entry;

typedef struct Ov34List {
    Ov34Entry entries[30];
    u32 count;
    u32 unk34C;
} Ov34List;

typedef struct Ov34Sub {
    u8 unk0[0x4D8];
    void *unk4D8;
} Ov34Sub;

typedef struct Ov34Unk {
    u8 unk0[4];
    Ov34Sub *sub;
    u8 unk8[8];
    u32 unk10;
    u8 unk14[4];
    u32 unk18;
    u32 unk1C;
    u8 unk20[0x250];
    Ov34List *list;
} Ov34Unk;

String *ov34_0225E2BC(void *a0, u32 a1, u32 a2, u32 a3, u32 a4);
void ov34_0225E348(Ov34Unk *data, u32 a1, MailMessage *mailMsg, PlayerProfile *profile);

// @fn ov34_0225E348
void ov34_0225E348(Ov34Unk *data, u32 a1, MailMessage *mailMsg, PlayerProfile *profile) {
    Ov34List *list = data->list;
    void *unk = data->sub->unk4D8;
    u32 *index;

    if (list->count == 30) {
        index = &list->unk34C;
    } else {
        index = &list->count;
    }
    if (list->entries[*index].message != NULL) {
        String_Delete(list->entries[*index].message);
    }
    if (list->entries[*index].text != NULL) {
        String_Delete(list->entries[*index].text);
    }
    CopyU16ArrayToString(list->entries[*index].name, PlayerProfile_GetNamePtr(profile));
    list->entries[*index].mailMsg = *mailMsg;
    list->entries[*index].unkC = a1;
    list->entries[*index].gender = PlayerProfile_GetTrainerGender(profile);
    list->entries[*index].message = MailMsg_GetExpandedString(mailMsg, HEAP_ID_87);
    list->entries[*index].text = ov34_0225E2BC(unk, a1, data->unk18, data->unk1C, data->unk10);
    (*index)++;
    if (list->unk34C == 30) {
        list->unk34C = 0;
    }
}
