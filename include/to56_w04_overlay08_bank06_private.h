#ifndef TO56_W04_OVERLAY08_BANK06_PRIVATE_H
#define TO56_W04_OVERLAY08_BANK06_PRIVATE_H

#include "global.h"

#include "bg_window.h"
#include "palette.h"

typedef struct Ov08W04State {
    u8 pad0000[0x1E4];
    void *bgConfig;
    void *paletteData;
    u8 pad01EC[0x1F60 - 0x1EC];
    u8 paletteCopy[0x40];
    u8 pad1FA0[4];
    void *printer;
    void *msgData;
    void *msgFormat;
    void *string;
    u8 pad1FB4[0x207C - 0x1FB4];
    u8 state;
    u8 pad207D[0x2088 - 0x207D];
    void *smallState;
} Ov08W04State;

typedef struct Ov08W04Root {
    u8 pad[0xC];
    u32 heapId;
} Ov08W04Root;

extern const struct GraphicsModes ov08_02224E34;
extern const struct BgTemplate ov08_02224EAC;
extern const struct BgTemplate ov08_02224EE4;
extern const struct BgTemplate ov08_02224F00;
extern const struct BgTemplate ov08_02224EC8;
extern const u8 ov08_02224F1C[];
extern const u8 ov08_02224E54[];
extern const u8 ov08_02224E68[];
extern const u8 ov08_02224F5C[];
extern const u8 ov08_02224E7C[];

void DestroyMsgData(void *);
void MessagePrinter_Delete(void *);
void MessageFormat_Delete(void *);
void String_Delete(void *);
void *NewMsgDataFromNarc(int, int, int, u32);
void *MessagePrinter_New(int, int, int, u32);
void *MessageFormat_New(u32);
void *String_New(int, u32);
void FontID_Release(int);
void DestroySysTaskAndEnvironment(void *);
void ov08_02220A50(void *);
void ov08_0221DD40(void *);
void ov08_0221D14C(Ov08W04State *);
u8 BattleDpadCursor_IsActive(void *);
void BattleDpadCursor_Free(void *);
int ov08_0221D5D0(void *, const void *);
int BattleDpadCursor_HandleInput(void *);
BOOL ov08_0221D5DC(void *, int);
void ov08_022217C8(void *);

int ov08_0221CD64(Ov08W04State *work);
int ov08_0221CD90(void *task, Ov08W04State *work);
void ov08_0221CDF8(Ov08W04State *work);
void ov08_0221CF08(void *bgConfig);
void ov08_0221D0F4(Ov08W04State *work);
u8 ov08_0221D4B0(void *work);
u8 ov08_0221D4F8(void *work);
u8 ov08_0221D540(void *work);
u8 ov08_0221D588(void *work);
BOOL ov08_0221D438(void *work);

#endif
