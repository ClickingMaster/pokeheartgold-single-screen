#include "overlay71_opus_partial_internal.h"

extern u32 GetBoxMonData(void *boxmon, int field, void *dest);
extern u32 GetMonBaseStat_HandleAlternateForm(u32 species, u32 form, u32 stat);
extern void SpriteList_RenderAndAnimateSprites(void *list);
extern void OamManager_ApplyAndResetBuffers(void);
extern void *SysTask_GetData(void *task);
extern void SysTask_Destroy(void *task);
extern void GF_AssertFail(void);
extern void Heap_Free(void *ptr);
extern void *Heap_Alloc(u32 heapId, u32 size);
extern u32 Options_GetFrame(void *options);
extern void Camera_GetLookAtCamPos(Ov71Vec3 *dst, void *camera);
extern void Camera_SetAnglePos(void *arg0, void *camera);
extern void Camera_AdjustAngleTarget(void *arg0, void *camera);
extern void Camera_ApplyPerspectiveType(void *arg0, void *camera);
extern void Camera_SetPerspectiveAngle(void *arg0, void *camera);
extern void Camera_UnsetStaticPtr(void);
extern void Camera_Delete(void *camera);
extern void Sprite_SetAnimCtrlSeq(void *sprite, int seq);
extern void Sprite_SetDrawFlag(void *sprite, int flag);
extern void Sprite_Delete(void *sprite);
extern void G3X_Reset(void);
extern void RequestSwap3DBuffers(int a, int b);
extern void Main_SetVBlankIntrCB(void *cb, void *arg);

extern void TradeSequenceSend_Draw(Ov71Sub2 *w);
extern void TradeSequence_DrawModels(void *arg);

extern s32 sTradeSequenceDeferredFreeCount;
extern void *sTradeSequenceDeferredFrees[0x20];
extern u32 ov71_0224C0C4;
extern u32 sTradeSequenceDepartCameraAngle[2];
extern u32 ov71_0224C0D0[4];
extern u32 _0224C020[8];

BOOL TradeSequence_BoxMonCantFlip(void *boxmon);
void TradeSequence_DrawSpritesTask(void *unused, Ov71Sprs *sprs);
void TradeSequence_StopPulses(Ov71Sys *sys);
void TradeSequence_SetPulseColor(Ov71Sys *sys, int clear, int set, u16 value);
void TradeSequence_DestroyPulse(Ov71Sys *sys, int index);

void TradeSequence_FreeCellAnim(Ov71Pair *pair);
void TradeSequence_InitSpriteHeader(Ov71Ctx *ctx, Ov71Src *src, Ov71Opt *opt, void *a3, u8 a4);
void *TradeSequence_GetBgConfig(Ov71Ctx *ctx);
void *TradeSequence_GetMsgFormat(Ov71Ctx *ctx);
void *TradeSequence_GetMsgData(Ov71Ctx *ctx);
u32 TradeSequence_GetSentBoxMon(Ov71Ctx *ctx);
u32 TradeSequence_GetReceivedBoxMon(Ov71Ctx *ctx);
u32 TradeSequence_GetTimeOfDay(Ov71Ctx *ctx);
u16 TradeSequence_GetSentSpecies(Ov71Big *w);
u16 TradeSequence_GetSentForm(Ov71Big *w);
u16 TradeSequence_GetReceivedSpecies(Ov71Big *w);
u16 TradeSequence_GetReceivedForm(Ov71Big *w);
u32 TradeSequence_GetFrameType(Ov71Ctx *ctx);
u32 TradeSequence_GetTradeMode(Ov71Ctx *ctx);
void TradeSequence_ResetDeferredFrees(void);
void TradeSequence_FlushDeferredFrees(void);
void TradeSequence_DeferFree(void *ptr);
void TradeSequence_FreeModel(Ov71Cam *cam);
void TradeSequence_GetModelPosition(Ov71Cam *cam, Ov71Vec3 *dst);
void TradeSequence_SetModelPosition(Ov71Cam *cam, const Ov71Vec3 *src);
void TradeSequence_GetModelRotation(Ov71Cam *cam, Ov71S16Vec3 *dst);
void TradeSequence_SetModelRotation(Ov71Cam *cam, const Ov71S16Vec3 *src);
void TradeSequence_SetModelVisible(Ov71Cam *cam, void *value);
void TradeSequence_SetModelScale(Ov71Cam2 *cam, u32 value);
void TradeSequence_GetCameraPos(Ov71Cam *cam, Ov71Vec3 *dst);
void TradeSequence_SetCameraAngle(Ov71Cam *cam, void *arg0);
void TradeSequence_AdjustCameraAngle(Ov71Cam *cam, void *arg0);
void TradeSequence_SetCameraPerspectiveType(Ov71Cam *cam, void *arg0);
void TradeSequence_SetCameraPerspectiveAngle(Ov71Cam *cam, void *arg0);
void TradeSequence_FreeCamera(Ov71Cam *cam);
void TradeSequence_DestroyModels(Ov71CamSet *set);
BOOL TradeSequenceSend_Main(Ov71Sub2 *w, u32 *state);
BOOL TradeSequenceDepart_Main(Ov71D4 *w, u32 *state);
extern void *sTradeSequenceSendStates[4];
extern void *sTradeSequenceDepartStates[4];
void TradeSequenceSend_DeleteSprites(Ov71Sprs *sprs);
BOOL TradeSequenceSend_IsBrightnessDone(Ov71Cam2 *cam);
void TradeSequenceSend_StopBrightness(Ov71Cam2 *cam);
BOOL TradeSequenceSend_IsBounceDone(Ov71Cam2 *cam);
u32 TradeSequenceSend_GetBounceCount(Ov71Cam2 *cam);
void TradeSequenceSend_StopBounce(Ov71Cam2 *cam);
void TradeSequenceDepart_RequestScreenSwap(Ov71Flag *w);
void TradeSequenceDepart_StopStreakFall(void *task);
void TradeSequenceDepart_StopScroll(void *task);
void *TradeSequenceDepart_IsFlightOutOfView(void *task);
void *TradeSequenceDepart_IsFlightFlyingAway(void *task);

