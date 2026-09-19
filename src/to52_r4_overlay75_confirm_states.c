#include "to52_r4_overlay75_confirm_states_private.h"

int ov75_02247A98(void *work) {
    int input;
    if (ov75_02249534(U32_AT(work, 0x44)) == 1) {
        return 0;
    }
    switch (U32_AT(work, 0x94)) {
    case 0:
        ov75_022494CC(work, PTR_AT(work, 0x34), 0x19, 1, 0xF0F);
        U32_AT(work, 0x94)
        ++;
        break;
    case 1:
        PTR_AT(work, 0x88) = ov75_02247890(PTR_AT(work, 4), 0x234, 0);
        U32_AT(work, 0x94)
        ++;
        break;
    case 2:
        input = YesNoPrompt_HandleInput(PTR_AT(work, 0x88));
        if (input == 1) {
            YesNoPrompt_Destroy(PTR_AT(work, 0x88));
            ov75_022494CC(work, PTR_AT(work, 0x34), 0x1A, 1, 0xF0F);
            sub_02031780(PTR_AT(PTR_AT(work, 0), 4), 0, 1);
            U32_AT(work, 0x94)
            ++;
        } else if (input == 2) {
            YesNoPrompt_Destroy(PTR_AT(work, 0x88));
            ov75_022494CC(work, PTR_AT(work, 0x34), 0x1B, 1, 0xF0F);
            sub_02031780(PTR_AT(PTR_AT(work, 0), 4), 0, 0);
            U32_AT(work, 0x94)
            ++;
        }
        break;
    case 3:
        ov75_02247854(work, 0x19, 0);
        break;
    }
    return 0;
}

int ov75_02247B98(void *work) {
    int input;
    if (ov75_02249534(U32_AT(work, 0x44)) == 1) {
        return 0;
    }
    switch (U32_AT(work, 0x94)) {
    case 0:
        ov75_022494CC(work, PTR_AT(work, 0x34), 0x1C, 1, 0xF0F);
        U32_AT(work, 0x94)
        ++;
        break;
    case 1:
        PTR_AT(work, 0x88) = ov75_02247890(PTR_AT(work, 4), 0x234, 1);
        U32_AT(work, 0x94)
        ++;
        break;
    case 2:
        input = YesNoPrompt_HandleInput(PTR_AT(work, 0x88));
        if (input == 1) {
            YesNoPrompt_Destroy(PTR_AT(work, 0x88));
            ov75_022494CC(work, PTR_AT(work, 0x34), 0x1D, 1, 0xF0F);
            sub_02031734(PTR_AT(PTR_AT(work, 0), 4));
            U32_AT(work, 0x94)
            ++;
        } else if (input == 2) {
            YesNoPrompt_Destroy(PTR_AT(work, 0x88));
            U32_AT(work, 8) = 0;
        }
        break;
    case 3:
        ov75_02247854(work, 0x19, 0);
        break;
    }
    return 0;
}
