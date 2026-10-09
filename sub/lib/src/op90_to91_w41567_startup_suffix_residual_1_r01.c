// ARM7 _start_ModuleParams (0x02380198, 20 B): the autoload/static-bss parameter block that crt0
// reads, stored in .text right after the startup code. Written as an MWCC asm body of dcd words
// (same bytes and ABS32 relocations as the assembly; the symbol becomes FUNC instead of NOTYPE).
#include <nitro/types.h>
extern u32 SDK_AUTOLOAD_LIST[];
extern u32 SDK_AUTOLOAD_LIST_END[];
extern u32 SDK_AUTOLOAD_START[];
extern u32 SDK_STATIC_BSS_START[];
extern u32 SDK_STATIC_BSS_END[];
void _start_ModuleParams(void);
#include <nitro/code32.h>
asm void _start_ModuleParams(void)
{
    dcd SDK_AUTOLOAD_LIST
    dcd SDK_AUTOLOAD_LIST_END
    dcd SDK_AUTOLOAD_START
    dcd SDK_STATIC_BSS_START
    dcd SDK_STATIC_BSS_END
}
#include <nitro/codereset.h>
