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
    enum HeapID heapId;
    BgConfig *bgConfig;
    u32 event;
    NARC *narc;
    Window nameWindow;
    Window placeWindow;
};

// The message box of the commentator (msg 309, and msg 311 for the lines of the NPC teams).
struct PokeathlonCourseMessageBox {
    BgConfig *bgConfig;
    MessageFormat *msgFormat;
    MsgData *msgData;    // msg 309
    MsgData *npcMsgData; // msg 311
    String *string;
    enum HeapID heapId;
    Window window;
    BOOL linkMode; // keep the box busy a little longer after printing
    s32 printerId;
    WaitingIcon *waitingIcon;
    u32 holdTimer : 16;
    u32 textSpeed : 8;
    u32 bgLayer : 3;
    u32 unk34_27 : 1;
    u32 busy : 1;
    u32 unk34_29 : 3;
};

void PokeathlonCourse_LoadSpritePalette(ManagedSprite *sprite, NarcId narcId, int fileId, BOOL mainScreen, BOOL grayScale, enum HeapID heapId);
int PokeathlonCourse_GetNpcTrainerPicFileId(int trainerClass);

BOOL PokeathlonCourseCeremony_IsOpeningFinished(PokeathlonCourseData *course);
BOOL PokeathlonCourseCeremony_IsClosingFinished(PokeathlonCourseData *course);
PokeathlonEventBanner *PokeathlonEventBanner_New(u32 event, enum HeapID heapId);
void PokeathlonEventBanner_Free(PokeathlonEventBanner *banner);
void PokeathlonEventBanner_AddWindows(PokeathlonEventBanner *banner, BgConfig *bgConfig);
void PokeathlonEventBanner_LoadGraphics(PokeathlonEventBanner *banner);
void PokeathlonEventBanner_PrintTexts(PokeathlonEventBanner *banner);
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
BOOL PokeathlonCourseMessageBox_IsBusy(PokeathlonCourseMessageBox *box);
void PokeathlonCourse_LoadTrainerPic(ManagedSprite *sprite, int fileId, int index, enum HeapID heapId);
void PokeathlonCourse_LoadTrainerPicIntoSprite(Sprite *sprite, int fileId, int index, enum HeapID heapId);
void PokeathlonCourse_LoadTeamTrainerPic(ManagedSprite **sprites, PokeathlonCourseData *course, u8 team, int index);
void PokeathlonCourse_LoadPlayerTrainerPic(Sprite *sprite, PokeathlonCourseData *course, u32 team, u32 index);
u16 PokeathlonCourse_GetTrainerPicFileId(u32 kind);
void PokeathlonCourse_LoadMonPic(ManagedSprite *sprite, PokeathlonCourseMonInfo *info, BOOL mainScreen, u32 palette, enum HeapID heapId);
void PokeathlonCourse_LoadMonPicWithPalette(ManagedSprite *sprite, PokeathlonCourseMonInfo *info, BOOL mainScreen, u32 palette, enum HeapID heapId, BOOL loadPalette);
void PokeathlonCourse_LoadMonIcon(ManagedSprite *sprite, const u16 *monForm, int palette, NARC *narc, u32 offset, enum HeapID heapId);

#endif // POKEHEARTGOLD_POKEATHLON_POKEATHLON_COURSE_UI_H
