#ifndef POKEHEARTGOLD_FRONTIER_CARD_H
#define POKEHEARTGOLD_FRONTIER_CARD_H

#include "heap.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"

// A player's card as shown by the VS Recorder: name, gender, partner Pokemon, location, greeting and avatar.
typedef struct FrontierCard FrontierCard;

FrontierCard *FrontierCard_New(enum HeapID heapId);
void FrontierCard_Delete(FrontierCard *card);
void FrontierCard_InitFromSave(FrontierCard *card, SaveData *saveData);
String *FrontierCard_NewNameString(const u16 *name, enum HeapID heapId); // the name is the card's first field
u32 FrontierCard_GetGender(FrontierCard *card);
u32 FrontierCard_GetSpecies(FrontierCard *card);
u32 FrontierCard_GetSanitizedForm(FrontierCard *card);
u32 FrontierCard_IsEgg(FrontierCard *card);
u32 FrontierCard_GetCountry(FrontierCard *card);
u32 FrontierCard_GetRegion(FrontierCard *card);
String *FrontierCard_GetGreeting(FrontierCard *card, MailMessage *message, enum HeapID heapId);
u32 FrontierCard_GetBirthdayMonth(FrontierCard *card);
u32 FrontierCard_GetAvatar(FrontierCard *card);

#endif // POKEHEARTGOLD_FRONTIER_CARD_H
