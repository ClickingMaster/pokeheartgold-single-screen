#include "trainer_card_text_panels_private.h"

void ov51_021E76A4(TrainerCardMainState *state) {
    int x = 0x30 - FontID_String_GetWidth(4, *(void **)&state->data[0x33D8], 0);
    FillWindowPixelBuffer(&state->data[0xD4], 0);
    AddTextPrinterParameterizedWithColor(&state->data[0xD4], 4, *(void **)&state->data[0x33D8], (u32)x >> 1, 4, 0, 0x50400, 0);
}

void ov51_021E76EC(TrainerCardMainState *state, int mode) {
    void *window = &state->data[0xC4];

    if (mode == 0) {
        u32 flag = state->data[0x343A];
        flag <<= 30;
        flag >>= 31;
        if (flag != 0) {
            mode = 1;
        }
    }
    switch (mode) {
    case 0: {
        int width;
        FillWindowPixelBuffer(window, 0);
        width = 0x60 - FontID_String_GetWidth(4, *(void **)&state->data[0x33E0], 0);
        AddTextPrinterParameterizedWithColor(window, 4, *(void **)&state->data[0x33E0], (u32)width >> 1, 4, 0, 0x50400, 0);
        ov51_021E7D44((TrainerCardMainState *)&state->data[0x311C], 1, 2, 1);
        break;
    }
    case 1:
        FillWindowPixelBuffer(window, 0);
        AddTextPrinterParameterizedWithColor(window, 0, *(void **)&state->data[0x33DC], 0, 4, 0, 0x50400, 0);
        ov51_021E7D44((TrainerCardMainState *)&state->data[0x311C], 1, 4, 1);
        break;
    }
}
