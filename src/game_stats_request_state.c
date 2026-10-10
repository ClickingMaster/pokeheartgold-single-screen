#include "global.h"

#include "game_stats_request.h"

BOOL ov00_021FA118(void);
int ov00_021FA21C(char *url, void (*callback)(const char *, int, int), void *arg);
void ov00_021FA0D8(void);
void ov00_021EC2EC(int category, void *ptr, u32 size);

GameStatsRequest gGameStatsRequest = { GAME_STATS_STATE_IDLE };
u32 gGameStatsRandState;

int GameStats_Update(void) {
    switch (gGameStatsRequest.state) {
    case GAME_STATS_STATE_PREPARED:
        gGameStatsRequest.requestId = ov00_021FA21C(gGameStatsRequest.url, GameStats_ResponseCallback, &gGameStatsRequest);
        GameStats_SetRequestError(gGameStatsRequest.requestId);
        if (gGameStatsRequest.requestId >= 0) {
            gGameStatsRequest.state = GAME_STATS_STATE_WAIT_CHALLENGE;
        } else {
            gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
        }
        break;
    case GAME_STATS_STATE_WAIT_CHALLENGE:
        if (!ov00_021FA118()) {
            gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
        }
        break;
    case GAME_STATS_STATE_SEND_DATA:
        gGameStatsRequest.requestId = ov00_021FA21C(gGameStatsRequest.url, GameStats_ResponseCallback, &gGameStatsRequest);
        GameStats_SetRequestError(gGameStatsRequest.requestId);
        if (gGameStatsRequest.requestId >= 0) {
            gGameStatsRequest.state = GAME_STATS_STATE_WAIT_RESPONSE;
        } else {
            gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
        }
        break;
    case GAME_STATS_STATE_WAIT_RESPONSE:
        if (!ov00_021FA118()) {
            gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
        }
        break;
    case GAME_STATS_STATE_DONE:
        break;
    }
    return gGameStatsRequest.state;
}

void GameStats_Shutdown(void) {
    if (gGameStatsRequest.url != NULL) {
        ov00_021EC2EC(10, gGameStatsRequest.url, 0);
        gGameStatsRequest.url = NULL;
    }
    ov00_021FA0D8();
    gGameStatsRequest.state = GAME_STATS_STATE_IDLE;
}

int GameStats_GetError(void) {
    return gGameStatsRequest.error;
}

int GameStats_GetResponseLength(void) {
    return gGameStatsRequest.responseLength;
}
