#pragma force_active on
#include <nitro/types.h>
#include <nitro/os/thread.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u8 wmspRequestThread[];
typedef char W40803ThreadSize[(sizeof(OSThread)==0xa4)?1:-1];
OSThread *WMSP_GetRequestThread(void);
// @fn WMSP_GetRequestThread
OSThread *WMSP_GetRequestThread(void){return (OSThread*)wmspRequestThread;}
