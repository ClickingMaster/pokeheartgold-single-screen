#ifndef POKEHEARTGOLD_EASY_CHAT_APP_PANELS_H
#define POKEHEARTGOLD_EASY_CHAT_APP_PANELS_H

#include "easy_chat_app_internal.h"
#include "filesystem.h"
#include "sprite.h"
#include "touchscreen.h"

// The panels the easy chat screen (EasyChatAppView) is made of.
typedef struct EasyChatSentenceView EasyChatSentenceView;
typedef struct EasyChatMenuView EasyChatMenuView;
typedef struct EasyChatCategoryView EasyChatCategoryView;
typedef struct EasyChatWordListView EasyChatWordListView;
typedef struct EasyChatPageButtonsView EasyChatPageButtonsView;

EasyChatSentenceView *EasyChatSentenceView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void EasyChatSentenceView_Delete(EasyChatSentenceView *sentence);
void EasyChatSentenceView_Load(EasyChatSentenceView *sentence, NARC *narc);
void EasyChatSentenceView_StartScroll(EasyChatSentenceView *sentence, u8 direction);
BOOL EasyChatSentenceView_UpdateScroll(EasyChatSentenceView *sentence);
void EasyChatSentenceView_DrawSentence(EasyChatSentenceView *sentence);
void EasyChatSentenceView_GetSlotHitbox(EasyChatSentenceView *sentence, TouchscreenHitbox *hitbox, u32 slot);
u32 EasyChatSentenceView_GetNumSlots(EasyChatSentenceView *sentence);
void EasyChatSentenceView_SelectMessageWindow(EasyChatSentenceView *sentence, u32 index);
void EasyChatSentenceView_PrintMessage(EasyChatSentenceView *sentence, u32 message);
void EasyChatSentenceView_DeactivateCursor(EasyChatSentenceView *sentence);
void EasyChatSentenceView_ActivateCursor(EasyChatSentenceView *sentence);
void EasyChatSentenceView_SetCursorVisible(EasyChatSentenceView *sentence, BOOL draw);
void EasyChatSentenceView_HideSlider(EasyChatSentenceView *sentence);
void EasyChatSentenceView_ShowSlider(EasyChatSentenceView *sentence);
Sprite *EasyChatSentenceView_GetSliderThumb(EasyChatSentenceView *sentence);
void EasyChatSentenceView_MoveCursorToSlot(EasyChatSentenceView *sentence, u16 slot);
void EasyChatSentenceView_StartYesNo(EasyChatSentenceView *sentence, int initialCursorPos);
int EasyChatSentenceView_HandleYesNo(EasyChatSentenceView *sentence);
void EasyChatSentenceView_DrawYesNoCursor(EasyChatSentenceView *sentence, int index);
EasyChatMenuView *EasyChatMenuView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void EasyChatMenuView_Delete(EasyChatMenuView *menuView);
void EasyChatMenuView_Load(EasyChatMenuView *menuView, NARC *narc);
void EasyChatMenuView_DrawMainButtons(EasyChatMenuView *menuView, BOOL draw, BOOL showCursor);
void EasyChatMenuView_DeactivateCursor(EasyChatMenuView *menuView);
void EasyChatMenuView_ActivateCursor(EasyChatMenuView *menuView);
void EasyChatMenuView_SetCursorVisible(EasyChatMenuView *menuView, BOOL draw);
void EasyChatMenuView_MoveCursor(EasyChatMenuView *menuView, u16 cursor);
void EasyChatMenuView_StartModeSwitch(EasyChatMenuView *menuView);
BOOL EasyChatMenuView_IsModeSwitchDone(EasyChatMenuView *menuView);
void EasyChatMenuView_ShowListButton(EasyChatMenuView *menuView);
void EasyChatMenuView_ShowMainButtons(EasyChatMenuView *menuView);
EasyChatCategoryView *EasyChatCategoryView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void EasyChatCategoryView_Delete(EasyChatCategoryView *categories);
void EasyChatCategoryView_Load(EasyChatCategoryView *categories, NARC *narc);
void EasyChatCategoryView_SetCursorVisible(EasyChatCategoryView *categories, BOOL draw);
void EasyChatCategoryView_MoveCursor(EasyChatCategoryView *categories, u32 category);
void EasyChatCategoryView_StartOpen(EasyChatCategoryView *categories);
BOOL EasyChatCategoryView_UpdateOpen(EasyChatCategoryView *categories);
void EasyChatCategoryView_StartClose(EasyChatCategoryView *categories);
BOOL EasyChatCategoryView_UpdateClose(EasyChatCategoryView *categories);
void EasyChatCategoryView_ResetScrollY(EasyChatCategoryView *categories);
void EasyChatCategoryView_StartDim(EasyChatCategoryView *categories);
BOOL EasyChatCategoryView_UpdateDim(EasyChatCategoryView *categories);
void EasyChatCategoryView_StartFadeOut(EasyChatCategoryView *categories);
BOOL EasyChatCategoryView_UpdateFadeOut(EasyChatCategoryView *categories);
void EasyChatCategoryView_StartFadeIn(EasyChatCategoryView *categories);
BOOL EasyChatCategoryView_UpdateFadeIn(EasyChatCategoryView *categories);
void EasyChatCategoryView_ResetScrollX(EasyChatCategoryView *categories);
void EasyChatCategoryView_StartModeScroll(EasyChatCategoryView *categories);
BOOL EasyChatCategoryView_UpdateModeScroll(EasyChatCategoryView *categories);
EasyChatWordListView *EasyChatWordListView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void EasyChatWordListView_Delete(EasyChatWordListView *wordList);
void EasyChatWordListView_Load(EasyChatWordListView *wordList);
void EasyChatWordListView_DrawWords(EasyChatWordListView *wordList);
BOOL EasyChatWordListView_StartFadeIn(EasyChatWordListView *wordList);
BOOL EasyChatWordListView_UpdateFadeIn(EasyChatWordListView *wordList);
void EasyChatWordListView_StartFadeOut(EasyChatWordListView *wordList);
BOOL EasyChatWordListView_UpdateFadeOut(EasyChatWordListView *wordList);
void EasyChatWordListView_SetCursorVisible(EasyChatWordListView *wordList, BOOL draw);
void EasyChatWordListView_MoveCursor(EasyChatWordListView *wordList, u32 cursor);
void EasyChatWordListView_StartScroll(EasyChatWordListView *wordList, int rows);
BOOL EasyChatWordListView_UpdateScroll(EasyChatWordListView *wordList);
EasyChatPageButtonsView *EasyChatPageButtonsView_Create(EasyChatAppView *view, EasyChatApp *app, ECMenu *menu);
void EasyChatPageButtonsView_Delete(EasyChatPageButtonsView *pageButtons);
void EasyChatPageButtonsView_Load(EasyChatPageButtonsView *pageButtons, NARC *narc);
void EasyChatPageButtonsView_SetEnabled(EasyChatPageButtonsView *pageButtons, BOOL enabled);
void EasyChatPageButtonsView_SetPressed(EasyChatPageButtonsView *pageButtons, int button, int pressed);

#endif // POKEHEARTGOLD_EASY_CHAT_APP_PANELS_H
