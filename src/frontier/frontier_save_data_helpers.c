#include "frontier_save_data_helpers_internal.h"

void BattleTower_GenerateOpponents(void *data, void *saveData) {
    FrontierFieldSystem *tower = data;
    int i;
    u16 species[2];
    u16 items[2];

    switch (tower->towerMode) {
    case TOWER_MODE_WIFI:
    case TOWER_MODE_WIFI_PRACTICE:
        BattleTower_GetRoomTrainer(saveData, &tower->opponents[0], (u8)(tower->currentBattleNumber - 1));
        return;
    case TOWER_MODE_MULTI:
    case TOWER_MODE_LINK:
    case TOWER_MODE_6:
        BattleTower_GenerateTrainerMons(data, &tower->opponents[0], tower->trainersInCurrentRound[(tower->currentBattleNumber - 1) * 2], tower->numMons, NULL, NULL, 0, tower->heapID);
        for (i = 0; i < tower->numMons; i++) {
            species[i] = tower->opponents[0].frontierMons[i].species;
            items[i] = tower->opponents[0].frontierMons[i].item;
        }
        BattleTower_GenerateTrainerMons(data, &tower->opponents[1], tower->trainersInCurrentRound[(tower->currentBattleNumber - 1) * 2 + 1], tower->numMons, species, items, 0, tower->heapID);
        return;
    default:
        BattleTower_GenerateTrainerMons(data, &tower->opponents[0], tower->trainersInCurrentRound[tower->currentBattleNumber - 1], tower->numMons, NULL, NULL, 0, tower->heapID);
        return;
    }
}

u32 BattleTower_GetTrainerSprite(void *data, u32 index) {
    FrontierFieldSystem *tower = data;
    return FrontierTrainer_GetClassSprite((u8)tower->opponents[index].trainerClass);
}

void BattleTower_SetPalmerDefeated(void *data, u32 value) {
    FrontierFieldSystem *tower = data;
    tower->palmerDefeated = (u8)value;
}

u32 BattleTower_IncBattleNumber(void *data) {
    FrontierFieldSystem *tower = data;
    tower->curWinStreak++;
    if (tower->towerMode != TOWER_MODE_6) {
        tower->randomState = BattleTower_NextRandom(tower->randomState);
    }
    tower->currentBattleNumber++;
    return tower->currentBattleNumber;
}

u32 Overlay80Counter_GetSumCapped(void *data) {
    FrontierFieldSystem *tower = data;
    u32 streak = *(volatile u8 *)&tower->curWinStreak;
    u32 result = tower->currentBattleInSession + streak;
    u32 ret = 0xFFFF;
    if (result <= ret) {
        ret = (u16)result;
    }
    return ret;
}
