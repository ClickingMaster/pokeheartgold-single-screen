#include "to56_w16_private.h"

void ov13_02224100(void) {
    int running = 1;

    if (ov13_0224DF30.started != 0) {
        if (ov13_02223C10()) {
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
                    case 14:
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
        ov13_0224DF30.started = 0;
    }
    if (ov13_0224DF30.socStarted != 0) {
        ov13_0224DF30.socStarted = 0;
        SOC_Cleanup();
    }
}
