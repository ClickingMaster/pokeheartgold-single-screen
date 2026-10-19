#ifndef POKEHEARTGOLD_APPLICATION_TRADE_SEQUENCE_TRADE_SEQUENCE_RECEIVE_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_TRADE_SEQUENCE_TRADE_SEQUENCE_RECEIVE_INTERNAL_H

#include "application/trade_sequence/trade_sequence_internal.h"
#include "application/trade_sequence/trade_sequence_models.h"

#include "bg_window.h"
#include "filesystem.h"
#include "pm_string.h"
#include "pokepic.h"
#include "sprite.h"
#include "sys_task.h"

// Scene 6 of the trade animation.

typedef struct TradeSequenceReceive {
    TradeSequence *ts;
    u32 subState;
    int timer;
    BOOL fanfarePlaying;
    PokepicManager *pokepicManager;
    Pokepic *pokepic;
    PokepicAnimScript animScript[10];
    Sprite *sprites[2];
    TradeSequenceCellAnim cellAnim;
    BgConfig *bgConfig;
    Window window;
    String *msgTemplate;
    String *msgString;
    TradeSequenceModels *models;
    TradeSequenceModel *ball;
    SysTask *brightnessTask;
    SysTask *bounceTask;
    NARC *pokeAnimNarc;
} TradeSequenceReceive;

// The ball fades in, spins faster, rises, falls and bounces once, then lands while the screen brightens.
typedef struct TradeSequenceReceiveBounce {
    SysTask **task;
    int state;
    TradeSequenceReceive *receive;
    int timer;
    TradeSequenceModels *models;
    TradeSequenceModel *ball;
    fx32 alpha;
    fx32 alphaStep;
    int alphaFrames;
    VecFx32 position;
    fx32 speed;
    TradeSequenceRotation rotation;
    s16 spinSpeed;
    s16 spinStep;
    s16 spinTarget;
    u32 spinFrames;
} TradeSequenceReceiveBounce;

void TradeSequenceReceive_StartBrightness(TradeSequenceReceive *receive, int start, int target, int frames);
BOOL TradeSequenceReceive_IsBrightnessDone(TradeSequenceReceive *receive);
void TradeSequenceReceive_StopBounce(SysTask *task);
void TradeSequenceReceive_UpdateSpin(TradeSequenceReceiveBounce *bounce);
void TradeSequenceReceive_SetSpin(TradeSequenceReceiveBounce *bounce, int target, int frames);
void TradeSequenceReceive_BounceTask(SysTask *task, void *data);

#endif // POKEHEARTGOLD_APPLICATION_TRADE_SEQUENCE_TRADE_SEQUENCE_RECEIVE_INTERNAL_H
