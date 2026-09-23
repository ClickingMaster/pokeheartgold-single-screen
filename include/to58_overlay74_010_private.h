#ifndef TO58_W01_OV74_010_PRIVATE_H
#define TO58_W01_OV74_010_PRIVATE_H
#include "global.h"
#include "filesystem.h"
#include "screen_fade.h"
#include "save.h"
#include "save_arrays.h"
#include "palPark_migration.h"
#include "nnsys/g2d/load/g2d_NCG_load.h"
u32 ov74_02233F68(void);
void sub_0201A728(int);
void sub_0201A738(int);
void ov74_02231930(u8 *base);
int FontID_String_GetWidth(int font, void *string, int letterSpacing);
int ov74_022319F8(u8 *base, void *string, int flags);
void ov74_02231D48(u8 *dst, u8 *src, int unused, u32 value);
u32 AGB_GetBoxMonData(void *boxMon, int field, void *dest);
u32 ov74_02231D70(u8 *base, int box, int slot);
u32 ov74_02231D94(u8 *base, int box, int slot);
u32 ov74_02231DB8(u8 *base, int box, int slot);
void ov74_02231DDC(u8 *out, int y, int x, int height, int width);
void Heap_Free(void *ptr);
void *ov74_02231E00(int narcId, int member, NNSG2dCharacterData **out, int heapId);
int ov74_02231E38(int member, NNSG2dCharacterData **out, void *buffer, NARC *narc);
void ov74_02232DA4(void *window);
#endif
