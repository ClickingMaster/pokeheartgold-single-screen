#ifndef POKEHEARTGOLD_TRADE_SEQUENCE_ARRIVE_INTERNAL_H
#define POKEHEARTGOLD_TRADE_SEQUENCE_ARRIVE_INTERNAL_H

#include "bg_window.h"
#include "sys_task.h"
#include "trade_sequence_internal.h"
#include "trade_sequence_models.h"

// Scene 5 of the trade animation (trade_sequence_arrive.c, trade_sequence_arrive_tasks.c).

typedef struct TradeSequenceArrive {
    TradeSequence *ts;              // 0x00
    u32 subState;                   // 0x04
    int timer;                      // 0x08
    BgConfig *bgConfig;             // 0x0C
    TradeSequencePulses *pulses;    // 0x10
    int pulsesRunning;              // 0x14
    TradeSequenceBgZoom *bgZoom;    // 0x18
    int bgZoomFinished;             // 0x1C
    TradeSequenceModels *models;    // 0x20
    TradeSequenceModel *ball;       // 0x24
    TradeSequenceRotation rotation; // 0x28
    u8 padding_2E[2];               // 0x2E
    SysTask *lowerTask;             // 0x30
    SysTask *scrollTask;            // 0x34
    SysTask *alphaFadeTask;         // 0x38
} TradeSequenceArrive;              // size: 0x3C

void TradeSequenceArrive_Init3D(TradeSequenceArrive *arrive);
void TradeSequenceArrive_Free3D(TradeSequenceArrive *arrive);
void TradeSequenceArrive_StartScroll(TradeSequenceArrive *arrive, SysTask **task);
void TradeSequenceArrive_StopScroll(SysTask *task);
void TradeSequenceArrive_StartLower(TradeSequenceArrive *arrive, SysTask **task);
void TradeSequenceArrive_StopLower(SysTask *task);
void TradeSequenceArrive_StartAlphaFade(TradeSequenceModel *model, int start, int target, int frames, SysTask **task);
void TradeSequenceArrive_StopAlphaFade(SysTask *task);

#endif // POKEHEARTGOLD_TRADE_SEQUENCE_ARRIVE_INTERNAL_H
