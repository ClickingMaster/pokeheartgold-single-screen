#ifndef TO53_R3_W01_PRIVATE_H
#define TO53_R3_W01_PRIVATE_H

#include "global.h"
#include "gf_gfx_planes.h"

extern u8 ov112_021FFAA4[];
extern u8 ov112_021FFB08[];
extern u8 ov112_021FFB24[];
extern u8 ov112_021FFBD8[];
extern const GraphicsBanks ov112_021F36DC;

void *PokewalkerIr_IsConnected(void);

void *Save_Bag_Get(void *save);
void *Save_PlayerData_GetProfile(void *save);
void *Save_PlayerData_GetOptionsAddr(void *save);
void *Save_Pokewalker_Get(void *save);
void *Save_Pokedex_Get(void *save);

void DoScheduledBgGpuUpdates(void *bg);
void GF_RunVramTransferTasks(void);
void SpriteSystem_TransferOam(void);

void FreeBgTilemapBuffer(void *bg, int id);

void String_Delete(void *str);
void DestroyMsgData(void *msgData);
void MessageFormat_Delete(void *fmt);
void GfGfxLoader_LoadScrnData(int narcId, int member, void *bg, int bgId, int a4, int a5, int a6, int heapId);

void PokewalkerApp_BuildUploadImage(void *state, void *dst);
void PokewalkerApp_BuildStrollData(void *ptr, void *state);
void ov112_021E7DC0(void *state);
void ov112_021F3244(void *save, void *dst);
void *sub_02032728(void *pokewalker);
void PokewalkerIr_Stop(void);
void *ov112_021E7668(void);
u8 PokewalkerIr_GetChunkProgress(void);
u8 PokewalkerIr_GetTotalProgress(void);
u32 PokewalkerIr_GetAndClearError(void);
void PokewalkerApp_GetSaveData(void *state);
void PokewalkerApp_VBlankCB(void *state);
void PokewalkerApp_FreeBgs(void *bg);
void PokewalkerApp_FreeMessages(void *state);
void PokewalkerApp_LoadTilemap(void *state, int bgId, int member);
void ov112_021E7D7C(void *state);
void ov112_021E7DA4(void *state);
void PokewalkerApp_Teardown(void *state);
void PokewalkerApp_SetGxBanks(void);
void PokewalkerApp_FreeSprites(void *state);
void PokewalkerApp_RemoveWindowBank(void *state, int which);
void PokewalkerApp_FreeBgs(void *bg);
void GF_TouchpadStopSampling(void);
void GF_DestroyVramTransferManager(void);
void PokewalkerApp_FreeStrings(void *state);
void FontID_Release(int fontId);

#endif
