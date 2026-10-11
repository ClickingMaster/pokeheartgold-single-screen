#include "global.h"

void ov41_0224B878(void *work);
void DressUpCanvas_FreeBackdrop(void *);

void ov41_0224B878(void *work)
{
    DressUpCanvas_FreeBackdrop((u8 *)work + 0x190);
}
