#ifndef TO57_NEW53_PRIVATE_H
#define TO57_NEW53_PRIVATE_H
#include "global.h"

#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_02013534.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
extern const ObjCharTransferTemplate ov91_02261C64;
void GF_AssertFail(void);
void ov91_0225FA9C(u8 *, enum HeapID);
void ov91_0225FB48(u8 *);
void ov91_0225FB80(u8 *, NARC *, enum HeapID);
void ov91_0225FC84(u8 *);
#endif
