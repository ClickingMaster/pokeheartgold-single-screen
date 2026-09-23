#ifndef TO56_R2W11_PRIVATE_H
#define TO56_R2W11_PRIVATE_H
#include "global.h"

#include "constants/moves.h"

#include "bg_window.h"
typedef struct Ov08R2W11Work {
    void *root;
    u8 pad_0004[0x1FA4];
    void *msgData;
    u8 pad_1FAC[4];
    void *message;
    void *spriteManager;
    void *sprites[38];
    Window windows[2];
    void *windowArray;
    u8 windowCount;
    u8 pad_2075[6];
    u8 printer;
} Ov08R2W11Work;
extern void ov08_0221F4A4(void *);
extern void ov08_0221F5D0(void *);
extern void ov08_0221F900(void *);
extern void ov08_0221F7C0(void *);
extern void ov08_0221FB18(void *);
extern void ov08_0221FF70(void *);
extern void ov08_0221FC7C(void *);
extern void ov08_0221FDA4(void *);
extern void ov08_02220084(void *);
extern void ov08_0221DF80(void *, int, int, int, u8);
extern void ov08_0221F3D0(void *, void *, int);
extern void DrawFrameAndWindow2(void *, BOOL, int, int);
extern void ov08_022201E8(void *);
extern void TextFlags_SetCanABSpeedUpPrint(BOOL);
extern int BattleSystem_GetTextFrameDelay(void *);
extern int AddTextPrinterParameterized(void *, int, void *, int, int, int, int);
extern void ov08_0222061C(void *);
extern void ov08_02220668(void *);
extern void ov08_02220750(void *);
extern void ov08_02220800(void *);
extern void ov08_02220878(void *);
extern void ov08_02220928(void *);
extern void ov08_02220A28(void *);
extern void ov08_0222162C(void *);
extern void *ov08_022209B8(void *, int);
extern void ov08_02220AAC(void *);
extern void *BattleSystem_GetSpriteSystem(void *);
extern void Sprite_DeleteAndFreeResources(void *);
extern void ov08_02221698(void *);
extern void SpriteSystem_FreeResourcesAndManager(void *, void *);
extern void *NewString_ReadMsgData(void *, int);
extern void String_Delete(void *);
extern int AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, int, int);
extern u32 FontID_String_GetWidth(int, void *, int);
extern void *NewMsgDataFromNarc(int, int, int, int);
extern void DestroyMsgData(void *);
void ov08_0222057C(void *);
void ov08_0221DD40(void *);
void ov08_0221DD70(void *, int);
void ov08_0221F5B0(void *, int);
void ov08_02220064(void *, int, int);
void ov08_022201C0(void *);
void ov08_022201E8(void *);
void ov08_022205E0(void *);
void ov08_02220A28(void *);
void ov08_02220A50(void *);
void ov08_0221E2E8(void *, int, int, int);
void ov08_0221E340(void *, int);
void ov08_0221E3A4(void *, int, int);
void ov08_0221ECE8(void *, int);
void ov08_0221EE18(void *, int);
void ov08_0221EFA8(void *, int);
void ov08_0221F00C(void *, int, int);
void ov08_0221F1B0(void *, int);
extern void ov08_0221DFC8(void *, int, int, int, u8);
extern void ov08_0221E048(void *, int, int, int, int);
void ov08_0221F550(void *, int);
void ov08_0221EF48(void *, int, int);
extern void *BattleSystem_GetPartyMon(void *, int, int);
extern void *Mon_GetBoxMon(void *);
extern void BufferBoxMonNickname(void *, u32, void *);
extern void BufferMoveName(void *, u32, u32);
extern void StringExpandPlaceholders(void *, void *, void *);
#endif
