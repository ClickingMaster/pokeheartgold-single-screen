#ifndef POKEHEARTGOLD_PHONE_REMATCH_H
#define POKEHEARTGOLD_PHONE_REMATCH_H

#include "gf_rtc.h"
#include "phonebook_dat.h"
#include "save.h"

u16 PhoneBookTrainerGetRematchInfo(u8 idx, SaveData *saveData, struct PhoneBook *phoneBook, u8 timeOfDay);

#endif // POKEHEARTGOLD_PHONE_REMATCH_H
