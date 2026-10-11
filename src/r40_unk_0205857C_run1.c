#include "r40_unk_0205857C_private.h"

void sub_0205857C(void) {
    int i;

    for (i = 0; i < Comm_CountConnected(); i++) {
        if (i != Comm_GetLocalNetId() && sub_02037B5C(i) == 0x5e && _021D41C8->field->unk10 == 0 && _021D41C8->field->low6 == 0) {
            int j;
            for (j = 0; j < 4; j++) {
                if (_021D41C8->slots[j] != NULL) {
                    Heap_Free(_021D41C8->slots[j]);
                    _021D41C8->slots[j] = NULL;
                }
            }
            StartMapSceneScript(_021D41C8->field, 0x238e, 0);
        }
    }
    sub_020399FC(4, _021D41C8->field->unk08);
}
