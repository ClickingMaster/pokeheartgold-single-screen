#pragma force_active on
#include "global.h"
void ov13_02222850(s8 *in, u8 *out, int length);
// @fn ov13_02222850
void ov13_02222850(s8 *in, u8 *out, int length) { int i; int half = length / 2; for (i = 0; i < half; i++) { out[i + half] ^= in[i]; } }
