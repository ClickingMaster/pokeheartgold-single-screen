#include "to52_r4_overlay81_cleanup_private.h"

void ov81_02240BB0(Work *w) {
    ov81_0224275C(w);
    {
        int i;
        for (i = 0; i < w->count; i++) {
            if (w->objects[i] != NULL) {
                w->objects[i] = ov81_02242EA4(w->objects[i]);
            }
        }
    }
    ov81_02242D74(PTR(w, 0x388));
    ov81_02242D74(PTR(w, 0x38C));
    ov81_02242D74(PTR(w, 0x390));
    ov81_02242D74(PTR(w, 0x394));
    ov81_02242D74(PTR(w, 0x398));
    ov81_02241364(w);
    ov81_02241FC0(w);
    ov81_02242170(w);
    sub_0203A914();
    PaletteData_FreeBuffers(PTR(w, 0x1A0), 2);
    PaletteData_FreeBuffers(PTR(w, 0x1A0), 0);
    PaletteData_Free(PTR(w, 0x1A0));
    PTR(w, 0x1A0) = NULL;
    ov81_02242B38((u8 *)w + 0x1C4);
    {
        int i;
        for (i = 0; i < 3; i++) {
            if (w->pics[i] != NULL) {
                Pokepic_Delete(w->pics[i]);
            }
        }
    }
    PokepicManager_Delete(w->picManager);
    DestroyMsgData(w->msgData);
    MessageFormat_Delete(w->format);
    String_Delete(w->string24);
    String_Delete(w->string28);
    {
        int i;
        for (i = 0; i < 4; i++) {
            String_Delete(w->strings[i]);
        }
    }
    FontID_Release(4);
    ov81_0224300C((u8 *)w + 0x50);
    ov81_02240458(PTR(w, 0x4C));
    NARC_Delete(PTR(w, 0x3DC));
    GF_3DVramMan_Delete(PTR(w, 0x1A4));
}
