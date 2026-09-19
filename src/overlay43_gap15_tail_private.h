#ifndef W08_OV43_PRIVATE_H
#define W08_OV43_PRIVATE_H
#include "global.h"
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define S16(p,o) (*(s16 *)((u8 *)(p)+(o)))
void Sprite_Delete(void *);
void RemoveWindow(void *);
void String_Delete(void *);
void ListMenuItems_Delete(void *);
void ov42_02227274(void *);
void ov42_02227114(void *);
void ov43_0222C7EC(void *, void *);
void ov43_0222A9F4(void *, void *, int);
void CopyToBgTilemapRect(void *, u8, u8, u8, u8, u8, const void *, u8, u8, u8, u8);
void ScheduleWindowCopyToVram(void *);
void ScheduleBgTilemapBufferTransfer(void *, int);
void ov43_0222AD00(void *, int);
void ov43_0222C378(void *, void *, void *, int, int, int, int);
void ov43_0222AD74(void *, int, int);
void ov43_0222AD98(void *, int, int);
void ov43_0222AA70(void *);
void ClearWindowTilemapAndScheduleTransfer(void *);
void ov43_0222C9A4(void *, void *, void *);
void ov43_0222BAB8(void *, void *, void *);
void ov43_0222BB20(void *, void *, void *, int);
void ov43_0222BBB8(void *, void *, void *);
#endif
