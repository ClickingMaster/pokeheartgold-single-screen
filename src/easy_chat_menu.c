#include "global.h"

#include "easy_chat.h"
#include "heap.h"
#include "mail_misc.h"
#include "pokedex.h"

typedef struct ECMenu ECMenu;

typedef u32 (*ECMenuBuildFunc)(ECMenu *menu, const u16 *words, u32 count, u16 *dest);

typedef struct ECMenuSpec {
    ECMenuBuildFunc func;
    const u16 *words;
    u32 count;
} ECMenuSpec;

// The words the player can pick: per category (12, e.g. Pokemon, moves, trendy sayings) and per initial (27
// groups, A-Z + others), each list in one array with an offset and a count per list. Total 0x195C.
struct ECMenu {
    EasyChatArgs *args;       // 0x0000
    EasyChatManager *manager; // 0x0004
    u32 categoryCounts[12];   // 0x0008
    u32 categoryOffsets[12];  // 0x0038
    u16 categoryWords[1496];  // 0x0068
    u32 initialCounts[27];    // 0x0C18
    u32 initialOffsets[27];   // 0x0C84
    u16 initialWords[1495];   // 0x0CF0
    u8 knownWords[0xBB];      // 0x189E .. 0x1958, padded to 0x195C
};

ECMenu *ECMenu_New(enum HeapID heapID, EasyChatArgs *args);
void ECMenu_Delete(ECMenu *menu);
void ECMenu_BuildCategoryLists(ECMenu *menu);
u32 ECMenuBuild_Pokemon(ECMenu *menu, const u16 *words, u32 count, u16 *dest);
u32 ECMenuBuild_Move(ECMenu *menu, const u16 *words, u32 count, u16 *dest);
u32 ECMenuBuild_TrendySayings(ECMenu *menu, const u16 *words, u32 count, u16 *dest);
u32 ECMenuBuild_Greetings(ECMenu *menu, const u16 *words, u32 count, u16 *dest);
u32 ECMenuBuild_Generic(ECMenu *menu, const u16 *words, u32 count, u16 *dest);
void ECMenu_BuildInitialLists(ECMenu *menu);
u32 ECMenu_CollectKnownWords(ECMenu *menu, const u16 *words, u16 *dest, u32 destIndex);
u32 ECMenu_GetCategoryCount(ECMenu *menu, u32 category);
void ECMenu_GetCategoryWordString(ECMenu *menu, u32 category, u32 index, String *dest);
u16 ECMenu_GetCategoryWord(ECMenu *menu, u32 category, u32 index);
u32 ECMenu_GetInitialCount(ECMenu *menu, u32 group);
void ECMenu_GetInitialWordString(ECMenu *menu, u32 group, u32 index, String *dest);
u16 ECMenu_GetInitialWord(ECMenu *menu, u32 group, u32 index);

extern const u16 *const gECMenuInitialLists[];
extern const ECMenuSpec sECMenuSpecs[12];

ECMenu *ECMenu_New(enum HeapID heapID, EasyChatArgs *args) {
    ECMenu *menu = Heap_Alloc(heapID, sizeof(ECMenu));
    s32 i;

    menu->args = args;
    menu->manager = EasyChatManager_New(heapID);
    for (i = 0; i < 0xBB; i++) {
        menu->knownWords[i] = 0;
    }
    ECMenu_BuildCategoryLists(menu);
    ECMenu_BuildInitialLists(menu);
    return menu;
}

void ECMenu_Delete(ECMenu *menu) {
    if (menu != NULL) {
        EasyChatManager_Delete(menu->manager);
        Heap_Free(menu);
    }
}

#pragma force_active on

void ECMenu_BuildCategoryLists(ECMenu *menu) {
    int i;
    u32 total;
    u32 n;

    i = 0;
    total = 0;
    for (; i < 12; i++) {
        menu->categoryOffsets[i] = total;
        n = sECMenuSpecs[i].func(menu, sECMenuSpecs[i].words, sECMenuSpecs[i].count, &menu->categoryWords[total]);
        menu->categoryCounts[i] = n;
        total += n;
    }
}

#pragma force_active off

u32 ECMenuBuild_Pokemon(ECMenu *menu, const u16 *words, u32 count, u16 *dest) {
    u32 n = 0;
    u32 i;
    Pokedex *pokedex = EasyChatArgs_GetPokedex(menu->args);

    for (i = 0; i < count; i++) {
        if (Pokedex_CheckMonSeenFlag(pokedex, words[i])) {
            u32 word = words[i];
            menu->knownWords[word >> 3] |= 1 << (word & 7);
            n++;
            *dest = words[i];
            dest++;
        }
    }
    return n;
}

u32 ECMenuBuild_Move(ECMenu *menu, const u16 *words, u32 count, u16 *dest) {
    u32 i;

    if (EasyChatArgs_GetGameCleared(menu->args)) {
        for (i = 0; i < count; i++) {
            u32 word = words[i];
            menu->knownWords[word >> 3] |= 1 << (word & 7);
            *dest = words[i];
            dest++;
        }
        return count;
    }
    return 0;
}

