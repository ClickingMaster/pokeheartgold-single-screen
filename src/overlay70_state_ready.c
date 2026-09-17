#include "overlay70_state_ready_private.h"

BOOL ov70_02237F38(void) {
    switch (_02246800.state) {
    case 1:
        return TRUE;
    case 24:
        _02246800.state = 1;
        return TRUE;
    default:
        return FALSE;
    }
}
