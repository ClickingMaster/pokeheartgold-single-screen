#include "global.h"

extern int ov01_021F5AE8(int direction);
extern void GF_AssertFail(void);
void ov01_021F5B24(int direction, int target, u8 *value);

void ov01_021F5B24(int direction, int target, u8 *value) {
    int current = ov01_021F5AE8(direction);
    u8 result;
    if (target < current) {
        int distance = current - target;
        if (distance == 1) {
            result = *value - 1;
        } else if (distance == 2) {
            result = *value - 2;
        } else {
            GF_AssertFail();
        }
    } else if (target > current) {
        int distance = target - current;
        if (distance == 1) {
            result = *value + 1;
        } else if (distance == 2) {
            result = *value + 2;
        } else {
            GF_AssertFail();
        }
    } else {
        GF_AssertFail();
    }
    *value = result;
}
