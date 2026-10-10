#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u32 flags;
    u8 unused[44];
    s32 scaleS, scaleT;
} TextureInput;

typedef struct {
    u32 command, matrixMode;
    MtxFx44 matrix;
    u32 restoreMode;
} TexturePacket;

extern void (*_02110C3C[8])(MtxFx44 *, const TextureInput *);
void NNS_G3dGeBufferOP_N(u32, const u32 *, u32);
void NNSi_G3dSendTexSRT3dsMax(const TextureInput *);

void NNSi_G3dSendTexSRT3dsMax(const TextureInput *input) {
    TexturePacket packet;
    int zero = 0;
    if (input->flags & 8) {
        packet.command = 0x101610;
    } else {
        packet.command = 0x101810;
    }
    packet.matrixMode = 3;
    packet.restoreMode = 2;
    packet.matrix._02 = packet.matrix._03 = packet.matrix._12 = packet.matrix._13 = packet.matrix._20 = packet.matrix._21 = packet.matrix._22 = packet.matrix._23 = packet.matrix._32 = zero;
    packet.matrix._33 = 4096;
    _02110C3C[input->flags & 7](&packet.matrix, input);
    if (input->scaleS != 4096) {
        packet.matrix._00 = (s64)input->scaleS * packet.matrix._00 >> 12;
        packet.matrix._01 = (s64)input->scaleS * packet.matrix._01 >> 12;
        packet.matrix._30 = (s64)input->scaleS * packet.matrix._30 >> 12;
    }
    if (input->scaleT != 4096) {
        packet.matrix._10 = (s64)input->scaleT * packet.matrix._10 >> 12;
        packet.matrix._11 = (s64)input->scaleT * packet.matrix._11 >> 12;
        packet.matrix._31 = (s64)input->scaleT * packet.matrix._31 >> 12;
    }
    NNS_G3dGeBufferOP_N(packet.command, (const u32 *)&packet + 1, 18);
}
