#include "global.h"

typedef struct Overlay05State {
    u8 data[0xc00];
} Overlay05State;

extern void PlaySE(int);
extern void ManagedSprite_SetDrawFlag(void *, int);
extern void sub_020880CC(int, void *);
extern void Comm_GetLocalNetId(void *);
extern int sub_020378AC(void);
extern void DoScheduledBgGpuUpdates(void *);
extern void PaletteData_PushTransparentBuffers(void *);
extern void GF_RunVramTransferTasks(void);
extern void SpriteSystem_TransferOam(void);
extern void RemoveWindow(void *);
extern void String_Delete(void *);
extern void MessageFormat_Delete(void *);
extern void DestroyMsgData(void *);
extern void TextOBJ_SetPosition(void *, int, int);
extern void FontOAM_Delete(void *);
extern void FontSystem_Delete(void *);
extern void Thunk_G3X_Reset(void);
extern int ParticleSystem_GetActiveEmitterCount(void *);
extern void ParticleSystem_DrawAll(void);
extern void ParticleSystem_CalcAll(void);
extern void ParticleSystem_Free(void *);
extern void Heap_Free(void *);
extern void GF_3DVramMan_Delete(void *);
extern void ParticleSystem_RegisterTexKey(void);
extern void ParticleSystem_RegisterPlttKey(void);
extern void GF_AssertFail(void);
extern void GfGfx_EngineATogglePlanes(int, int);
extern void FreeBgTilemapBuffer(void *, int);
extern void AddWindow(void *, void *, const void *);
extern void FillWindowPixelRect(void *, int, int, int, int, int);
extern void DrawFrameAndWindow2(void *, int, int, int);
extern int sub_0202FC48(void *);
extern void *WaitingIcon_New(void *, int);
extern void WaitingIcon_Delete(void *);
extern const u8 ov05_0221ECE4[];
extern int IsPaletteFadeFinished(void);
typedef struct Banks40 {
    u32 words[10];
} Banks40;
extern const Banks40 ov05_0221EC00;
extern void GfGfx_SetBanks(const void *);
extern void ManagedSprite_SetAnim(void *, int);

int ov05_0221C558(Overlay05State *);
int ov05_0221C7FC(Overlay05State *);
int ov05_0221C8E0(Overlay05State *);
int ov05_0221CC58(Overlay05State *);
int ov05_0221CCF4(void *);
void ov05_0221CE88(Overlay05State *);
void ov05_0221CFF0(Overlay05State *);
void ov05_0221D020(Overlay05State *);
void ov05_0221D228(void);
void ov05_0221D664(Overlay05State *, int, int, int);
void ov05_0221D690(Overlay05State *);
int ov05_0221DAE0(Overlay05State *);
void ov05_0221DB18(Overlay05State *);
u32 ov05_0221DB4C(int, int);
u32 ov05_0221DB70(int, int);
void ov05_0221DC34(Overlay05State *);
void ov05_0221E5E4(Overlay05State *);
void ov05_0221E60C(Overlay05State *);
void ov05_0221E9C4(Overlay05State *);
int ov05_0221E9F8(Overlay05State *);
void ov05_0221EA18(Overlay05State *);
void ov05_0221EA38(Overlay05State *);
int ov05_0221C018(Overlay05State *);
int ov05_0221C568(Overlay05State *);
void ov05_0221BB00(void);
int ov05_0221C8A0(Overlay05State *);
void ov05_0221D054(Overlay05State *);

int ov05_0221CCF4(void *arg) {
    Comm_GetLocalNetId(arg);
    switch (sub_020378AC()) {
    case 0:
    case 2:
        return 1;
    case 1:
    case 3:
        return 0;
    }
    return 0;
}
