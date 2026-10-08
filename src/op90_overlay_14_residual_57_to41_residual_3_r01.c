#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "msgdata.h"
#include "string.h"
#include "heap.h"
typedef struct { u8 pad[0x20]; MsgData *msgData; u8 pad2[0xc]; Window windows[1]; } Inner;
typedef struct { u8 pad[0x34]; Inner *inner; } Work;
void ov14_021F4F24(Window *window, String *str, s32 x, s32 y, u32 font, u32 color, u32 align);
void ov14_021F5950(Work *p, u32 win, u32 msgId, u32 style);
void ov14_021F5BD8(Work *p, u32 win, u32 msgId, u32 style, u8 align);
// @fn ov14_021F5950
void ov14_021F5950(Work *p, u32 win, u32 msgId, u32 style) {
Window *w = &p->inner->windows[win]; void *buf; u32 color; u16 offset; u8 bg, x, y, width, height; NNSG2dCharacterData *chr; u8 *pix; u8 i; String *str; bg = GetWindowBgId(w); x = GetWindowX(w); y = GetWindowY(w); width = GetWindowWidth(w); height = GetWindowHeight(w);
if (style == 1) { FillWindowPixelBuffer(w, 11); color = 0xE0F00; offset = 0x180; } else { FillWindowPixelBuffer(w, 4); color = 0xE0F00; offset = 0; }
buf = GfGfxLoader_GetCharData((NarcId)19, 64, TRUE, &chr, HEAP_ID_10); pix = chr->pRawData;
BlitBitmapRect(w, pix + offset, 0, 0, 8, 8, 0, 0, 8, 8, 0xFF);
BlitBitmapRect(w, pix + (offset + 0x40), 0, 0, 8, 8, (width - 1) * 8, 0, 8, 8, 0xFF);
BlitBitmapRect(w, pix + (offset + 0x120), 0, 0, 8, 8, 0, (height - 1) * 8, 8, 8, 0xFF);
BlitBitmapRect(w, pix + (offset + 0x160), 0, 0, 8, 8, (width - 1) * 8, (height - 1) * 8, 8, 8, 0xFF);
for (i = 1; i < height - 1; i++) {
BlitBitmapRect(w, pix + (offset + 0x60), 0, 0, 8, 8, 0, i * 8, 8, 8, 0xFF);
BlitBitmapRect(w, pix + (offset + 0xA0), 0, 0, 8, 8, (width - 1) * 8, i * 8, 8, 8, 0xFF);
}
for (i = 1; i < width - 1; i++) {
BlitBitmapRect(w, pix + (offset + 0x20), 0, 0, 8, 8, i * 8, 0, 8, 8, 0xFF);
BlitBitmapRect(w, pix + (offset + 0x140), 0, 0, 8, 8, i * 8, (height - 1) * 8, 8, 8, 0xFF);
}
Heap_Free(buf);
str = NewString_ReadMsgData(p->inner->msgData, msgId);
ov14_021F4F24(w, str, width * 8 / 2, 4, 4, color, 2);
String_Delete(str);
CopyWindowPixelsToVram_TextMode(w);
}

// @fn ov14_021F5BD8
void ov14_021F5BD8(Work *p, u32 win, u32 msgId, u32 style, u8 align) {
Window *w = &p->inner->windows[win]; u8 bg, x, y, width, height; String *str; u32 color; bg = GetWindowBgId(w); x = GetWindowX(w); y = GetWindowY(w); width = GetWindowWidth(w); height = GetWindowHeight(w); if (style == 1) { FillWindowPixelBuffer(w, 11); color = 0xE0F00; } else { FillWindowPixelBuffer(w, 4); color = 0xE0F00; } str = NewString_ReadMsgData(p->inner->msgData, msgId); if (align == 2 || align == 3) { ov14_021F4F24(w, str, width * 8 / 2, 0, 4, color, align); } else { ov14_021F4F24(w, str, 0, 0, 4, color, align); } String_Delete(str); CopyWindowPixelsToVram_TextMode(w);
}
