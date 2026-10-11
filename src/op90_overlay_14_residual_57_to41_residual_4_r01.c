#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "msgdata.h"
#include "string.h"
#include "heap.h"
typedef struct { u8 pad[0x20]; MsgData *msgData; u8 pad2[0xc]; Window windows[1]; } Inner;
typedef struct { u8 pad[0x34]; Inner *inner; } Work;
void PCBox_PrintAligned(Window *window, String *str, s32 x, s32 y, u32 font, u32 color, u32 align);
void ov14_021F5C94(Work *p, u32 win, u32 msgId);
// @fn ov14_021F5C94
void ov14_021F5C94(Work *p, u32 win, u32 msgId) {
Window *w = &p->inner->windows[win]; u8 width; NNSG2dCharacterData *chr; void *buf; u8 *pix; u8 i; String *str; width = GetWindowWidth(w); buf = GfGfxLoader_GetCharData((NarcId)19, 14, TRUE, &chr, HEAP_ID_10); pix = chr->pRawData;
BlitBitmapRect(w, pix + 0xAE0, 0, 0, 24, 8, 0, 0, 24, 8, 0xFF);
BlitBitmapRect(w, pix + 0xB60, 0, 0, 24, 8, 0, 8, 24, 8, 0xFF);
BlitBitmapRect(w, pix + 0xB20, 0, 0, 24, 8, 0, 16, 24, 8, 0xFF);
BlitBitmapRect(w, pix + 0xD60, 0, 0, 8, 8, (width - 1) * 8, 0, 8, 8, 0xFF);
BlitBitmapRect(w, pix + 0xD20, 0, 0, 8, 8, (width - 1) * 8, 8, 8, 8, 0xFF);
BlitBitmapRect(w, pix + 0xD40, 0, 0, 8, 8, (width - 1) * 8, 16, 8, 8, 0xFF);
for (i = 1; i < width - 1; i++) {
BlitBitmapRect(w, pix + 0xB00, 0, 0, 8, 8, i * 8, 0, 8, 8, 0xFF);
BlitBitmapRect(w, pix + 0xB80, 0, 0, 8, 8, i * 8, 8, 8, 8, 0xFF);
BlitBitmapRect(w, pix + 0xB40, 0, 0, 8, 8, i * 8, 16, 8, 8, 0xFF);
}
Heap_Free(buf);
str = NewString_ReadMsgData(p->inner->msgData, msgId);
PCBox_PrintAligned(w, str, width * 8 / 2, 4, 4, 0x90A00, 2);
String_Delete(str);
CopyWindowPixelsToVram_TextMode(w);
}
