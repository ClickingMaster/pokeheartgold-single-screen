#ifndef PRIVATE_H
#define PRIVATE_H
#include "global.h"
extern u8 *gCommState;
extern int WifiConnection_Update(int); extern void sub_0203786C(int); extern int Comm_CountConnected(void); extern void CommState_SetState(void (*)(void),int); extern int sub_02033FC4(int); extern int WifiConnection_StartMatchmaking(int,int,int); extern void sub_0203622C(void);
extern void sub_02038E90(void); extern void sub_02038E94(void); extern void sub_02038EB0(void); extern void sub_02038EB4(void); extern void sub_02038EB8(void); extern void sub_02038F9C(void);
void CommState_SetWifiError(int,int,int); void sub_0203900C(void);
#endif
