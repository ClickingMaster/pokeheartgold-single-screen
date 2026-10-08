#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
#pragma dollar_identifiers on
extern u32 isFirstCheck$3769[];
extern u32 lock_id[];
extern BOOL CTRDG_IsPulledOut(void);
extern BOOL CTRDG_IsExisting(void);
extern void OS_Sleep(u32);
extern int PXI_SendWordByFifo(int, u32, BOOL);
void CTRDG_CheckPullOut_Polling(void);
// @fn CTRDG_CheckPullOut_Polling
void CTRDG_CheckPullOut_Polling(void)
{
    u32 next = isFirstCheck$3769[1];
    if (next == 0xffffffff) {
        isFirstCheck$3769[1] = *(vu32 *)0x027ffc3c + 10;
        return;
    }
    if (lock_id[4] || lock_id[3])
        return;
    if (*(vu32 *)0x027ffc3c < next)
        return;
    isFirstCheck$3769[1] = *(vu32 *)0x027ffc3c + 10;
    lock_id[3] = CTRDG_IsPulledOut();
    if (!CTRDG_IsExisting()) {
        if (isFirstCheck$3769[0]) {
            lock_id[4] = TRUE;
            return;
        }
        lock_id[3] = TRUE;
    }
    isFirstCheck$3769[0] = 0;
    if (lock_id[3]) {
        while (PXI_SendWordByFifo(13, 17, FALSE))
            OS_Sleep(100);
    }
}
