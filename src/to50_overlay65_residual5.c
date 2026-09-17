#include "to50_overlay65_residual5_private.h"

s32 ov65_0221DEA0(PlayerProfile *profile, Ov65RecordR5 *records, Ov65PacketR5 *packet) {
    s32 i;
    CopyU16StringArray(packet->name, PlayerProfile_GetNamePtr(profile));
    packet->trainerId = PlayerProfile_GetTrainerID(profile);
    packet->language = PlayerProfile_GetLanguage(profile);
    packet->version = PlayerProfile_GetVersion(profile);
    packet->gender = PlayerProfile_GetTrainerGender(profile);
    for (i = 0; i < 16; i++, records++) {
        packet->ids[i] = records->trainerId;
        packet->versions[i] = records->version;
        packet->languages[i] = records->language;
        packet->genders[i] = records->gender;
    }
    return sub_02036FD8(0x1C, packet, sizeof(*packet));
}
