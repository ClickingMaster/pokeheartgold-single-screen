#include "to58_bank_ov96_textbox_private.h"

BOOL ov96_021EE830(Ov96TextBoxR43 *work)
{
    if (work->printerId != -1) {
        if (!TextPrinterCheckActive((u8)work->printerId)) {
            work->flags.raw = (work->flags.raw & 0xffff0000) | 0x14;
            String_Delete(work->string);
            work->string = NULL;
            work->printerId = -1;
        }
        work->flags.bits.active = TRUE;
    } else {
        GF_ASSERT(work->printerId == -1);
        if (work->flags.bits.timer != 0) {
            if (work->field28 == 0) {
                work->flags.bits.active = FALSE;
            } else {
                work->flags.bits.timer--;
                work->flags.bits.active = work->flags.bits.timer != 0;
            }
        } else {
            work->flags.bits.active = FALSE;
        }
    }
    return work->flags.bits.active;
}

void ov96_021EE8CC(Ov96TextBoxR43 *work, u32 value)
{
    work->printerId = ov96_021EEA08(work->field00, work->field18, &work->string,
        work->flags.bits.color, work->field04, work->field08, value,
        (u8)((work->flags.raw << 5) >> 29), work->field14);
}

void ov96_021EE908(Ov96TextBoxR43 *work, u32 value)
{
    work->printerId = ov96_021EEA08(work->field00, work->field18, &work->string,
        work->flags.bits.color, work->field04, work->field0C, value,
        (u8)((work->flags.raw << 5) >> 29), work->field14);
}
