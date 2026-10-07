#ifndef TO52_OVERLAY89_R5_PRIVATE_H
#define TO52_OVERLAY89_R5_PRIVATE_H

#include "global.h"
#include "sprite_system.h"

typedef struct Ov89PartyEntry {
    u16 value;
    u16 species;
    u32 personality;
    u8 form;
    u8 padding[3];
} Ov89PartyEntry;

void *SaveArray_Party_Get(void *saveData);
int Party_GetCount(void *party);
void *Party_GetMonByIndex(void *party, int slot);
u32 GetMonData(void *mon, int attr, void *dest);
u16 ov89_0225C830(u32 column, u32 value);
void ov89_0225C724(void *msgData, void *format, void *windows, void *context, void *network);
BOOL ov89_0225A49C(void *context, void *work, const void *input, void *narc, void *narc2, void *network);
int ov45_0222EC7C(void *network);
BOOL ov89_02259A3C(int species, int form, ManagedSprite *sprite, NARC *narc, NARC *narc2, int mode, void *network);
void PlaySE(u16 seqNo);
void ov89_02259E50(u32 column, void *saveData, Ov89PartyEntry *entries);
BOOL ov89_02259EC4(u8 *context, Ov89PartyEntry *entry, void *network);
void ov89_0225A148(u8 *context, void *network);

#endif
