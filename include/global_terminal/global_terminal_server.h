#ifndef POKEHEARTGOLD_GLOBAL_TERMINAL_SERVER_H
#define POKEHEARTGOLD_GLOBAL_TERMINAL_SERVER_H

#include "global_terminal/global_terminal_net.h"

typedef struct GlobalTerminalServerVideoUpload {
    u32 data[1875];
} GlobalTerminalServerVideoUpload;

// The Global Terminal server connection: the requests are sent by a socket thread; the result is read with
// GlobalTerminalServer_GetState and the response with GlobalTerminalServer_GetResponse.
BOOL GlobalTerminalServer_Init(GlobalTerminalNetLogin *login);
void GlobalTerminalServer_Stop(void);
int GlobalTerminalServer_GetState(void);
int GlobalTerminalServer_GetError(void);
BOOL GlobalTerminalServer_UploadDressUp(GlobalTerminalNetDressUpUpload *upload, void *response);
BOOL GlobalTerminalServer_DownloadDressUp(int id, void *response);
BOOL GlobalTerminalServer_UploadBox(int tray, GlobalTerminalNetBoxUpload *box, void *response);
BOOL GlobalTerminalServer_DownloadBox(int id, void *response);
int GlobalTerminalServer_GetResponseSize(void);
void *GlobalTerminalServer_GetResponse(void);
int GlobalTerminalServer_GetResponseBufferSize(int command);
BOOL GlobalTerminalServer_SendRequest22000(void *response);
BOOL GlobalTerminalServer_GetRankings(GlobalTerminalNetRankingsRequest *request, void *response);
BOOL GlobalTerminalServer_UploadVideo(GlobalTerminalServerVideoUpload *video, void *response);
BOOL GlobalTerminalServer_SearchVideos(GlobalTerminalNetVideoSearch *search, void *response);
BOOL GlobalTerminalServer_SearchVideosKind1(u32 *arg, void *response);
BOOL GlobalTerminalServer_SearchVideosKind2(u32 *arg, void *response);
BOOL GlobalTerminalServer_DownloadVideo(u32 idLow, u32 idHigh, int arg2, void *response);
BOOL GlobalTerminalServer_SendRequest23003(u32 idLow, u32 idHigh, void *response);

#endif // POKEHEARTGOLD_GLOBAL_TERMINAL_SERVER_H
