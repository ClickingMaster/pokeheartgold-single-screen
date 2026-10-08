// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct { u8 id; u8 length; u8 pad[2]; u8 *body; } WMOtherElement;
typedef struct { u8 count; u8 pad[3]; WMOtherElement element[16]; } WMOtherElementResults;
typedef struct { u16 length; u8 pad[0x3a]; u16 gameInfoLength; u16 otherElementCount; u8 elements[1]; } WMBssDescView;
WMOtherElementResults WM_GetOtherElements(WMBssDescView *bss);
// @fn WM_GetOtherElements
WMOtherElementResults WM_GetOtherElements(WMBssDescView *bss)
{
    WMBssDescView *desc = bss;
    u8 * p;
    WMOtherElementResults elems;
    int i;
    u8 elemLen;
    u8 total;
    if (desc->gameInfoLength != 0)
    {
        elems.count = 0;
        return elems;
    }
    elems.count = (u8)desc->otherElementCount;
    if (elems.count == 0)
    {
        return elems;
    }
    if (elems.count > 16)
    {
        elems.count = 16;
    }
    p = desc->elements;
    elemLen = (u8)(desc->length * 2 - 0x40);
    total = 0;
    i = 0;
    if (i < elems.count)
    {
        do
        {
            u8 len;
            elems.element[i].id = p[0];
            elems.element[i].length = p[1];
            elems.element[i].body = p + 2;
            len = (u8)(elems.element[i].length + 2);
            total = (u8)(total + len);
            if (total > elemLen)
            {
                elems.count = 0;
                return elems;
            }
            p += len;
            i++;
        }
        while (i < elems.count);
    }
    return elems;
}
