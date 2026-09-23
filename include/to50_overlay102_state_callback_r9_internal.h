#ifndef TO50_OV102_R9_PRIVATE_H
#define TO50_OV102_R9_PRIVATE_H
#include "global.h"
typedef void (*Ov102Callback)(u8 *, u32 *);
BOOL ov102_021E8EF8(void);
BOOL sub_02090D88(void *);
BOOL ov102_021E8F14(void *);
void ov102_021E8F5C(void *, u32, u32);
void ov102_021E940C(void *, u32);
void ov102_021E8D50(void *, Ov102Callback);
u32 ov102_021EA258(void *);
void MailMsg_SetTrailingFieldsEmpty(void *);
void sub_02090E04(void *, void *, void *);
void ov102_021E7A4C(void *, Ov102Callback);
void ov102_021E8D1C(u8 *, u32 *);
BOOL System_GetTouchNew(void);
BOOL ov102_021E9464(void *);
void ov102_021E8D58(void *);
void ov102_021E8D94(u8 *, u32 *);
void ov102_021E8E84(u8 *, u32 *);
#endif
