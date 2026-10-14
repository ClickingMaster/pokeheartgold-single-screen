#ifndef POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_UI_H
#define POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_UI_H

#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_course_objects.h"
#include "pokeathlon/pokeathlon_course_types_def.h"

#include "bg_window.h"
#include "filesystem.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "sprite.h"
#include "sprite_system.h"

// Screen helpers shared by the course screens: the banner with an event's picture and name,
// the commentator's message box, and pictures of trainers and Pokemon loaded into sprites.

// Banner of an event (NARC a/2/2/1 members by event, msg 309 names): picture on sub BG 3, name
// and place windows.
struct PokeathlonEventBanner {
    enum HeapID heapId;  // 0x00
    BgConfig *bgConfig;  // 0x04
    u32 event;           // 0x08
    NARC *narc;          // 0x0C
    Window nameWindow;   // 0x10
    Window placeWindow;  // 0x20
}; // size: 0x30

// The message box of the commentator (msg 309, and msg 311 for the lines of the NPC teams).
struct PokeathlonCourseMessageBox {
    BgConfig *bgConfig;       // 0x00
    MessageFormat *msgFormat; // 0x04
    MsgData *msgData;         // 0x08: msg 309
    MsgData *npcMsgData;      // 0x0C: msg 311
    String *string;           // 0x10
    enum HeapID heapId;       // 0x14
    Window window;            // 0x18
    BOOL linkMode;            // 0x28: keep the box busy a little longer after printing
    s32 printerId;            // 0x2C
    WaitingIcon *waitingIcon; // 0x30
    u32 holdTimer : 16;       // 0x34
    u32 textSpeed : 8;
    u32 bgLayer : 3;
    u32 unk34_27 : 1;
    u32 busy : 1;
    u32 unk34_29 : 3;
}; // size: 0x38

// Defined in pokeathlon_event_intro.c.
void PokeathlonCourse_LoadSpritePalette(ManagedSprite *sprite, NarcId narcId, int fileId, BOOL mainScreen, BOOL grayScale, enum HeapID heapId);
int PokeathlonCourse_GetNpcTrainerPicFileId(int trainerClass);

BOOL PokeathlonCourseCeremony_IsOpeningFinished(PokeathlonCourseData *course);
BOOL PokeathlonCourseCeremony_IsClosingFinished(PokeathlonCourseData *course);
PokeathlonEventBanner *PokeathlonEventBanner_New(u32 event, enum HeapID heapId);
void PokeathlonEventBanner_Free(PokeathlonEventBanner *banner);
void PokeathlonEventBanner_AddWindows(PokeathlonEventBanner *banner, BgConfig *bgConfig);
void PokeathlonEventBanner_LoadGraphics(PokeathlonEventBanner *banner);
void PokeathlonEventBanner_PrintTexts(PokeathlonEventBanner *banner);
void PokeathlonEventBanner_PrintText(Window *window, MsgData *msgData, String *string, u32 msgId, u32 font);
PokeathlonCourseMessageBox *PokeathlonCourseMessageBox_New(enum HeapID heapId);
void PokeathlonCourseMessageBox_Init(PokeathlonCourseMessageBox *box, BgConfig *bgConfig, u32 bgLayer, u32 linkMode, SaveData *saveData);
void PokeathlonCourseMessageBox_Free(PokeathlonCourseMessageBox *box);
BOOL PokeathlonCourseMessageBox_Update(PokeathlonCourseMessageBox *box);
void PokeathlonCourseMessageBox_Print(PokeathlonCourseMessageBox *box, u32 msgId);
void PokeathlonCourseMessageBox_PrintNpcLine(PokeathlonCourseMessageBox *box, u32 msgId);
void PokeathlonCourseMessageBox_Close(PokeathlonCourseMessageBox *box);
MessageFormat *PokeathlonCourseMessageBox_GetMsgFormat(PokeathlonCourseMessageBox *box);
void PokeathlonCourseMessageBox_ShowWaitingIcon(PokeathlonCourseMessageBox *box);
void PokeathlonCourseMessageBox_HideWaitingIcon(PokeathlonCourseMessageBox *box);
void PokeathlonCourseMessageBox_LoadFrame(BgConfig *bgConfig, int layer, enum HeapID heapId);
u8 PokeathlonCourseMessageBox_PrintToWindow(BgConfig *bgConfig, Window *window, String **string, u32 textSpeed, MessageFormat *msgFormat, MsgData *msgData, u32 msgId, u8 bgLayer, enum HeapID heapId);
BOOL PokeathlonCourseMessageBox_IsBusy(PokeathlonCourseMessageBox *box);
void PokeathlonCourse_LoadTrainerPic(ManagedSprite *sprite, int fileId, int index, enum HeapID heapId);
void PokeathlonCourse_LoadTrainerPicIntoSprite(Sprite *sprite, int fileId, int index, enum HeapID heapId);
void PokeathlonCourse_LoadTeamTrainerPic(ManagedSprite **sprites, PokeathlonCourseData *course, u8 team, int index);
void PokeathlonCourse_LoadPlayerTrainerPic(Sprite *sprite, PokeathlonCourseData *course, u32 team, u32 index);
u16 PokeathlonCourse_GetTrainerPicFileId(u32 kind);
void PokeathlonCourse_LoadMonPic(ManagedSprite *sprite, PokeathlonCourseObjectInfo *info, BOOL mainScreen, u32 palette, enum HeapID heapId);
void PokeathlonCourse_LoadMonPicWithPalette(ManagedSprite *sprite, PokeathlonCourseObjectInfo *info, BOOL mainScreen, u32 palette, enum HeapID heapId, BOOL loadPalette);
void PokeathlonCourse_LoadMonPicEx(ManagedSprite *sprite, PokeathlonCourseObjectInfo *info, BOOL mainScreen, u32 grayScale, enum HeapID heapId, BOOL loadPalette);
void PokeathlonCourse_LoadMonIcon(ManagedSprite *sprite, const u16 *monForm, int palette, NARC *narc, u32 offset, enum HeapID heapId);
void PokeathlonCourse_LoadSpriteCharsAt(Sprite *sprite, void *data, u32 size, u32 offset);
void PokeathlonCourse_LoadSpriteChars(Sprite *sprite, void *data, u32 size);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_UI_H
