#ifndef POKEHEARTGOLD_FIELD_FOLLOW_MON_BALL_H
#define POKEHEARTGOLD_FIELD_FOLLOW_MON_BALL_H

#include "field_warp_reset.h"

// FollowMonBall_Spawn kinds
enum FollowMonBallKind {
    FOLLOW_MON_BALL_RELEASE,      // the ball bursts open and the Pokemon appears
    FOLLOW_MON_BALL_RECALL_WHITE, // the Pokemon turns white and shrinks into the ball, which stays shown
    FOLLOW_MON_BALL_BURST,        // the burst alone
    FOLLOW_MON_BALL_RECALL,       // as FOLLOW_MON_BALL_RECALL_WHITE without the white palette; the Pokemon is hidden once it has shrunk
};

FieldEffect *FollowMonBall_Spawn(LocalMapObject *obj, int kind);

#endif // POKEHEARTGOLD_FIELD_FOLLOW_MON_BALL_H
