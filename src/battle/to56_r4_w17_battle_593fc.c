#include "global.h"

#include "constants/heap.h"

#include "battle/battle_system.h"

#include "pokemon.h"
#include "pokepic.h"
#include "unk_02013FDC.h"

typedef struct Ov12B593FCCommand {
    u8 command;
    u8 form;
    u16 species;
    u8 gender;
    u8 shiny;
    u8 pad06[2];
    u32 personality;
} Ov12B593FCCommand;

s16 ov07_02234B5C(u32 flags, u32 value);
void ov12_0226430C(BattleSystem *battleSystem, int battlerId, int command);
void ov12_02259928(OpponentData *opponentData);
void ov12_022593FC(BattleSystem *battleSystem, OpponentData *opponentData);

void ov12_022593FC(BattleSystem *battleSystem, OpponentData *opponentData) {
    PokepicTemplate template;
    Ov12B593FCCommand *command = (Ov12B593FCCommand *)((u8 *)opponentData + 0x94);
    int facing;
    PokepicTemplate *pokepicTemplate;
    u8 *buffer;
    int height;

    if (opponentData->battlerType & 1) {
        facing = 2;
    } else {
        facing = 0;
    }
    GetMonSpriteCharAndPlttNarcIdsEx(&template, command->species, command->gender, facing, command->shiny, command->form, command->personality);
    pokepicTemplate = Pokepic_GetTemplate(opponentData->pokepic);
    *pokepicTemplate = template;
    Pokepic_ScheduleReloadFromNarc(opponentData->pokepic);

    buffer = ov12_0223BB94(ov12_0223A99C(battleSystem), opponentData->unk194);
    sub_02014540((NarcId)pokepicTemplate->narcID, pokepicTemplate->charDataID, HEAP_ID_BATTLE, buffer, command->personality, FALSE, facing, pokepicTemplate->species);
    ov12_0223BBA8(ov12_0223A99C(battleSystem), opponentData->unk194, pokepicTemplate->narcID);
    ov12_0223BBC0(ov12_0223A99C(battleSystem), opponentData->unk194, pokepicTemplate->palDataID);

    height = GetMonPicHeightBySpeciesGenderForm(command->species, command->gender, facing, command->form, command->personality);
    ov12_0223BBD8(ov12_0223A99C(battleSystem), opponentData->unk194, height);
    Pokepic_SetAttr(opponentData->pokepic, 1, height + ov07_02234B5C(opponentData->battlerType, 1));
    ov12_0226430C(battleSystem, opponentData->unk194, command->command);
    ov12_02259928(opponentData);
}
