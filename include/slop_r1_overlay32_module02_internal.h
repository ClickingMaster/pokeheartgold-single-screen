#ifndef SLOP_R1_OVERLAY32_MODULE02_INTERNAL_H
#define SLOP_R1_OVERLAY32_MODULE02_INTERNAL_H

#include <nitro/gx/gx_vramcnt.h>

#include "global.h"

#include "bg_window.h"
#include "field_system.h"
#include "filesystem.h"
#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "unk_0202C034.h"

#define NAME_PICKER_NUM_WINDOWS    18
#define NAME_PICKER_NUM_SPRITES    4
#define NAME_PICKER_NAMES_PER_PAGE 8
#define NAME_PICKER_NUM_PAGES      4

#define NAME_PICKER_SPRITE_CURSOR 0
#define NAME_PICKER_SPRITE_CANCEL 1
#define NAME_PICKER_INPUT_CANCEL  8

enum NamePickerState {
    NAME_PICKER_STATE_INPUT = 0,
    NAME_PICKER_STATE_BLINK,
    NAME_PICKER_STATE_REDRAW,
    NAME_PICKER_STATE_DONE,
};

typedef struct NamePickerBlink {
    u8 isBg : 1;
    u8 target : 7;
    u8 seqA : 4;
    u8 seqB : 4;
    u8 phase;
    u8 timer;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} NamePickerBlink;

typedef struct NamePickerData {
    BgConfig *bgConfig;
    void *unk4;
    FieldSystem *fieldSystem;
    int *unkC;
    SysTask *task;
    UnkStruct_021D2230 *names;
    GridInputHandler *gridInput;
    NamePickerBlink blink;
    Window windows[NAME_PICKER_NUM_WINDOWS];
    MsgData *msgData;
    MessageFormat *msgFormat;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMans[4];
    SpriteResource *resObjs[4];
    Sprite *sprites[NAME_PICKER_NUM_SPRITES];
    u8 state;
    u8 nextState;
    s8 page;
    u8 bufferToggle;
    int selection;
    int *result;
} NamePickerData;

extern const TouchscreenHitbox _0225E15C[];
extern const GridCallbacks ov32_0225E168;
extern const TouchscreenHitbox ov32_0225E1DC[];
extern const DpadMenuBox ov32_0225E204[];
extern const WindowTemplate ov32_0225E24C[];

SysTask *ov32_0225D520(BgConfig *bgConfig, void *a1, FieldSystem *fieldSystem, int *result);
void ov32_0225D60C(void);
void ov32_0225D634(BgConfig *bgConfig);
void ov32_0225D6E0(NamePickerData *data, NARC *narc);
void ov32_0225D748(NamePickerData *data);
void ov32_0225D788(NamePickerData *data);
void ov32_0225D84C(NamePickerData *data);
void ov32_0225D988(NamePickerData *data);
void ov32_0225DA88(NamePickerData *data, NARC *narc);
void ov32_0225DADC(NamePickerData *data);
void ov32_0225DC68(NamePickerData *data, NARC *narc);
void ov32_0225DD04(NamePickerData *data);
void ov32_0225DD24(NamePickerData *data, int pos);
void ov32_0225DD74(NamePickerData *data);
void ov32_0225DDC4(SysTask *task, void *taskData);
int ov32_0225DE34(NamePickerData *data);
int ov32_0225DF80(NamePickerData *data);
int ov32_0225DF9C(NamePickerData *data, int delta);
int ov32_0225E0A8(NamePickerData *data, u8 target, u8 seqA, u8 seqB, u8 nextState);
int ov32_0225E0FC(NamePickerData *data, u8 x, u8 y, u8 nextState);

#endif
