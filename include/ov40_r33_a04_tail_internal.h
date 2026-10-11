#include "global.h"

#define PTR(p, off)  (*(void **)((u8 *)(p) + (off)))
#define WORD(p, off) (*(u32 *)((u8 *)(p) + (off)))

void TouchHitboxController_Destroy(void *);
void GfGfx_EngineBTogglePlanes(int, BOOL);
void GfGfx_EngineATogglePlanes(int, BOOL);
void ov40_0223A83C(void *);
void VsRecorder_SetVBlankPaused(void *, int);
void VsRecorder_FreeVideoInfo(void *, void *);
void VsRecorder_FreeProfile(void *, void *);
int VsRecorder_StepFade(void *, int);
BOOL VsRecorder_StepBlend(void *, void *, int, int);
void VsRecorder_SetTextSpriteMsg(void *, void *, int);
void ManagedSprite_SetAnim(void *, int);
void ov40_0223B4BC(void *, int);
void PaletteData_BlendPalettes(void *, int, int, int, int);
void VsRecorder_SetSubState(void *, int);
void *NewString_ReadMsgData(void *, int);
void *VsRecorder_NewMessageFormat(int);
void *FrontierCard_NewNameString(u32, int);
void VsRecorder_SanitizeName(void *, void *);
void *String_New(int, int);
void BufferString(void *, int, void *, int, int, int);
void StringExpandPlaceholders(void *, void *, void *);
void String_Delete(void *);
void MessageFormat_Delete(void *);
void InitWindow(void *);
void AddTextWindowTopLeftCorner(void *, void *, int, int, int, int);
u8 AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, void *);
void TextOBJ_CopyFromBGWindow(void *, void *, void *, int);
void RemoveWindow(void *);
void ov40_0222D78C(void *, int);
void *ov40_0222D800(void *, int);
void VsRecorder_CreateTextSprite(void *, void *, int);
void ManagedSprite_SetPositionXY(void *, s16, s16);
void TextOBJ_SetPosition(void *, int, int);
void TextOBJ_SetSpritesDrawFlag(void *, BOOL);
void VsRecorder_FreeTextSprite(void *);
void Sprite_DeleteAndFreeResources(void *);
void ov40_0222D7DC(void *);
void BgClearTilemapBufferAndCommit(void *, int);
void ov40_0223A430(void *);
void ov40_0223CD14(void *);
void VsRecorder_CreateVideoInfo(void *, void *);
void VsRecorder_SetVideoPartyIconsDrawFlag(void *, int);
int ov40_02230410(void *);
void VsRecorder_LoadScreen(void *, int, int);
void VsRecorder_CreateProfile(void *, void *);
void VsRecorder_SetProfileDrawFlag(void *, int);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
typedef struct FourInts {
    int v[4];
} FourInts;
typedef struct Ov40Entry {
    u8 raw[0xe4];
} Ov40Entry;
typedef struct Ov40Outer {
    u8 pad[0x2608];
    Ov40Entry *ptrs[30];
    Ov40Entry entries[30];
    int count;
} Ov40Outer;
extern const FourInts ov40_022454C0;
extern const FourInts ov40_022454D0;
int ov40_0223AF3C(void *);
int ov40_0223B190(void *);
void ov40_0223B29C(void *, void *, int);
void ov40_0223B374(void *);
void ov40_0223B44C(void *);
