#include "global.h"

#include "constants/sndseq.h"

#include "sound_playback.h"
#include "sys_task.h"
#include "trade_sequence_models.h"
#include "trade_sequence_receive_internal.h"

// Scene 6 of the trade animation, continued (trade_sequence_receive.c): the ball's bounce task. A separate TU: the
// overlay's data lies between the two.

#pragma force_active on

void TradeSequenceReceive_BounceTask(SysTask *task, void *data) {
    TradeSequenceReceiveBounce *bounce = data;

    TradeSequenceReceive_UpdateSpin(bounce);
    switch (bounce->state) {
    case 0:
        if (bounce->alphaFrames) {
            bounce->alpha += bounce->alphaStep;
            bounce->alphaFrames--;
            TradeSequence_SetModelAlpha(bounce->ball, bounce->alpha >> FX32_SHIFT);
        } else {
            TradeSequence_SetModelAlpha(bounce->ball, 31);
            bounce->state++;
        }
        break;
    case 1:
        TradeSequenceReceive_SetSpin(bounce, 384, 30);
        bounce->timer = 0;
        bounce->state++;
        break;
    case 2:
        bounce->timer++;
        if (bounce->timer > 10) {
            bounce->speed = 0;
            bounce->state++;
        }
        break;
    case 3: {
        bounce->position.y += bounce->speed;
        bounce->speed -= 1920;
        TradeSequence_SetModelPosition(bounce->ball, &bounce->position);
        s32 y = bounce->position.y;
        if (y < FX32_CONST(-11)) {
            PlaySE(SEQ_SE_DP_KON);
            bounce->speed *= -1;
            TradeSequenceReceive_StartBrightness(bounce->receive, 0, 16, 8);
            bounce->state++;
        }
        break;
    }
    case 4: {
        bounce->position.y += bounce->speed;
        s32 y = bounce->position.y;
        if (y >= FX32_CONST(19)) {
            bounce->position.y = FX32_CONST(19);
        }
        TradeSequence_SetModelPosition(bounce->ball, &bounce->position);
        if (bounce->position.y == FX32_CONST(19)) {
            bounce->state++;
        }
        break;
    }
    case 5:
        if (TradeSequenceReceive_IsBrightnessDone(bounce->receive)) {
            TradeSequenceReceive_StopBounce(task);
        }
        break;
    }
}
