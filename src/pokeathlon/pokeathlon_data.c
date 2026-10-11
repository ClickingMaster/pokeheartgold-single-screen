#include "pokeathlon/pokeathlon.h"

extern void PokeathlonDiscCatch_Intro(void);
extern void PokeathlonDiscCatch_Play(void);
extern void PokeathlonDiscCatch_Finish(void);
extern void PokeathlonGoalRoll_State_Play(void);
extern void PokeathlonGoalRoll_State_Finish(void);
extern void PokeathlonRingDrop_State_FadeIn(void);
extern void PokeathlonRingDrop_State_Play(void);
extern void PokeathlonRingDrop_State_Finish(void);

u8 ov96_0221DC80[] = { 0x00, 0x04, 0x02, 0x01, 0x00, 0x01, 0x00, 0x01 };

void *ov96_0221DC88[] = {
    PokeathlonGoalRoll_State_Play,
    PokeathlonGoalRoll_State_Finish
};

void *ov96_0221DC90[] = {
    PokeathlonRingDrop_State_FadeIn,
    PokeathlonRingDrop_State_Play,
    PokeathlonRingDrop_State_Finish,
    NULL
};

// u8 _0221DCA0[0x900]; // Non-matching yet if moved here (not sure why)

u8 ov96_0221E5A0[4];
u8 ov96_0221E5A4[0x1c];
