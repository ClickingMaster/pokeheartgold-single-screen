#ifndef GUARD_POKEHEARTGOLD_UNK_0207F42C_H
#define GUARD_POKEHEARTGOLD_UNK_0207F42C_H

#include "party_menu.h"

u32 GetPartyMenuContextMenuActionFunc(int index);
int PartyMenu_Subtask_PrintTakeItemMessage(PartyMenu *partyMenu);
int PartyMenu_Subtask_PrintTakeGriseousOrbMessage(PartyMenu *partyMenu);
int PartyMenu_Subtask_WaitTakeGriseousOrbAnim(PartyMenu *partyMenu);
int PartyMenu_Subtask_AfterTakeMail(PartyMenu *partyMenu);
int PartyMenu_ConfirmRemoveCapsuleYes(PartyMenu *partyMenu);
int PartyMenu_ConfirmRemoveCapsuleNo(PartyMenu *partyMenu);
void PartyMenu_CancelSwitchMon(PartyMenu *partyMenu);
void PartyMenu_BeginSwapMons(PartyMenu *partyMenu);
BOOL PartyMenu_AnimateSwapMons(PartyMenu *partyMenu);
int PartyMenu_Subtask_SelectMonsErrorMsgClose(PartyMenu *partyMenu);
int PartyMenu_HandleSetMonCapsule(PartyMenu *partyMenu);
int PartyMenu_Subtask_ReturnToSelectMon(PartyMenu *partyMenu);

#endif // GUARD_POKEHEARTGOLD_UNK_0207F42C_H
