#ifndef POKEHEARTGOLD_GTS_SERVER_H
#define POKEHEARTGOLD_GTS_SERVER_H

#include "global.h"

// Requests to the GTS server (gamestats2.gs.nintendowifi.net/pokemondpds), one at a time: start a request,
// call GtsServer_Update every frame until GtsServer_IsRequestDone, then read GtsServer_GetResult.
// The Wii message settings (OVY_75) use them for common/setProfile.asp.
void GtsServer_Init(u32 pid, u32 friendKeyLo, u32 friendKeyHi);
void GtsServer_Exit(void);
void GtsServer_Update(void);
BOOL GtsServer_IsRequestDone(void);
int GtsServer_GetResult(void);
void GtsServer_RequestPost(const void *record);
void GtsServer_RequestPostFinish(void);
void GtsServer_RequestGet(void *response);
void GtsServer_RequestResult(void *response);
void GtsServer_RequestDelete(void);
void GtsServer_RequestReturn(void);
void GtsServer_RequestSearch(const void *criteria, int count, void *response);
void GtsServer_RequestSearchEx(const void *criteria, void *response);
void GtsServer_RequestExchange(void *arg, const void *record, void *response);
void GtsServer_RequestExchangeFinish(void);
void GtsServer_RequestInfo(void);
void GtsServer_RequestSetProfile(void *profile, void *response);

#endif // POKEHEARTGOLD_GTS_SERVER_H
