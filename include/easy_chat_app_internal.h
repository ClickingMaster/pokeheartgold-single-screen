#ifndef POKEHEARTGOLD_EASY_CHAT_APP_INTERNAL_H
#define POKEHEARTGOLD_EASY_CHAT_APP_INTERNAL_H

#include "heap.h"
#include "menu_input_state.h"
#include "pm_string.h"
#include "sprite_system.h"
#include "touchscreen.h"

// Overlay 102 (easy chat): the app logic (EasyChatApp) drives a screen object (EasyChatAppView) by starting
// numbered view commands; the view reads the app state back through the getters below.
typedef struct EasyChatApp EasyChatApp;
typedef struct EasyChatAppView EasyChatAppView;
typedef struct ECMenu ECMenu;

EasyChatAppView *ov102_021E909C(EasyChatApp *app, ECMenu *menu);
void ov102_021E9144(EasyChatAppView *view);
void ov102_021E940C(EasyChatAppView *view, u32 command);
BOOL ov102_021E9464(EasyChatAppView *view);
BOOL ov102_021E947C(EasyChatAppView *view, u32 command);
Sprite *ov102_021EA228(EasyChatAppView *view);
u32 ov102_021EA238(EasyChatAppView *view);
void ov102_021EA248(EasyChatAppView *view, TouchscreenHitbox *hitbox, u8 slot);
int ov102_021EA258(EasyChatAppView *view);
u8 ov102_021EC4F8(u32 category);
u8 ov102_021EC514(u32 category);
u8 ov102_021EC530(u32 category);
u8 ov102_021EC54C(u32 category);
u8 ov102_021EC568(u32 category);
u8 ov102_021EC584(u32 category);
int ov102_021EC5A0(u32 x, u32 y);

MenuInputState *EasyChatApp_GetMenuInputStatePtr(EasyChatApp *app);
u32 EasyChatApp_GetType(EasyChatApp *app);
u8 ov102_021E8F70(EasyChatApp *app);
u8 EasyChatApp_GetListMode(EasyChatApp *app);
u16 EasyChatApp_GetMsgBank(EasyChatApp *app);
u16 EasyChatApp_GetWord(EasyChatApp *app, int slot);
String *EasyChatApp_GetMessageString(EasyChatApp *app, enum HeapID heapID);
u8 EasyChatApp_GetMessageIndex(EasyChatApp *app);
u16 EasyChatApp_GetWordSlot(EasyChatApp *app);
u16 EasyChatApp_GetMenuCursor(EasyChatApp *app);
u16 EasyChatApp_GetCategory(EasyChatApp *app);
u32 EasyChatApp_GetCategoryWordCount(EasyChatApp *app);
void EasyChatApp_GetCategoryWord(EasyChatApp *app, u32 index, String *dest);
int EasyChatApp_GetWordListCursor(EasyChatApp *app);
s32 EasyChatApp_GetWordListScroll(EasyChatApp *app);
BOOL EasyChatApp_CanScrollWordListUp(EasyChatApp *app);
BOOL EasyChatApp_CanScrollWordListDown(EasyChatApp *app);
u32 ov102_021E9084(EasyChatApp *app);
u8 ov102_021E9090(EasyChatApp *app);

#endif // POKEHEARTGOLD_EASY_CHAT_APP_INTERNAL_H
