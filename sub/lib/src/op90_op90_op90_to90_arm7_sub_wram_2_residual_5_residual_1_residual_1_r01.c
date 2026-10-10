#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
void SVC_Sleep(void);
void __VENEER_SVC_Sleep(void);
// @fn __VENEER_SVC_Sleep
void __VENEER_SVC_Sleep(void)
{
    SVC_Sleep();
}
