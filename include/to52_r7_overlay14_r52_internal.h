#ifndef TO52_SOL_R7_W05_PRIVATE_H
#define TO52_SOL_R7_W05_PRIVATE_H

#include "global.h"

void ManagedSprite_SetAffineTranslation(void *sprite, int x, int y);
void ManagedSprite_SetAffineScale(void *sprite, f32 x, f32 y);
void ManagedSprite_SetAffineOverwriteMode(void *sprite, int mode);
u32 ov14_021E6070(void *work, u32 index, u32 field, u32 value);
void ov14_021F2A9C(void *data, u8 value, u32 enabled);

#endif
