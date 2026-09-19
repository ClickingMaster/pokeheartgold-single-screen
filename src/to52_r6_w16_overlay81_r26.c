#include "to52_r6_w16_overlay81_r26_private.h"
void ov81_02243370(void *work, void *input) {
    ov81_02243220(input);
    if (ov81_02242710(work) == 1) {
        ov81_022432DC(work, 0);
    } else {
        ov81_022432AC(work, 0);
    }
}
GridInputHandler *ov81_02243398(void *work, void *input) {
    ov81_02243220(input);
    if (ov81_02242710(work) == 1) {
        return GridInputHandler_Create(ov81_022436CC, ov81_02243788, &ov81_02243638, work, TRUE, 0, (enum HeapID)0x64);
    }
    return GridInputHandler_Create(ov81_022436A8, ov81_022437C8, &ov81_02243638, work, TRUE, 0, (enum HeapID)0x64);
}
void ov81_022433FC(GridInputHandler *grid, int mode, void *work) {
    if (mode == 1) {
        GridInputHandler_SetAllEnabled(grid);
        GridInputHandler_ClearEnabledFlag(grid, 4);
        GridInputHandler_ClearEnabledFlag(grid, 5);
    } else {
        GridInputHandler_SetAllEnabled(grid);
        GridInputHandler_ClearEnabledFlag(grid, 6);
        GridInputHandler_ClearEnabledFlag(grid, 7);
    }
    if (ov81_02242704(work) == 0) {
        GridInputHandler_ClearEnabledFlag(grid, 3);
    }
}
