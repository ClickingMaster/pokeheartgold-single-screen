#ifndef TO52_OV85_GAP4_H
#define TO52_OV85_GAP4_H
#include "overlay85_arcade_scene_core_private.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "msgdata.h"
#include "message_format.h"
#include "filesystem.h"
#include "sound_02004A44.h"
#include "touchscreen.h"
#include "palette.h"
#include "sound.h"
#include "system.h"

typedef struct ArcArgs85 {
    u8 pad_00[0x30];
    void *unk_30;
    u8 pad_34[4];
    ArcScene *scene;
} ArcArgs85;
extern void *OverlayManager_GetArgs(void *);
extern void *OverlayManager_CreateAndGetData(void *, u32, enum HeapID);
extern void HBlankInterruptDisable(void);
extern void GfGfx_DisableEngineAPlanes(void);
extern void GfGfx_DisableEngineBPlanes(void);
extern void SetKeyRepeatTimers(int, int);
extern void ov85_021E8C3C(void);
extern void ov85_021E8C5C(BgConfig *);
extern void ov85_021E8E6C(ArcScene *, NARC *);
extern void ov85_021E8D64(ArcScene *, NARC *);
extern void ov85_021E8F58(void);
extern void ov85_021E8F88(ArcScene *, NARC *);
extern void ov85_021E9084(ArcScene *);
extern void ov85_021E9160(ArcScene *);
extern void ov85_021E8C14(void *);
extern void ov85_021E8BB0(SysTask *, void *);
extern int ov85_021E9FD0(void);
extern BOOL ov85_021E9EC0(Window *, u32, u32, ArcScene *);
extern void ov85_021EA1AC(ArcScene *);
extern int ov85_021EA3F0(ArcScene *, int);
extern int (*ov85_021EA9E0[])(ArcScene *);
extern u32 sub_02033250(void);
extern void sub_0203897C(void);
extern void sub_02038C1C(int);
extern void sub_0203A880(void);
extern void sub_02096CE0(void *);
extern BOOL sub_02096D4C(void *, int, void *, int);
extern BOOL ov85_021E88AC(void *, int *);
extern BOOL ov85_021E8A08(void *, int *);
extern BOOL IsPaletteFadeFinished(void);
#endif