extern BOOL IsPaletteFadeFinished(void);
extern void FreeBgTilemapBuffer(void *bgConfig, u32 layer);
extern void BeginNormalPaletteFade(u32 a, u32 b, u32 c, u16 d, u16 e, u8 f, u32 g);
extern void ov71_0224A080(Ov71Sub2 *w);
extern BOOL ov71_02249E6C(Ov71Sub2 *w);
extern BOOL ov71_0224AB7C(Ov71Sub2 *w);
extern BOOL ov71_0224ADE4(Ov71Sub2 *w);
extern void *ov71_0224BDE8[2];
extern void *ov71_0224BE7C[2];

void TradeSequence_SetModelAlpha(Ov71Cam *cam, u32 value);
BOOL TradeSequenceSend_WaitFadeIn(Ov71Spr *w, const u32 *state);
void TradeSequenceDepart_Draw(Ov71D4 *w);
void TradeSequenceDepart_Free3D(Ov71D4 *w);
void TradeSequenceDepart_DeleteStreaks(Ov71Grid2 *grid);
Ov71Scene *ov71_022494A0(Ov71Ctx *ctx);
void ov71_022494D0(Ov71Scene *sc);
void ov71_02249538(Ov71Scene *sc);
void ov71_022497C8(Ov71Scene *sc);
void ov71_022498E0(Ov71Scene *sc);
void ov71_02249970(Ov71Scene *sc);
void ov71_02249988(Ov71Sub2 *w);
void ov71_02249990(Ov71Sub2 *w);
BOOL ov71_02249A50(Ov71Sub2 *w);
void ov71_02249C90(Ov71Sub2 *w);
Ov71Sub2 *ov71_02249CF0(Ov71Ctx *ctx);
void ov71_02249D14(Ov71Sub2 *w);
BOOL ov71_02249D28(Ov71Sub2 *w, u32 *state);
BOOL ov71_02249D58(Ov71Sub2 *w);
void ov71_0224A12C(Ov71Scene *sc);
BOOL ov71_0224A15C(Ov71Sub2 *w, u32 *state);
void ov71_0224A3D8(Ov71Scene *sc);
void ov71_0224A500(Ov71Scene *sc);
void ov71_0224A590(Ov71Scene *sc);
void ov71_0224A5A8(Ov71Sub2 *w);
void ov71_0224A5B0(Ov71Sub2 *w);
BOOL ov71_0224A684(Ov71Sub2 *w);
void ov71_0224A8A8(Ov71Sub2 *w);
BOOL ov71_0224A9F8(Ov71Sub2 *w);
void ov71_0224AEF4(Ov71Scene *sc);
void ov71_0224AFB8(void *task);
void ov71_0224B068(void *task);
void ov71_0224B11C(void *task);

/* ---- round-2 batch 1 ---- */
extern BOOL IsFanfarePlaying(void);
extern BOOL IsPaletteFadeFinished(void);
extern int _s32_div_f(int lhs, int rhs);
extern void TradeSequence_DestroyPulses(void *arg);

BOOL ov71_0224B960(Ov71Task2 *w);
void ov71_0224B970(Ov71Task2 *w);
void ov71_0224B990(void *task, Ov71Bright *w);
void ov71_0224BA48(void *task);
void ov71_0224BAA0(Ov71Scroll *w, s32 target, s32 frames);
void ov71_0224B8EC(Ov71SprPair *w);
BOOL ov71_0224B450(Ov71SprPair *w, void **pending);
BOOL ov71_0224B6F0(Ov71Fanfare *w, s32 limit);
Ov71Cell *ov71_0224A920(Ov71Ctx *ctx);
void ov71_0224ADB0(Ov71Bgs *w);
void ov71_0224B0E8(void *task, Ov71Slide *w);

/* ---- round-2 batch 2 ---- */
extern void BgSetPosTextAndCommit(void *bgConfig, u32 layer, u32 op, u32 value);
extern void *ov71_0224BEF0[3];
extern void *ov71_0224BD7C[2];
extern void ov71_0224A9C4(Ov71Cell *w);
extern void ov71_02249538(Ov71Scene *w);

void ov71_0224A954(Ov71Cell *w);
BOOL ov71_0224A98C(Ov71Cell *w, u32 *state);
BOOL ov71_02249500(Ov71Scene *w, u32 *state);
void ov71_0224A0B8(void *bgConfig, u32 y, u32 x);

void ov71_0224B068(void *task) {
    Ov71TaskB *w;
    if (task != NULL) {
        w = (Ov71TaskB *)SysTask_GetData(task);
        *w->unk_00 = 0;
        Heap_Free(w);
        SysTask_Destroy(task);
    }
}
