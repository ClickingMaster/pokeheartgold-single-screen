#include "gts_server.h"

#include "global.h"

#include "game_stats_request.h"
#include "gts_internal.h"

void GtsServer_Init(u32 pid, u32 friendKeyLo, u32 friendKeyHi) {
    gGtsServer.state = GTS_SERVER_STATE_IDLE;
    gGtsServer.result = 0;
    gGtsServer.pid = pid;
    gGtsServer.friendKey[0] = friendKeyLo;
    gGtsServer.friendKey[1] = friendKeyHi;
}

// Polls the request in flight; when it completes, maps the server's answer to a result
// (>= 0: success, -1..-13: server answers, -14/-15: connection errors) and goes to GTS_SERVER_STATE_DONE.
void GtsServer_Update(void) {
    u32 responseLength;

    switch (gGtsServer.state) {
    case GTS_SERVER_STATE_POST:
        switch (GameStats_Update()) {
        case GAME_STATS_STATE_IDLE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            gGtsServer.result = GtsServer_GetErrorResult(GameStats_GetError());
            GameStats_Shutdown();
            return;
        case GAME_STATS_STATE_DONE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            switch (ov70_02246920.shortResponse[0]) {
            case 1:
                gGtsServer.result = 0;
                break;
            case 2:
                gGtsServer.result = -5;
                break;
            case 3:
                gGtsServer.result = -4;
                break;
            case 7:
                gGtsServer.result = -1;
                break;
            case 12:
                gGtsServer.result = -6;
                break;
            case 13:
                gGtsServer.result = -7;
                break;
            case 8:
                gGtsServer.result = -8;
                break;
            case 9:
                gGtsServer.result = -9;
                break;
            case 10:
                gGtsServer.result = -10;
                break;
            case 11:
                gGtsServer.result = -11;
                break;
            case 14:
                gGtsServer.result = -2;
                break;
            default:
                gGtsServer.result = -13;
                break;
            }
            GameStats_Shutdown();
            return;
        default:
            return;
        }
    case GTS_SERVER_STATE_POST_FINISH:
        switch (GameStats_Update()) {
        case GAME_STATS_STATE_IDLE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            gGtsServer.result = GtsServer_GetErrorResult(GameStats_GetError());
            GameStats_Shutdown();
            return;
        case GAME_STATS_STATE_DONE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            switch (ov70_02246920.shortResponse[0]) {
            case 1:
                gGtsServer.result = 0;
                break;
            case 2:
                gGtsServer.result = -5;
                break;
            case 3:
                gGtsServer.result = -4;
                break;
            case 5:
                gGtsServer.result = -3;
                break;
            case 14:
                gGtsServer.result = -2;
                break;
            default:
                gGtsServer.result = -13;
                break;
            }
            GameStats_Shutdown();
            return;
        default:
            return;
        }
    case GTS_SERVER_STATE_GET:
        switch (GameStats_Update()) {
        case GAME_STATS_STATE_IDLE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            gGtsServer.result = GtsServer_GetErrorResult(GameStats_GetError());
            GameStats_Shutdown();
            return;
        case GAME_STATS_STATE_DONE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            if (GameStats_GetResponseLength() == GTS_RECORD_SIZE) {
                gGtsServer.result = 0;
            } else {
                switch (ov70_02246900.response[0]) {
                case 5:
                    gGtsServer.result = -3;
                    break;
                case 3:
                    gGtsServer.result = -4;
                    break;
                case 14:
                    gGtsServer.result = -2;
                    break;
                default:
                    gGtsServer.result = -13;
                    break;
                }
            }
            GameStats_Shutdown();
            return;
        default:
            return;
        }
    case GTS_SERVER_STATE_RESULT:
        switch (GameStats_Update()) {
        case GAME_STATS_STATE_IDLE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            gGtsServer.result = GtsServer_GetErrorResult(GameStats_GetError());
            GameStats_Shutdown();
            return;
        case GAME_STATS_STATE_DONE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            if (GameStats_GetResponseLength() == GTS_RECORD_SIZE) {
                gGtsServer.result = 1;
            } else {
                switch (ov70_02246900.response[0]) {
                case 5:
                    gGtsServer.result = -3;
                    break;
                case 3:
                    gGtsServer.result = -4;
                    break;
                case 4:
                    gGtsServer.result = 0;
                    break;
                case 14:
                    gGtsServer.result = -2;
                    break;
                default:
                    gGtsServer.result = -13;
                    break;
                }
            }
            GameStats_Shutdown();
            return;
        default:
            return;
        }
    case GTS_SERVER_STATE_DELETE:
        switch (GameStats_Update()) {
        case GAME_STATS_STATE_IDLE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            gGtsServer.result = GtsServer_GetErrorResult(GameStats_GetError());
            GameStats_Shutdown();
            return;
        case GAME_STATS_STATE_DONE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            switch (ov70_02246920.shortResponse[0]) {
            case 1:
                gGtsServer.result = 0;
                break;
            case 5:
                gGtsServer.result = -3;
                break;
            case 3:
                gGtsServer.result = -4;
                break;
            case 14:
                gGtsServer.result = -2;
                break;
            default:
                gGtsServer.result = -13;
                break;
            }
            GameStats_Shutdown();
            return;
        default:
            return;
        }
    case GTS_SERVER_STATE_RETURN:
        switch (GameStats_Update()) {
        case GAME_STATS_STATE_IDLE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            gGtsServer.result = GtsServer_GetErrorResult(GameStats_GetError());
            GameStats_Shutdown();
            return;
        case GAME_STATS_STATE_DONE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            switch (ov70_02246920.shortResponse[0]) {
            case 1:
                gGtsServer.result = 0;
                break;
            case 5:
                gGtsServer.result = -3;
                break;
            case 2:
                gGtsServer.result = -5;
                break;
            case 3:
                gGtsServer.result = -4;
                break;
            case 14:
                gGtsServer.result = -2;
                break;
            default:
                gGtsServer.result = -13;
                break;
            }
            GameStats_Shutdown();
            return;
        default:
            return;
        }
    case GTS_SERVER_STATE_SEARCH:
        switch (GameStats_Update()) {
        case GAME_STATS_STATE_IDLE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            gGtsServer.result = GtsServer_GetErrorResult(GameStats_GetError());
            GameStats_Shutdown();
            return;
        case GAME_STATS_STATE_DONE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            responseLength = GameStats_GetResponseLength();
            if (responseLength >= GTS_RECORD_SIZE) {
                // the number of records found
                gGtsServer.result = (u32)GameStats_GetResponseLength() / GTS_RECORD_SIZE;
            } else if (responseLength == 0) {
                gGtsServer.result = 0;
            } else if (ov70_02246900.response[0] == 14) {
                gGtsServer.result = -2;
            } else {
                gGtsServer.result = -13;
            }
            GameStats_Shutdown();
            return;
        default:
            return;
        }
    case GTS_SERVER_STATE_EXCHANGE:
        switch (GameStats_Update()) {
        case GAME_STATS_STATE_IDLE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            gGtsServer.result = GtsServer_GetErrorResult(GameStats_GetError());
            GameStats_Shutdown();
            return;
        case GAME_STATS_STATE_DONE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            if (GameStats_GetResponseLength() == GTS_RECORD_SIZE) {
                gGtsServer.result = 0;
            } else {
                switch (ov70_02246900.response[0]) {
                case 2:
                    gGtsServer.result = -5;
                    break;
                case 12:
                    gGtsServer.result = -6;
                    break;
                case 13:
                    gGtsServer.result = -7;
                    break;
                case 8:
                    gGtsServer.result = -8;
                    break;
                case 9:
                    gGtsServer.result = -9;
                    break;
                case 10:
                    gGtsServer.result = -10;
                    break;
                case 11:
                    gGtsServer.result = -11;
                    break;
                case 14:
                    gGtsServer.result = -2;
                    break;
                default:
                    gGtsServer.result = -13;
                    break;
                }
            }
            GameStats_Shutdown();
            return;
        default:
            return;
        }
    case GTS_SERVER_STATE_EXCHANGE_FINISH:
        switch (GameStats_Update()) {
        case GAME_STATS_STATE_IDLE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            gGtsServer.result = GtsServer_GetErrorResult(GameStats_GetError());
            GameStats_Shutdown();
            return;
        case GAME_STATS_STATE_DONE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            switch (ov70_02246920.shortResponse[0]) {
            case 1:
                gGtsServer.result = 0;
                break;
            case 14:
                gGtsServer.result = -2;
                break;
            case 2:
                gGtsServer.result = -5;
                break;
            default:
                break;
            }
            GameStats_Shutdown();
            return;
        default:
            return;
        }
    case GTS_SERVER_STATE_INFO:
        switch (GameStats_Update()) {
        case GAME_STATS_STATE_IDLE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            gGtsServer.result = GtsServer_GetErrorResult(GameStats_GetError());
            GameStats_Shutdown();
            return;
        case GAME_STATS_STATE_DONE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            switch (ov70_02246920.shortResponse[0]) {
            case 1:
                gGtsServer.result = 0;
                break;
            case 6:
                gGtsServer.result = 1;
                break;
            case 7:
                gGtsServer.result = 2;
                break;
            case 14:
                gGtsServer.result = -2;
                break;
            default:
                gGtsServer.result = -13;
                break;
            }
            GameStats_Shutdown();
            return;
        default:
            return;
        }
    case GTS_SERVER_STATE_SET_PROFILE:
        switch (GameStats_Update()) {
        case GAME_STATS_STATE_IDLE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            gGtsServer.result = GtsServer_GetErrorResult(GameStats_GetError());
            GameStats_Shutdown();
            return;
        case GAME_STATS_STATE_DONE:
            gGtsServer.state = GTS_SERVER_STATE_DONE;
            if (GameStats_GetResponseLength() == 8) {
                gGtsServer.result = 0;
            } else {
                switch (ov70_02246920.shortResponse[0]) {
                case 1:
                    gGtsServer.result = 0;
                    break;
                case 6:
                    gGtsServer.result = 1;
                    break;
                case 7:
                    gGtsServer.result = 2;
                    break;
                case 14:
                    gGtsServer.result = -2;
                    break;
                default:
                    gGtsServer.result = -13;
                    break;
                }
            }
            GameStats_Shutdown();
            return;
        default:
            return;
        }
    case 3: // odd states: never set here
    case 5:
    case 7:
    case 9:
    case 11:
    case 13:
    case 15:
    case 17:
    case 19:
    case 21:
    case 23:
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -12;
        GameStats_Shutdown();
        return;
    case GTS_SERVER_STATE_NONE:
    case GTS_SERVER_STATE_IDLE:
    case GTS_SERVER_STATE_DONE:
    default:
        return;
    }
}
