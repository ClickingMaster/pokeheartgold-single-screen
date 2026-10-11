#include "summary_screen_helpers_internal.h"

int sub_02088AF8(void *data) {
    return BgCellManager_Delete(*(void **)((u8 *)data + 0x7D0));
}
