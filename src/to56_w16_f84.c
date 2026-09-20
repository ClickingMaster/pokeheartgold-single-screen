#include "to56_w16_private.h"

int ov13_02223F84(void) {
    int running = 1;

    if (ov13_02223C48()) {
        int zero = 0;
        int keepGoing = zero;
        do {
            OS_Sleep(10);
            int state = ov13_02223E6C();
            while (state != 0) {
                switch (state) {
                case 4:
                case 5:
                    break;
                case 20:
                    running = keepGoing;
                    break;
                default:
                    running = zero;
                    break;
                }
                state = ov13_02223E6C();
            }
        } while (running != 0);
    }

    if (ov13_0224DF30.allocation != NULL) {
        void (*freeFn)(void *) = *(void (**)(void *))((u8 *)&ov13_0224DF30 + 0x0c);
        freeFn(ov13_0224DF30.allocation);
        ov13_0224DF30.allocation = NULL;
    }
    return 1;
}
