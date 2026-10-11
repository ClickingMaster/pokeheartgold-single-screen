#ifndef POKEHEARTGOLD_TRADE_SEQUENCE_INTERNAL_H
#define POKEHEARTGOLD_TRADE_SEQUENCE_INTERNAL_H

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pokemon_types_def.h"
#include "sprite.h"

// Shared by the TUs of the trade animation app (OVY_71, trade_sequence.h): the app work and the helpers the scenes
// use. The app's args are a TRADE_ANIM_WORK (trade_anim.h).

typedef struct TradeSequence TradeSequence;
typedef struct TradeSequencePulses TradeSequencePulses;
typedef struct TradeSequenceBgZoom TradeSequenceBgZoom;

// A cell bank and an animation bank loaded from a NARC (TradeSequence_LoadCellAnim).
typedef struct TradeSequenceCellAnim {
    void *cellBankFile;           // 0x0
    void *animBankFile;           // 0x4
    NNSG2dCellDataBank *cellBank; // 0x8
    NNSG2dAnimBankData *animBank; // 0xC
} TradeSequenceCellAnim;          // size: 0x10

TradeSequencePulses *TradeSequence_CreatePulses(int *running, int direction);
void TradeSequence_StopPulses(TradeSequencePulses *pulses);
void TradeSequence_DestroyPulses(TradeSequencePulses *pulses);
TradeSequenceBgZoom *TradeSequence_StartBgZoom(BgConfig *bgConfig, fx32 scale, fx32 targetScale, fx32 speed, fx32 acceleration, int centerYIndex, int *finished);
void TradeSequence_LoadMonPicToBg(TradeSequence *ts, u32 whichMon, u32 bgId, u32 palette, BOOL large);
void TradeSequence_LoadMonPicTilemap(TradeSequence *ts, int whichMon, int bgId, int palette, int x, int y);
void TradeSequence_LoadCellAnim(TradeSequenceCellAnim *cellAnim, NarcId narcId, s32 cellBankId, s32 animBankId);
void TradeSequence_FreeCellAnim(TradeSequenceCellAnim *cellAnim);
void TradeSequence_InitSpriteHeader(SpriteResourcesHeader *header, TradeSequenceCellAnim *cellAnim, const NNSG2dImageProxy *imageProxy, const NNSG2dImagePaletteProxy *plttProxy, u32 priority);
Sprite *TradeSequence_CreateSprite(TradeSequence *ts, const SpriteResourcesHeader *header, int x, int y, u32 priority, NNS_G2D_VRAM_TYPE whichScreen);
BgConfig *TradeSequence_GetBgConfig(TradeSequence *ts);
MessageFormat *TradeSequence_GetMsgFormat(TradeSequence *ts);
MsgData *TradeSequence_GetMsgData(TradeSequence *ts);
BoxPokemon *TradeSequence_GetSentBoxMon(TradeSequence *ts);
BoxPokemon *TradeSequence_GetReceivedBoxMon(TradeSequence *ts);
int TradeSequence_GetTimeOfDay(TradeSequence *ts);
u16 TradeSequence_GetSentSpecies(TradeSequence *ts);
u16 TradeSequence_GetSentForm(TradeSequence *ts);
u16 TradeSequence_GetReceivedSpecies(TradeSequence *ts);
u16 TradeSequence_GetReceivedForm(TradeSequence *ts);
u32 TradeSequence_GetFrameType(TradeSequence *ts);
int TradeSequence_GetTradeMode(TradeSequence *ts);
void TradeSequence_DeferFree(void *ptr);

#endif // POKEHEARTGOLD_TRADE_SEQUENCE_INTERNAL_H
