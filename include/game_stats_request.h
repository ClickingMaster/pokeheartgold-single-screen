#ifndef POKEHEARTGOLD_GAME_STATS_REQUEST_H
#define POKEHEARTGOLD_GAME_STATS_REQUEST_H

#include "global.h"

// Request/response client for the game-stats web service used by the GTS screens.
// A request is "<url>?pid=<pid>"; the server answers with a 32-character challenge, the client then
// sends "&hash=<sha1 hex of salt + challenge>&data=<obfuscated payload, base64>" and stores the
// response.

typedef enum GameStatsRequestState {
    GAME_STATS_STATE_IDLE = 1,
    GAME_STATS_STATE_STARTED,
    GAME_STATS_STATE_PREPARED,
    GAME_STATS_STATE_WAIT_CHALLENGE,
    GAME_STATS_STATE_SEND_DATA,
    GAME_STATS_STATE_WAIT_RESPONSE,
    GAME_STATS_STATE_DONE,
} GameStatsRequestState;

typedef struct GameStatsRequest {
    int state;            // 0x00, GameStatsRequestState
    int requestId;        // 0x04, HTTP library request handle, -1 when none
    int error;            // 0x08
    int responseLength;   // 0x0C
    int pid;              // 0x10, also the payload obfuscation key
    void *payload;        // 0x14
    int payloadSize;      // 0x18
    void *response;       // 0x1C
    int responseCapacity; // 0x20
    char *url;            // 0x24, full request text, heap allocated
    char *hashText;       // 0x28, position of the hash after "&hash="
    char *dataText;       // 0x2C, position of the payload after "&data="
    int dataTextSize;     // 0x30
} GameStatsRequest;

extern GameStatsRequest gGameStatsRequest;
extern u32 gGameStatsRandState;

void GameStats_SeedRandom(u32 seed);
u8 GameStats_NextRandom(void);
int GameStats_EncodePayload(u32 key, const u8 *data, int len, char *out, int outSize);
int GameStats_GetBase64Length(int size);
void GameStats_SetRequestError(int result);
void GameStats_ResponseCallback(const char *response, int length, int error);
void GameStats_Start(void);
int GameStats_PrepareRequest(char *url, int pid, void *payload, int payloadSize, void *response, int responseCapacity);
int GameStats_Update(void);
void GameStats_Shutdown(void);
int GameStats_GetError(void);
int GameStats_GetResponseLength(void);

#endif // POKEHEARTGOLD_GAME_STATS_REQUEST_H
