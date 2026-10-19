#ifndef POKEHEARTGOLD_GAME_STATS_REQUEST_H
#define POKEHEARTGOLD_GAME_STATS_REQUEST_H

#include "global.h"

// Client for the game-stats web service used by the GTS screens: the server answers "<url>?pid=<pid>" with a
// challenge, then the client sends "&hash=<sha1 hex of salt + challenge>&data=<obfuscated payload, base64>".

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
    int state;     // GameStatsRequestState
    int requestId; // HTTP library request handle, -1 when none
    int error;
    int responseLength;
    int pid; // also the payload obfuscation key
    void *payload;
    int payloadSize;
    void *response;
    int responseCapacity;
    char *url;      // full request text, heap allocated
    char *hashText; // position of the hash after "&hash="
    char *dataText; // position of the payload after "&data="
    int dataTextSize;
} GameStatsRequest;

extern GameStatsRequest gGameStatsRequest;
extern u32 gGameStatsRandState;

void GameStats_Start(void);
int GameStats_PrepareRequest(char *url, int pid, void *payload, int payloadSize, void *response, int responseCapacity);
int GameStats_Update(void);
void GameStats_Shutdown(void);
int GameStats_GetError(void);
int GameStats_GetResponseLength(void);

#endif // POKEHEARTGOLD_GAME_STATS_REQUEST_H
