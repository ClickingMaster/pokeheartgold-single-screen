#ifndef TO57_NEW47_PRIVATE_H
#define TO57_NEW47_PRIVATE_H

#include "global.h"

#include "system.h"

typedef struct String String;
typedef struct W12Work {
    u8 raw[0x1F378];
} W12Work;

#define U32(w, n) (*(u32 *)((w)->raw + (n)))
#define S32(w, n) (*(s32 *)((w)->raw + (n)))
#define PTR(w, n) (*(void **)((w)->raw + (n)))

extern void ov112_021E9C10(W12Work *, int);
extern void ov112_021E9A78(void *, int);
extern void FillBgTilemapRect(void *, int, int, int, int, int, int, int);
extern void BgCommitTilemapBufferToVram(void *, int);
extern void PokewalkerApp_LoadTilemap(W12Work *, int, int);
extern String *NewString_ReadMsgData(void *, int);
extern void ov112_021E9FA4(W12Work *, int, String *, int, u32);
extern void String_Delete(String *);
extern void ov112_021E9F40(W12Work *, int);
extern void ov112_021EA51C(W12Work *, int);
extern void sub_020326A4(void *, u16, u16);
extern const u32 ov112_021FF0AC[];
int ov112_021EAF5C(W12Work *);
int ov112_021EAFFC(W12Work *);

#endif
