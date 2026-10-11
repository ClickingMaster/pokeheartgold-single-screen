#include "summary_text_work_internal.h"

void sub_02088894(SummaryTextWork *work) {
    work->msgData300 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, (NarcId)27, 302, HEAP_ID_19);
    work->msgData424 = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, (NarcId)27, 424, HEAP_ID_19);
    work->printer = MessagePrinter_New(1, 2, 0, HEAP_ID_19);
    work->msgFormat = MessageFormat_New(HEAP_ID_19);
    work->string230 = String_New(12, HEAP_ID_19);
    work->string234 = String_New(12, HEAP_ID_19);
    work->string238 = String_New(8, HEAP_ID_19);
    work->string7AC = String_New(128, HEAP_ID_19);
    work->msgData750 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, (NarcId)27, 750, HEAP_ID_19);
    work->string7B0 = String_New(8, HEAP_ID_19);
    if (work->input->presetName != NULL) {
        CopyU16ArrayToString(work->string7B0, work->input->presetName);
    }
}

void sub_0208895C(SummaryTextWork *work) {
    DestroyMsgData(work->msgData750);
    DestroyMsgData(work->msgData424);
    DestroyMsgData(work->msgData300);
    MessagePrinter_Delete(work->printer);
    MessageFormat_Delete(work->msgFormat);
    String_Delete(work->string230);
    String_Delete(work->string234);
    String_Delete(work->string238);
    String_Delete(work->string7AC);
    String_Delete(work->string7B0);
}

void sub_020889D0(SummaryTextWork *work, NARC *narc) {
    work->cellManager = BgCellManager_New(work->bgConfig, 2, 7, HEAP_ID_19);
    BgCellManager_InitCell(work->cellManager, 0, 6, 17, 6);
    BgCellManager_LoadCellTilemapFromOpenNarc(work->cellManager, 0, narc, 69, 0);
    BgCellManager_InitCell(work->cellManager, 1, 6, 17, 6);
    BgCellManager_LoadCellTilemapFromOpenNarc(work->cellManager, 1, narc, 70, 0);
    BgCellManager_InitCell(work->cellManager, 2, 6, 10, 4);
    BgCellManager_LoadCellTilemapFromOpenNarc(work->cellManager, 2, narc, 71, 0);
    BgCellManager_InitCell(work->cellManager, 3, 6, 10, 4);
    BgCellManager_LoadCellTilemapFromOpenNarc(work->cellManager, 3, narc, 72, 0);
    BgCellManager_InitCell(work->cellManager, 4, 6, 11, 14);
    BgCellManager_LoadCellTilemapFromOpenNarc(work->cellManager, 4, narc, 73, 0);
    BgCellManager_InitCell(work->cellManager, 5, 6, 6, 3);
    BgCellManager_LoadCellTilemapFromOpenNarc(work->cellManager, 5, narc, 74, 0);
    BgCellManager_InitCell(work->cellManager, 6, 6, 6, 3);
    BgCellManager_LoadCellTilemapFromOpenNarc(work->cellManager, 6, narc, 75, 0);
}
