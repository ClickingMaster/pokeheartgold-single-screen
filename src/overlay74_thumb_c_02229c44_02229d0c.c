#include "overlay74_thumb_w06_private.h"

void ov74_02229C44(void) {
    if (sub_02034DB8()) {
        ov74_02229BC0(ov74_02229C5C, 0);
    }
}
void ov74_02229C5C(void) {
}
void ov74_02229C60(void) {
    sub_02037E1C(U8(ov74_0223D0A4, 0x27));
    ov74_02229BC0(ov74_02229C80, 0);
}
void ov74_02229C80(void) {
    if (sub_02037EC8()) {
        U8(ov74_0223D0A4, 0x3C0) = 0;
        sub_02034434();
        sub_02037AC0(0xAB);
        ov74_02229BC0(ov74_02229CB4, 0);
    }
}
void ov74_02229CB4(void) {
}
void ov74_02229CB8(void *task) {
    void (*func)();
    if (ov74_0223D0A4 == NULL) {
        SysTask_Destroy(task);
        return;
    }
    func = (void (*)())PTR(ov74_0223D0A4, 0x20);
    if (func != NULL && U8(ov74_0223D0A4, 0x26) == 0) {
        func();
    }
}
void ov74_02229CE0(void *a, void *b, void *c) {
    if (!sub_02037474()) {
        sub_02038114(b, c);
        ov74_02229B68(a);
        ov74_02229BC0(ov74_02229BD0, 0);
    }
}
int ov74_02229D0C(void *a, void *b) {
    return sub_02037088(0x16, a, b);
}
