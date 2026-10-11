#ifndef POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_INTERNAL_H

#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font_sprite.h"
#include "gf_3d_vramman.h"
#include "msgdata.h"
#include "palette.h"
#include "pokepic.h"
#include "save.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "touch_hitbox_controller.h"

// VS Recorder app (overlay 40): the VS Recorder item (mode 0) and the Global Terminal data browser (modes 1-6).
// The work is allocated by the main-ARM9 launcher (unk_02087E70.c), so only the fields known so far are named.

typedef struct VsRecorderWork VsRecorderWork;

// one step of a screen; TRUE when the screen is done
typedef BOOL (*VsRecorderScreenFunc)(VsRecorderWork *work);

// VsRecorderWork.screenPtr
#define VS_RECORDER_SCREEN_TITLE         0
#define VS_RECORDER_SCREEN_MENU          1
#define VS_RECORDER_SCREEN_SAVED_VIDEO   2
#define VS_RECORDER_SCREEN_FRONTIER      3
#define VS_RECORDER_SCREEN_DRESS_UP_SEND 4
#define VS_RECORDER_SCREEN_DRESS_UP_VIEW 5
#define VS_RECORDER_SCREEN_BOX_DATA_SEND 6
#define VS_RECORDER_SCREEN_BOX_DATA_VIEW 7
#define VS_RECORDER_SCREEN_RANKINGS      8
#define VS_RECORDER_SCREEN_RANKINGS_LAST 9
#define VS_RECORDER_SCREEN_COLOSSEUM_TOP 10
#define VS_RECORDER_SCREEN_FRONTIER_TOP  11
#define VS_RECORDER_SCREEN_NEWEST        12
#define VS_RECORDER_SCREEN_SEARCH        13
#define VS_RECORDER_SCREEN_SEARCH_NUMBER 14
#define VS_RECORDER_SCREEN_SEND_VIDEO    15
#define VS_RECORDER_SCREEN_EXIT          16
#define VS_RECORDER_SCREEN_DELETE        17
#define VS_RECORDER_SCREEN_MESSAGE       18
#define VS_RECORDER_SCREEN_NONE          0xFF

struct VsRecorderWork {
    u32 mode;                                // 0x0000, 0 VS Recorder item, 1-6 Global Terminal
    u32 subState;                            // 0x0004, index into the screen's function table
    u32 step;                                // 0x0008
    u32 counter;                             // 0x000C
    int *screenPtr;                          // 0x0010, OverlayManager state = current screen
    NARC *narc;                              // 0x0014, NARC_a_1_9_1
    SpriteSystem *spriteSystem;              // 0x0018
    SpriteManager *spriteManager;            // 0x001C
    u32 unk_0020;                            // 0x0020
    BgConfig *bgConfig;                      // 0x0024
    PaletteData *plttData;                   // 0x0028
    TouchHitboxController *hitboxController; // 0x002C
    u8 unk_0030[0x14];                       // 0x0030
    u32 unk_0044;                            // 0x0044, 1: the title goes on to the menu's substate 0
    MsgData *msgData;                        // 0x0048, msg_0013
    MsgData *recordMsgData;                  // 0x004C, msg_0026 (frontier record)
    FontSystem *fontSystem;                  // 0x0050
    u8 unk_0054[4];                          // 0x0054
    u32 blendColor;                          // 0x0058
    u8 colorScheme;                          // 0x005C, Save_Misc
    GF3DVramMan *vramMan;                    // 0x0060
    PokepicManager *pokepicManager;          // 0x0064
    u8 unk_0068[0x6F0 - 0x68];               // 0x0068, starts with the OVY_39 client work
    void *banners[2];                        // 0x06F0, top / bottom screen banner (sub_02087284)
    u8 unk_06F8[0x830 - 0x6F8];              // 0x06F8
    SaveData *saveData;                      // 0x0830
    u8 unk_0834[0x87C - 0x834];              // 0x0834
    void *savedVideos[4];                    // 0x087C
    void *videoOwners[4];                    // 0x088C
    u8 unk_089C[0x416C - 0x89C];             // 0x089C
    SysTask *task;                           // 0x416C
}; // size 0x4170

// vs_recorder.c
void VsRecorder_InitResources(VsRecorderWork *work);
void VsRecorder_FreeResources(VsRecorderWork *work);
void VsRecorder_InitBgLayers(BgConfig *bgConfig);
void VsRecorder_SetScreensFlipped(int flipped);
void VsRecorder_ResetBg2(VsRecorderWork *work);
void VsRecorder_InitSpriteSystem(VsRecorderWork *work);
void VsRecorder_VBlankCB(void *data);
BOOL VsRecorder_Main(VsRecorderWork *work, int *screen);

// other OVY_40 modules (still address-named)
void ov40_0222BF64(VsRecorderWork *work, int nextScreen, BOOL done, int *screen);
void ov40_0222BF80(VsRecorderWork *work, int subState);
void ov40_0222C360(VsRecorderWork *work);
void ov40_0222C4F8(VsRecorderWork *work);
void ov40_0222FBF8(VsRecorderWork *work);
void ov40_0222FCCC(VsRecorderWork *work);
void ov40_02230D20(VsRecorderWork *work);
void ov40_0223D544(VsRecorderWork *work);
void ov40_0223D5E8(VsRecorderWork *work);
void ov40_0223D600(VsRecorderWork *work);

#endif // POKEHEARTGOLD_APPLICATION_VS_RECORDER_VS_RECORDER_INTERNAL_H
