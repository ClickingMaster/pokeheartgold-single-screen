#include "global.h"

#include "msgdata/msg/msg_0738_UNION.h"

#include "union_room_talk.h"

const int gUnionRoomTalkComeBackMsgs[2] = { msg_0738_UNION_00071, msg_0738_UNION_00131 };

const int gUnionRoomTalkUntradeableEggMsgs[2] = { msg_0738_UNION_00092, msg_0738_UNION_00152 };

const int gUnionRoomActivityBusyMsgs[2] = { msg_0738_UNION_00040, msg_0738_UNION_00100 };

const int gUnionRoomTalkLeaveMsgs[2] = { msg_0738_UNION_00038, msg_0738_UNION_00098 };

const int gUnionRoomTalkAnythingElseMsgs[2] = { msg_0738_UNION_00039, msg_0738_UNION_00099 };

const int gUnionRoomTalkGreetingMsgs[2] = { msg_0738_UNION_00037, msg_0738_UNION_00097 };

const int gUnionRoomActivityTradeMsgs[2][2] = {
    { msg_0738_UNION_00068, msg_0738_UNION_00128 },
    { msg_0738_UNION_00069, msg_0738_UNION_00129 },
};

const int gUnionRoomActivityTrainerCardMsgs[2][2] = {
    { msg_0738_UNION_00044, msg_0738_UNION_00104 },
    { msg_0738_UNION_00045, msg_0738_UNION_00105 },
};

const u16 gUnionRoomGroupFirstSlot[10] = { 10, 14, 18, 22, 26, 30, 34, 38, 42, 46 };

const int gUnionRoomTalkJoinGroupMsgs[3][2] = {
    { msg_0738_UNION_00049, msg_0738_UNION_00109 },
    { msg_0738_UNION_00075, msg_0738_UNION_00135 },
    { msg_0738_UNION_00087, msg_0738_UNION_00147 },
};

const int gUnionRoomTalkJoinAcceptedMsgs[3][2] = {
    { msg_0738_UNION_00051, msg_0738_UNION_00111 },
    { msg_0738_UNION_00077, msg_0738_UNION_00137 },
    { msg_0738_UNION_00089, msg_0738_UNION_00149 },
};

const int gUnionRoomTalkJoinDeclinedMsgs[3][2] = {
    { msg_0738_UNION_00050, msg_0738_UNION_00110 },
    { msg_0738_UNION_00076, msg_0738_UNION_00136 },
    { msg_0738_UNION_00088, msg_0738_UNION_00148 },
};

const int gUnionRoomTalkRequirementMsgs[3][2] = {
    { msg_0738_UNION_00060, msg_0738_UNION_00120 },
    { msg_0738_UNION_00070, msg_0738_UNION_00130 },
    { msg_0738_UNION_00086, msg_0738_UNION_00146 },
};

const int gUnionRoomLanguageGreetings[7] = { 0, 1, 2, 3, 4, -1, 5 };

const int gUnionRoomActivityDrawingMsgs[4][2] = {
    { msg_0738_UNION_00053, msg_0738_UNION_00113 },
    { msg_0738_UNION_00054, msg_0738_UNION_00114 },
    { msg_0738_UNION_00055, msg_0738_UNION_00115 },
    { msg_0738_UNION_00056, msg_0738_UNION_00116 },
};

const int gUnionRoomActivityRecordMixMsgs[4][2] = {
    { msg_0738_UNION_00079, msg_0738_UNION_00139 },
    { msg_0738_UNION_00080, msg_0738_UNION_00140 },
    { msg_0738_UNION_00081, msg_0738_UNION_00141 },
    { msg_0738_UNION_00082, msg_0738_UNION_00142 },
};

const int gUnionRoomActivitySpinTradeMsgs[4][2] = {
    { msg_0738_UNION_00093, msg_0738_UNION_00153 },
    { msg_0738_UNION_00094, msg_0738_UNION_00154 },
    { msg_0738_UNION_00095, msg_0738_UNION_00155 },
    { msg_0738_UNION_00096, msg_0738_UNION_00156 },
};

const int gUnionRoomActivityBattleMsgs[4][2] = {
    { msg_0738_UNION_00061, msg_0738_UNION_00121 },
    { msg_0738_UNION_00062, msg_0738_UNION_00122 },
    { msg_0738_UNION_00063, msg_0738_UNION_00123 },
    { msg_0738_UNION_00064, msg_0738_UNION_00124 },
};

const int gUnionRoomTalkDeclineRequestMsgs[5][2] = {
    { msg_0738_UNION_00043, msg_0738_UNION_00103 },
    { msg_0738_UNION_00048, msg_0738_UNION_00108 },
    { msg_0738_UNION_00059, msg_0738_UNION_00119 },
    { msg_0738_UNION_00067, msg_0738_UNION_00127 },
    { msg_0738_UNION_00085, msg_0738_UNION_00145 },
};

const int gUnionRoomTalkAcceptRequestMsgs[8][2] = {
    { msg_0738_UNION_00074, msg_0738_UNION_00134 },
    { msg_0738_UNION_00042, msg_0738_UNION_00102 },
    { msg_0738_UNION_00057, msg_0738_UNION_00117 },
    { msg_0738_UNION_00065, msg_0738_UNION_00125 },
    { msg_0738_UNION_00046, msg_0738_UNION_00106 },
    { msg_0738_UNION_00072, msg_0738_UNION_00132 },
    { msg_0738_UNION_00083, msg_0738_UNION_00143 },
    { msg_0738_UNION_00083, msg_0738_UNION_00143 },
};