u32 ECMenuBuild_TrendySayings(ECMenu *menu, const u16 *words, u32 count, u16 *dest) {
    u32 i;
    u32 n;
    SaveEasyChat *saveEasyChat = EasyChatArgs_GetEasyChat(menu->args);
    u32 base = GetECWordIndexByPair(0x122, 0); // NARC_msg_msg_0290_bin

    i = 0;
    n = i;
    for (; i < count; i++) {
        if (Save_EasyChat_GetTrendySayingFlag(saveEasyChat, words[i] - base)) {
            u32 word = words[i];
            menu->knownWords[word >> 3] |= 1 << (word & 7);
            n++;
            *dest = words[i];
            dest++;
        }
    }
    return n;
}

u32 ECMenuBuild_Greetings(ECMenu *menu, const u16 *words, u32 count, u16 *dest) {
    u32 n;

    struct {
        u32 i;
        u32 last;
        SaveEasyChat *saveEasyChat;
    } locals;

    locals.saveEasyChat = EasyChatArgs_GetEasyChat(menu->args);
    u16 base = GetECWordIndexByPair(0x11F, 0); // NARC_msg_msg_0289_bin
    locals.last = (u16)(base + 5);

    n = 0;
    for (locals.i = 0; locals.i < count; locals.i++) {
        if (words[locals.i] < base || words[locals.i] > locals.last || Save_EasyChat_GetGreetingsFlag(locals.saveEasyChat, words[locals.i] - base)) {
            u32 word = words[locals.i];
            menu->knownWords[word >> 3] |= 1 << (word & 7);
            n++;
            *dest = words[locals.i];
            dest++;
        }
    }
    return n;
}

u32 ECMenuBuild_Generic(ECMenu *menu, const u16 *words, u32 count, u16 *dest) {
    u32 i;

    for (i = 0; i < count; i++) {
        if (*words == 0xFFFE) {
            const u16 *groupSizePtr = words + 1;
            u32 groupSize;
            words += 2;
            groupSize = *groupSizePtr;
            *dest = *words;
            while (groupSize != 0) {
                u32 word = *words;
                menu->knownWords[word >> 3] |= 1 << (word & 7);
                words++;
                groupSize--;
            }
        } else {
            u32 word = *words;
            menu->knownWords[word >> 3] |= 1 << (word & 7);
            *dest = *words;
            words++;
            dest++;
        }
    }
    return count;
}

void ECMenu_BuildInitialLists(ECMenu *menu) {
    u32 total = 0;
    u32 i;

    for (i = 0; i < 27; i++) {
        menu->initialOffsets[i] = total;
        menu->initialCounts[i] = ECMenu_CollectKnownWords(menu, gECMenuInitialLists[i], menu->initialWords, total);
        total += menu->initialCounts[i];
    }
}

u32 ECMenu_CollectKnownWords(ECMenu *menu, const u16 *words, u16 *dest, u32 destIndex) {
    u32 n = 0;

    while (*words != 0xFFFF) {
        if (*words == 0xFFFE) {
            const u16 *groupSizePtr = words + 1;
            u32 groupSize;
            u32 j;
            words += 2;
            groupSize = *groupSizePtr;
            for (j = 0; j < groupSize; j++) {
                u32 word = words[j];
                if (menu->knownWords[word >> 3] & (1 << (word & 7))) {
                    dest[destIndex++] = words[j];
                    n++;
                    break;
                }
            }
            words += groupSize;
        } else {
            if (menu->knownWords[(u32)*words >> 3] & (1 << (*words & 7))) {
                dest[destIndex++] = *words;
                n++;
            }
            words++;
        }
    }
    return n;
}

u32 ECMenu_GetCategoryCount(ECMenu *menu, u32 category) {
    return menu->categoryCounts[category];
}

void ECMenu_GetCategoryWordString(ECMenu *menu, u32 category, u32 index, String *dest) {
    u32 base = menu->categoryOffsets[category];
    EasyChatManager_ReadWordIntoString(menu->manager, menu->categoryWords[base + index], dest);
}

u16 ECMenu_GetCategoryWord(ECMenu *menu, u32 category, u32 index) {
    u32 base = menu->categoryOffsets[category];
    return menu->categoryWords[base + index];
}

u32 ECMenu_GetInitialCount(ECMenu *menu, u32 group) {
    return menu->initialCounts[group];
}

void ECMenu_GetInitialWordString(ECMenu *menu, u32 group, u32 index, String *dest) {
    if (menu->initialCounts[group] != 0) {
        EasyChatManager_ReadWordIntoString(menu->manager, ECMenu_GetInitialWord(menu, group, index), dest);
    }
}

u16 ECMenu_GetInitialWord(ECMenu *menu, u32 group, u32 index) {
    if (menu->initialCounts[group] != 0) {
        return menu->initialWords[menu->initialOffsets[group] + index];
    }
    return 0xFFFF;
}
