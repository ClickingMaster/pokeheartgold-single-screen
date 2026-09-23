#include "global.h"

extern void *Save_Rankings_Get(void *);
extern void Save_UpdateRankingsFromMixing(void *, void *, u8, void *, void *);
void ov73_021EA324(void *);

void ov73_021EA324(void *work) {
    Save_Rankings_Get(*(void **)((u8 *)work + 4));
    Save_UpdateRankingsFromMixing(
        *(void **)((u8 *)work + 4),
        *(void **)((u8 *)work + 12),
        (u8)*(u32 *)((u8 *)work + 8),
        *(void **)((u8 *)work + 16),
        *(void **)work);
}
