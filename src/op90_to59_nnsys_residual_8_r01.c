#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef struct FrmPlttState { u32 lo; u32 hi; } FrmPlttState;
extern FrmPlttState _021D84C4;
u32 NNS_GfdAllocFrmPlttVram(u32 szByte, BOOL b4Pltt, u32 bAllocFromLo);
static inline BOOL AllocFromLo_(u32 *pAddr, u32 szByte, BOOL b4Pltt) {
    u32 lo = _021D84C4.lo;
    u32 pad = b4Pltt ? ((8 - (lo & 7)) & 7) : ((16 - (lo & 15)) & 15);
    u32 need = szByte + pad;
    if (_021D84C4.hi - lo >= need) {
        u32 end = lo + need;
        if (b4Pltt && end > 0x10000) return FALSE;
        *pAddr = lo + pad;
        _021D84C4.lo += need;
        return TRUE;
    }
    return FALSE;
}
static inline BOOL AllocFromHi_(u32 *pAddr, u32 szByte, BOOL b4Pltt) {
    u32 hi = _021D84C4.hi;
    u32 pad, need;
    if (hi >= szByte) {
        pad = b4Pltt ? ((hi - szByte) & 7) : ((hi - szByte) & 15);
        need = szByte + pad;
        if (hi - _021D84C4.lo >= need) {
            if (b4Pltt && hi > 0x10000) return FALSE;
            _021D84C4.hi -= need;
            *pAddr = _021D84C4.hi;
            return TRUE;
        }
    }
    return FALSE;
}
// @fn NNS_GfdAllocFrmPlttVram
u32 NNS_GfdAllocFrmPlttVram(u32 szByte, BOOL b4Pltt, u32 bAllocFromLo) {
    u32 addr = 0;
    BOOL result;
    szByte = (szByte == 0) ? 8 : ((szByte + 7) & ~7);
    if (szByte >= 0x7FFF8) return 0;
    if (bAllocFromLo == 1) {
        result = AllocFromLo_(&addr, szByte, b4Pltt);
    } else {
        result = AllocFromHi_(&addr, szByte, b4Pltt);
    }
    if (!result) return 0;
    return ((szByte >> 3) << 16) | ((addr >> 3) & 0xffff);
}
