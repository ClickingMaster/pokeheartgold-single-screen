#include "slop_r1_overlay83_m10_private.h"

GridInputHandler *ov83_02247A24(void *param0, int param1, int param2) {
    if (param2 == 3) {
        return GridInputHandler_Create(ov83_02248530, ov83_022485A8, &ov83_02248500, param0, TRUE, param1, HEAP_ID_107);
    }
    return GridInputHandler_Create(ov83_02248558, ov83_022485E8, &ov83_02248500, param0, TRUE, param1, HEAP_ID_107);
}

GridInputHandler *ov83_02247A7C(void *param0, int param1, int param2) {
    if (param2 == 3) {
        return GridInputHandler_Create(ov83_02248530, ov83_022485A8, &ov83_02248510, param0, TRUE, param1, HEAP_ID_107);
    }
    return GridInputHandler_Create(ov83_02248558, ov83_022485E8, &ov83_02248510, param0, TRUE, param1, HEAP_ID_107);
}

int ov83_02247AD4(GridInputHandler *inputHandler) {
    int input = GridInputHandler_HandleInput_NoHold(inputHandler);
    switch (input) {
    case GRID_MENU_BUTTON_MODE:
    case GRID_MENU_CURSOR_MOVE:
    case GRID_MENU_CANCEL:
    case GRID_MENU_NOTHING_CHOSEN:
        return input;
    }
    return ov83_02248544[input];
}
