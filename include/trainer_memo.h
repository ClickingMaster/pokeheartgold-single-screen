#ifndef POKEHEARTGOLD_TRAINER_MEMO_H
#define POKEHEARTGOLD_TRAINER_MEMO_H

#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pokemon_types_def.h"

// Each string in the notepad and which line it starts on.
typedef struct MemoNotepad {
    int natureLine;
    String *nature;
    int dateLocationMetLine;
    String *dateLocationMet;
    int characteristicLine;
    String *characteristic;
    int flavorPreferenceLine;
    String *flavorPreference;
    int eggWatchLine;
    String *eggWatch;
} MemoNotepad;

typedef struct TrainerMemo {
    enum HeapID heapID;
    MsgData *msgData;
    MessageFormat *msgFmt;
    Pokemon *mon;
    BOOL isMine;
    MemoNotepad notepad;
} TrainerMemo;

void MonSetTrainerMemo(Pokemon *mon, PlayerProfile *profile, int strat, int mapsec, enum HeapID heapID);
void BoxMonSetTrainerMemo(BoxPokemon *boxMon, PlayerProfile *profile, int strat, int mapsec, enum HeapID heapID);
BOOL MonMetadataMatchesEvent(u8 eventNo, Pokemon *mon, BOOL isMine);
TrainerMemo *TrainerMemo_New(Pokemon *a0, int a1, enum HeapID a2, int a3);
void TrainerMemo_Delete(TrainerMemo *a0);

#endif // POKEHEARTGOLD_TRAINER_MEMO_H
