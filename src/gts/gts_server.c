#include "gts/gts_server.h"

#include "global.h"

#include "dwc/net_common_internal.h"
#include "gts/gts_internal.h"
#include "nitrowifi/wcm_internal.h"

#include "game_stats_request.h"

static BOOL GtsServer_StartRequest(const char *url, void *payload, int payloadSize, void *response, int responseCapacity);
static int GtsServer_GetErrorResult(int error);

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
            switch (gGtsServerAnswer.shortResponse[0]) {
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
            switch (gGtsServerAnswer.shortResponse[0]) {
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
                switch (gGtsServerPending.response[0]) {
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
                switch (gGtsServerPending.response[0]) {
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
            switch (gGtsServerAnswer.shortResponse[0]) {
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
            switch (gGtsServerAnswer.shortResponse[0]) {
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
            } else if (gGtsServerPending.response[0] == 14) {
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
                switch (gGtsServerPending.response[0]) {
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
            switch (gGtsServerAnswer.shortResponse[0]) {
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
            switch (gGtsServerAnswer.shortResponse[0]) {
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
                switch (gGtsServerAnswer.shortResponse[0]) {
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

void GtsServer_Exit(void) {
    gGtsServer.state = GTS_SERVER_STATE_NONE;
}

BOOL GtsServer_IsRequestDone(void) {
    switch (gGtsServer.state) {
    case GTS_SERVER_STATE_IDLE:
        return TRUE;
    case GTS_SERVER_STATE_DONE:
        gGtsServer.state = GTS_SERVER_STATE_IDLE;
        return TRUE;
    default:
        return FALSE;
    }
}

int GtsServer_GetResult(void) {
    return gGtsServer.result;
}

void GtsServer_RequestPost(const void *record) {
    memcpy(gGtsServerRequest, record, GTS_RECORD_SIZE);
    GameStats_Start();
    if (GtsServer_StartRequest("http://gamestats2.gs.nintendowifi.net/pokemondpds/worldexchange/post.asp", gGtsServerRequest, GTS_RECORD_SIZE, gGtsServerShortResponse, 2)) {
        gGtsServer.state = GTS_SERVER_STATE_POST;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

void GtsServer_RequestPostFinish(void) {
    GtsFriendKey *src = &gGtsServerFriendKey;
    GtsFriendKey *dest = (GtsFriendKey *)gGtsServerRequest;
    dest->lo = src->lo;
    dest->hi = src->hi;
    GameStats_Start();
    if (GtsServer_StartRequest("http://gamestats2.gs.nintendowifi.net/pokemondpds/worldexchange/post_finish.asp", gGtsServerRequest, sizeof(GtsFriendKey), gGtsServerShortResponse, 2)) {
        gGtsServer.state = GTS_SERVER_STATE_POST_FINISH;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

void GtsServer_RequestGet(void *response) {
    gGtsServerPending.response = response;
    GameStats_Start();
    if (GtsServer_StartRequest("http://gamestats2.gs.nintendowifi.net/pokemondpds/worldexchange/get.asp", gGtsServerRequest, 0, response, GTS_RECORD_SIZE)) {
        gGtsServer.state = GTS_SERVER_STATE_GET;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

void GtsServer_RequestResult(void *response) {
    gGtsServerPending.response = response;
    GameStats_Start();
    if (GtsServer_StartRequest("http://gamestats2.gs.nintendowifi.net/pokemondpds/worldexchange/result.asp", gGtsServerRequest, 0, response, GTS_RECORD_SIZE)) {
        gGtsServer.state = GTS_SERVER_STATE_RESULT;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

void GtsServer_RequestDelete(void) {
    GameStats_Start();
    if (GtsServer_StartRequest("http://gamestats2.gs.nintendowifi.net/pokemondpds/worldexchange/delete.asp", gGtsServerRequest, 0, gGtsServerShortResponse, 2)) {
        gGtsServer.state = GTS_SERVER_STATE_DELETE;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

void GtsServer_RequestReturn(void) {
    GameStats_Start();
    if (GtsServer_StartRequest("http://gamestats2.gs.nintendowifi.net/pokemondpds/worldexchange/return.asp", gGtsServerRequest, 0, gGtsServerShortResponse, 2)) {
        gGtsServer.state = GTS_SERVER_STATE_RETURN;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

void GtsServer_RequestSearch(const void *criteria, int count, void *response) {
    gGtsServerPending.response = response;
    GameStats_Start();
    *(GtsSearchCriteria *)gGtsServerRequest = *(const GtsSearchCriteria *)criteria;
    gGtsServer.request.search.count = count;
    if (GtsServer_StartRequest("http://gamestats2.gs.nintendowifi.net/pokemondpds/worldexchange/search.asp", gGtsServerRequest, 7, response, GTS_RECORD_SIZE * count)) {
        gGtsServer.state = GTS_SERVER_STATE_SEARCH;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

void GtsServer_RequestSearchEx(const void *criteria, void *response) {
    u32 i;
    const u8 *src;
    u8 *dest;

    gGtsServerPending.response = response;
    GameStats_Start();
    src = criteria;
    dest = gGtsServerRequest;
    i = sizeof(GtsSearchCriteriaEx);
    do {
        *dest++ = *src++;
        i--;
    } while (i != 0);
    if (GtsServer_StartRequest("http://gamestats2.gs.nintendowifi.net/pokemondpds/worldexchange/search.asp", gGtsServerRequest, sizeof(GtsSearchCriteriaEx), response, ((const GtsSearchCriteriaEx *)criteria)->count * GTS_RECORD_SIZE)) {
        gGtsServer.state = GTS_SERVER_STATE_SEARCH;
        return;
    }
    gGtsServer.state = GTS_SERVER_STATE_DONE;
    gGtsServer.result = -13;
    GameStats_Shutdown();
}

void GtsServer_RequestExchange(void *arg, const void *record, void *response) {
    gGtsServerPending.response = response;
    GameStats_Start();
    memcpy(gGtsServerRequest, record, GTS_RECORD_SIZE);
    gGtsServerPending.exchangeArg = arg;
    if (GtsServer_StartRequest("http://gamestats2.gs.nintendowifi.net/pokemondpds/worldexchange/exchange.asp", gGtsServerRequest, GTS_RECORD_SIZE + 4, response, GTS_RECORD_SIZE)) {
        gGtsServer.state = GTS_SERVER_STATE_EXCHANGE;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

void GtsServer_RequestExchangeFinish(void) {
    GtsFriendKey *src = &gGtsServerFriendKey;
    GtsFriendKey *dest = (GtsFriendKey *)gGtsServerRequest;
    dest->lo = src->lo;
    dest->hi = src->hi;
    GameStats_Start();
    if (GtsServer_StartRequest("http://gamestats2.gs.nintendowifi.net/pokemondpds/worldexchange/exchange_finish.asp", gGtsServerRequest, sizeof(GtsFriendKey), gGtsServerShortResponse, 2)) {
        gGtsServer.state = GTS_SERVER_STATE_EXCHANGE_FINISH;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

void GtsServer_RequestInfo(void) {
    GameStats_Start();
    if (GtsServer_StartRequest("http://gamestats2.gs.nintendowifi.net/pokemondpds/worldexchange/info.asp", gGtsServerRequest, 0, gGtsServerShortResponse, 2)) {
        gGtsServer.state = GTS_SERVER_STATE_INFO;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

// The console's MAC address is filled in.
void GtsServer_RequestSetProfile(WifiServerProfile *profile, void *response) {
    OS_GetMacAddress(profile->macAddress);
    memcpy(gGtsServerRequest, profile, sizeof(WifiServerProfile));
    gGtsServerPending.response = response;
    GameStats_Start();
    if (GtsServer_StartRequest("http://gamestats2.gs.nintendowifi.net/pokemondpds/common/setProfile.asp", gGtsServerRequest, sizeof(WifiServerProfile), gGtsServerPending.response, 8)) {
        gGtsServer.state = GTS_SERVER_STATE_SET_PROFILE;
    } else {
        gGtsServer.state = GTS_SERVER_STATE_DONE;
        gGtsServer.result = -13;
        GameStats_Shutdown();
    }
}

static BOOL GtsServer_StartRequest(const char *url, void *payload, int payloadSize, void *response, int responseCapacity) {
    switch (GameStats_PrepareRequest((char *)url, gGtsServer.pid, payload, payloadSize, response, responseCapacity)) {
    case 0:
        return TRUE;
    case 1:
        break;
    case 2:
        break;
    }
    return FALSE;
}

// Maps a failed request's error to a result: -13 (failed), -15, -2; on a connection
// error (not -13), -14 when the wireless connection is no longer up.
static int GtsServer_GetErrorResult(int error) {
    int result;

    switch (error) {
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 9:
    case 10:
    case 14:
    case 20:
    case 21:
    case 22:
    case 23:
    case 32:
        result = -13;
        break;
    case 2:
    case 8:
    case 25:
        result = -13;
        break;
    case 11:
        result = -15;
        break;
    case 12:
    case 13:
        result = -15;
        break;
    case 16:
    case 17:
    case 18:
    case 19:
    case 26:
    case 27:
    case 28:
    case 29:
    case 31:
        result = -2;
        break;
    case 15:
    case 30:
        result = -2;
        break;
    default:
        result = -13;
        break;
    }
    if (result != -13) {
        if (WCM_GetPhase() != 9) {
            result = -14;
        }
        DWC_ClearError();
    }
    return result;
}
