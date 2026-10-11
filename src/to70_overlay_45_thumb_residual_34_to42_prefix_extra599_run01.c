#pragma force_active on
#include "global.h"
extern int WifiHistory_GetPlayerCountry(void *);
extern int WiFiHistory_GetPlayerRegion(void *);
extern void ov45_022322A4(int,int,void *,void *,int);
extern int WiFiPlazaLobbyClient_CheckResult(void);
extern void *ov45_02254EF0;
typedef struct {u16 country;u8 region,active;} Entry;
typedef struct {u8 p[8];void *history;u8 p2[0x128];Entry entries[20];} Work;
int ov45_0222F7B4(Work *);
int ov45_0222F7B4(Work *w){int country=WifiHistory_GetPlayerCountry(w->history);int region=WiFiHistory_GetPlayerRegion(w->history);int i;if(!country)return 0;for(i=0;i<20;i++){if(!w->entries[i].active)break;if(country==w->entries[i].country&&region==w->entries[i].region)return 0;}if(i>=20){GF_AssertFail();return 0;}w->entries[i].country=country;w->entries[i].region=region;w->entries[i].active=1;ov45_022322A4(-1,0,ov45_02254EF0,w->entries,80);return WiFiPlazaLobbyClient_CheckResult();}
