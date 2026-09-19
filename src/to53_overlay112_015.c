#include "to53_overlay112_w01_private.h"


void ov112_021E7464(void) {
    OS_LockMutex((OSMutex *)ov112_021FFB08);
    OS_KillThread((OSThread *)ov112_021FFBD8, NULL);
    OS_UnlockMutex((OSMutex *)ov112_021FFB08);
}
