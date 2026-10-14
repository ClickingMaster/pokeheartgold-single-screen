#ifndef POKEHEARTGOLD_OVERLAY_01_021EDAFC_H
#define POKEHEARTGOLD_OVERLAY_01_021EDAFC_H

#include "bg_window.h"
#include "field_system.h"

void PointsBox_Delete(Window *window);
void PointsBox_Update(FieldSystem *fieldSystem, Window *window, u8 type);
void MoneyBoxSys_Delete(Window *moneybox);
void MoneyBoxSys_Update(FieldSystem *fieldSystem, Window *moneybox);
Window *SavingsBox_New(FieldSystem *fieldSystem, u8 x, u8 y);
void SavingsBox_Delete(Window *window);
void SavingsBox_Update(FieldSystem *fieldSystem, Window *window);

#endif // POKEHEARTGOLD_OVERLAY_01_021EDAFC_H
