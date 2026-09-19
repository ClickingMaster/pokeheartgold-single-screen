#ifndef W17_R5_PRIVATE_H
#define W17_R5_PRIVATE_H
#include "global.h"
#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "render_window.h"
#include "string_util.h"
#include "text.h"
#include "yes_no_prompt.h"
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
u32 ov41_02247B7C(void *);
void ov41_02247BB8(void *, int, u8, u8, u8, u8);
void ov41_02247C7C(void *, int);
#endif
