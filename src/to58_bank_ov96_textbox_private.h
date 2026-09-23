#ifndef TO52_R6_W09_PRIVATE_H
#define TO52_R6_W09_PRIVATE_H

#include "global.h"
#include "msgdata.h"

typedef struct Ov96TextBoxR43 {
    void *field00;
    u32 field04;
    u32 field08;
    u32 field0C;
    String *string;
    u32 field14;
    u8 field18[0x10];
    u32 field28;
    s32 printerId;
    u32 field30;
    union {
        u32 raw;
        struct {
            u32 timer : 16;
            u32 color : 8;
            u32 field24 : 3;
            u32 field27 : 1;
            u32 active : 1;
            u32 rest : 3;
        } bits;
    } flags;
} Ov96TextBoxR43;

u32 ov96_021EEA08(void *, void *, void *, u32, u32, u32, u32, u32, u32);
BOOL TextPrinterCheckActive(u8 id);
void GF_AssertFail(void);
BOOL ov96_021EE830(Ov96TextBoxR43 *work);
void ov96_021EE8CC(Ov96TextBoxR43 *work, u32 value);
void ov96_021EE908(Ov96TextBoxR43 *work, u32 value);

#endif
