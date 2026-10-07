#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
extern void *_021D6F6C[3], *_021D6F84[3];
extern u32 _021D6F78[3];
void vct_init_audio_queue(u32 ch);
// @fn vct_init_audio_queue
void vct_init_audio_queue(u32 ch) {
    OSIntrMode enabled = OS_DisableInterrupts();
    _021D6F6C[ch] = _021D6F84[ch] = NULL;
    _021D6F78[ch] = 0;
    OS_RestoreInterrupts(enabled);
}
