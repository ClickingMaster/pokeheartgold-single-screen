#pragma force_active on
#include <nitro/types.h>
#include <nitro/code32.h>
typedef enum { SND_CAPTURE_REPEAT = 0, SND_CAPTURE_ONESHOT = 1 } SNDCaptureRepeat;
void SND_SetupCapture(int capture, int format, u32 dest, int length, BOOL loop, int in, int out);
// @fn SND_SetupCapture
void SND_SetupCapture(int capture, int format, u32 dest, int length, BOOL loop, int in, int out)
{
    int offset = capture * 8;
    SNDCaptureRepeat repeat = loop ? SND_CAPTURE_REPEAT : SND_CAPTURE_ONESHOT;
    *(vu8 *)(0x04000508 + capture) = (u8)((format << 3) | (repeat << 2) | (in << 1) | out);
    *(vu32 *)(0x04000510 + offset) = dest;
    *(vu16 *)(0x04000514 + offset) = (u16)length;
}
