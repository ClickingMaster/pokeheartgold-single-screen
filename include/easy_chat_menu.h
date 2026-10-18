#ifndef POKEHEARTGOLD_EASY_CHAT_MENU_H
#define POKEHEARTGOLD_EASY_CHAT_MENU_H

#include "heap.h"
#include "mail_misc.h"
#include "pm_string.h"

// The words the player can pick in the easy chat app, per category and per initial.
typedef struct ECMenu ECMenu;

ECMenu *ECMenu_New(enum HeapID heapID, EasyChatArgs *args);
void ECMenu_Delete(ECMenu *menu);
u32 ECMenu_GetCategoryCount(ECMenu *menu, u32 category);
void ECMenu_GetCategoryWordString(ECMenu *menu, u32 category, u32 index, String *dest);
u16 ECMenu_GetCategoryWord(ECMenu *menu, u32 category, u32 index);
u32 ECMenu_GetInitialCount(ECMenu *menu, u32 group);
void ECMenu_GetInitialWordString(ECMenu *menu, u32 group, u32 index, String *dest);
u16 ECMenu_GetInitialWord(ECMenu *menu, u32 group, u32 index);

#endif // POKEHEARTGOLD_EASY_CHAT_MENU_H
