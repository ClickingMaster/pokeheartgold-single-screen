#ifndef SLOP_R1_OVERLAY98_MODULE11_INTERNAL_H
#define SLOP_R1_OVERLAY98_MODULE11_INTERNAL_H

#define AddTextPrinterParameterizedWithColor AddTextPrinterParameterizedWithColor_RepositoryDecl
#define ov98_0221EC3C                        ov98_0221EC3C_RepositoryDecl
#include "overlay_98_remaining_internal.h"
#undef AddTextPrinterParameterizedWithColor
#undef ov98_0221EC3C

#include "msgdata/msg.naix"

#include "heap.h"

u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, PrinterCallback_t callback);

void ov98_0221EA4C(ManagedSprite *sprite, NarcId narcId, s32 memberNo, BOOL isMain, BOOL grayscale, enum HeapID heapId);
Ov98TextContext *ov98_0221EABC(enum HeapID heapId, BgConfig *bgConfig, int windowCount, const WindowTemplate *templates, s32 msgBank);
void ov98_0221EC3C(Ov98TextContext *textSys, u32 windowIdx, u32 msgId, BOOL center, u8 x, u8 y, u32 fontId, u8 fill);

#endif
