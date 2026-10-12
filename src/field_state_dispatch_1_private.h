#ifndef PRIVATE_H
#define PRIVATE_H
#include "global.h"
extern u8 *gCommState;
extern int WifiConnection_Update(int); extern void sub_0203786C(int); extern int Comm_CountConnected(void); extern void CommState_SetState(void (*)(void),int); extern int sub_02033FC4(int); extern int WifiConnection_StartMatchmaking(int,int,int); extern void sub_0203622C(void);
extern void CommState_WifiFailed(void); extern void CommState_WifiTimedOut(void); extern void CommState_WifiDisconnected(void); extern void CommState_WifiCancelled(void); extern void CommState_WifiConnected(void); extern void CommState_WifiLobby(void);
void CommState_SetWifiError(int,int,int); void CommState_WifiStartMatch(void);
#endif
