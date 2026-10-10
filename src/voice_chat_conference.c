#pragma force_active on
#include "global.h"

#include "voice_chat.h"

extern int VCT_AddConferenceClient(u8 aid);

int VoiceChat_AddConferenceClients(u32 aidMask, int skipAid) {
    int i;
    if (gVoiceChat.work == NULL || gVoiceChat.work->mode != VOICE_CHAT_MODE_CONFERENCE) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        if (i != skipAid && (aidMask & (1 << i)) && gVoiceChat.work->conferenceClients[i] != 1) {
            if (VCT_AddConferenceClient((u8)i) != 0) {
                return 0;
            }
            gVoiceChat.work->conferenceClients[i] = 1;
        }
    }
    return 1;
}
