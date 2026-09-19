#ifndef TO52_SOL_R7_W06_PRIVATE_H
#define TO52_SOL_R7_W06_PRIVATE_H
#include "global.h"
typedef struct Ov43R24Window { u8 raw[0x10]; } Ov43R24Window;
typedef struct Ov43R24Windows { Ov43R24Window windows[3]; u8 pad30[4]; void *screen; } Ov43R24Windows;
typedef struct Ov43R24Context { u32 unk00; void *save; } Ov43R24Context;
typedef struct Ov43R24Work { void *bgConfig; u8 pad04[0x4c]; void *msgFmt; u8 buffer[0x64 - 0x54]; u16 tilebuf[0x32]; } Ov43R24Work;
typedef struct Ov43R24Screen { u16 width; u16 height; u8 pad04[8]; u8 data[]; } Ov43R24Screen;
void *Save_PlayerData_GetProfile(void *); void BufferPlayersName(void *,u32,void *); void ov43_0222A9F4(void *,void *,u32); void ov43_0222AA70(void *);
void LoadRectToBgTilemapRect(void *,u32,void *,u32,u32,u32,u32); void ScheduleBgTilemapBufferTransfer(void *,u32);
void ScheduleWindowCopyToVram(void *); void ClearWindowTilemapAndScheduleTransfer(void *);
void FillBgTilemapRect(void *,u32,u32,u32,u32,u32,u32,u32); void BgTilemapRectChangePalette(void *,u32,u32,u32,u32,u32,u32);
typedef struct Ov43R24PalettePair { u8 unused; u8 first; u8 second; } Ov43R24PalettePair;
extern Ov43R24PalettePair _0222ED5C;
void ov43_0222D028(Ov43R24Windows *,Ov43R24Context *,Ov43R24Work *); void ov43_0222D090(Ov43R24Windows *,Ov43R24Work *); void ov43_0222D0F8(Ov43R24Windows *,Ov43R24Work *,u32);
#endif
