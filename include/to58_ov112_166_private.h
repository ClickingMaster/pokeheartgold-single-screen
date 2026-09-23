#ifndef OV112_166_PRIVATE_H
#define OV112_166_PRIVATE_H
#include "global.h"

void GF_AssertFail(void);
void *NewMsgDataFromNarc(int, int, int, int);
void *MessageFormat_New_Custom(int, int, int);
void AddWindow(void *, void *, const void *);
void BG_FillCharDataRange(void *, int, int, int, int);
void LoadFontPal1(int, int, int);
void FontID_Alloc(int, int);
extern const u8 ov112_021FF350[];
void ov112_021F1288(u8 *state);

#endif
