#include "global.h"

#define U8(p, n) (*(u8 *)((u8 *)(p) + (n)))
#define U16(p, n) (*(u16 *)((u8 *)(p) + (n)))
#define U32(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define PTR(p, n) ((void *)((u8 *)(p) + (n)))

typedef struct { u32 v[4]; } ScreenMode;
typedef struct { u32 v[7]; } BgTemplateRaw;
extern const ScreenMode ov67_021E6D5C;
extern const BgTemplateRaw ov67_021E6DA8;
extern const BgTemplateRaw ov67_021E6DFC;
extern const BgTemplateRaw ov67_021E6DC4;
extern const BgTemplateRaw ov67_021E6DE0;
extern void *BgConfig_Alloc(int);
extern void SetBothScreensModesAndDisable(const void *);
extern void InitBgFromTemplate(void *, int, const void *, int);
extern void BG_ClearCharDataRange(int, int, int, int);
extern void BgClearTilemapBufferAndCommit(void *, int);
extern const u8 ov67_021E6D50[];
extern u32 GridInputHandler_HandleInput_NoHold(void *);
extern void PlaySE(int);
extern void ov67_021E6BB8(void *);
extern void ov67_021E6B6C(void *, int);
extern int System_GetTouchNew(void);
extern int TouchscreenHitbox_FindRectAtTouchNew(const void *);
extern void ov67_021E6490(void *);
extern int GridInputHandler_GetNextInput(void *);
extern void ov67_021E6688(void *, u32);
extern void ov67_021E6530(void *);

int ov67_021E5C44(void *);
void ov67_021E5DA0(void *);
