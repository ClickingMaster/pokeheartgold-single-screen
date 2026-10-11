#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_CONNECT_TO_RANGER_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_CONNECT_TO_RANGER_H

#include "overlay_manager.h"

typedef struct ConnectToRangerApp ConnectToRangerApp;

extern const OverlayManagerTemplate gApp_MainMenu_SelectOption_ConnectToRanger;

u16 ConnectToRanger_ConvertCharToRanger(u16 c);
void ConnectToRanger_FormatNumber5Digits(u16 *dst, int value);
void ConnectToRanger_EncodePlayerData(ConnectToRangerApp *app);
void ConnectToRanger_SetupGraphics(ConnectToRangerApp *app);
void ConnectToRanger_LoadSubScreenBg(ConnectToRangerApp *app);
BOOL ConnectToRanger_ShowPokedexRequiredMessage(ConnectToRangerApp *app);
void ConnectToRanger_PrintScreen(ConnectToRangerApp *app);
void ConnectToRanger_PrintPlayerInfo(ConnectToRangerApp *app);
void ConnectToRanger_FreeGraphics(OverlayManager *manager);
BOOL ConnectToRanger_Init(OverlayManager *manager, int *state);
BOOL ConnectToRanger_Main(OverlayManager *manager, int *state);
BOOL ConnectToRanger_Exit(OverlayManager *manager, int *state);
void ConnectToRanger_StoreManaphyEggCard(ConnectToRangerApp *app);
void ConnectToRanger_SetTimeout(ConnectToRangerApp *app, int frames);
BOOL ConnectToRanger_CheckTimeoutOrCancel(ConnectToRangerApp *app, u32 *connectState, BOOL checkLink, u32 cancelKeys);
int ConnectToRanger_RunTransfer(ConnectToRangerApp *app);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_CONNECT_TO_RANGER_H
