#include "to50_overlay102_text_cleanup_r12_internal.h"
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
void ov102_021E937C(u8 *work) {
    u8 *obj = work;
    int i = 0;
    u8 *aux = work + 0x218;
    for (; i < 2; i++) {
        TextOBJ_Destroy(PTR(obj, 0x214));
        sub_02021B5C(aux);
        PTR(obj, 0x214) = NULL;
        obj += 0x10;
        aux += 0x10;
    }
    TextOBJLayout_Delete(PTR(work, 0x200));
    RemoveWindow(work + 0x204);
    FontSystem_Delete(PTR(work, 0x1FC));
}
