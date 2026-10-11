#pragma force_active on
#include "global.h"
#include "application/pokedex/pokedex_internal.h"
#include "system.h"
#pragma force_active on
u32 PokedexApp_HandleSearchMenuInput(PokedexAppData*w){MenuInputStateMgr_SetState(w->args->menuInputStatePtr,MENU_INPUT_STATE_BUTTONS);if(gSystem.newKeys&PAD_BUTTON_START)return PAD_BUTTON_START;return GridInputHandler_HandleInput_AllowHold(w->unk_1864);}
