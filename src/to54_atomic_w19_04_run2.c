#include "to54_atomic_w19_private.h"

int ov112_021EC99C(Ov112State *p) {
    FillBgTilemapRect(P(p, 0x18), 1, 0, 0, 0x10, 0x20, 8, 0x10);
    ov112_021E7CA4(p, 5, 0x11);
    ov112_021E7CA4(p, 2, 0x17);
    H(p, 0x1ec76) = ov112_021EC950(p);
    ov112_021EC7D0(p);
    ov112_021E9F40(p, 6);
    ov112_021ECA18(p, W(p, 0x1ec54));
    ov112_021EA51C(p, ov112_021FF0C4[W(p, 0x1ec50)]);
    return 10;
}

void ov112_021ECA18(Ov112State *p, int pos) {
    int pages = ov112_021EC7B8(p);
    if (H(p, 0x1ec76) > 5) {
        if (pos == 0) {
            ov112_021EA688(p, 3);
            ov112_021EA670(p, 4);
        } else if (pos > 0 && pos < pages - 1) {
            ov112_021EA670(p, 3);
            ov112_021EA670(p, 4);
        } else if (pos == pages - 1) {
            ov112_021EA670(p, 3);
            ov112_021EA688(p, 4);
        }
    } else {
        ov112_021EA688(p, 3);
        ov112_021EA688(p, 4);
    }
}

int ov112_021ECA88(Ov112State *p) {
    return S8(p, 0x1ec58 + W(p, 0x1ec54) * 5 + W(p, 0x1ec50));
}

int ov112_021ECAA8(Ov112State *p) {
    u32 a = ov112_021E9888(4);
    if (a != -1) {
        if (a <= 4) {
            W(p, 0x1ec50) = a;
            if (ov112_021ECA88(p) >= 0) {
                ov112_021EC440(p, a, ov112_021FF0C4[a], 11);
                PlaySE(0x5dd);
                ov112_021EC8A4(p, W(p, 0x1ec54), W(p, 0x1ec50));
                return 0x41;
            }
            ov112_021EC8A4(p, W(p, 0x1ec54), W(p, 0x1ec50));
            PlaySE(0x5dc);
            ov112_021EA51C(p, ov112_021FF0C4[W(p, 0x1ec50)]);
        } else if (a == 6) {
            ov112_021ECDA0(p, -1);
            ov112_021ECA18(p, W(p, 0x1ec54));
            ov112_021EA6A0(p, 3, 2);
        } else if (a == 7) {
            ov112_021ECDA0(p, 1);
            ov112_021ECA18(p, W(p, 0x1ec54));
            ov112_021EA6A0(p, 4, 3);
        } else {
            ov112_021EC440(p, a, ov112_021FF0C4[a], 11);
            PlaySE(0x5dd);
            return 0x41;
        }
        return 10;
    }
    if (gSystem.newKeys & 0x40) {
        if (W(p, 0x1ec50) > 0) {
            W(p, 0x1ec50)
            --;
            PlaySE(0x5dc);
        }
        ov112_021EA51C(p, ov112_021FF0C4[W(p, 0x1ec50)]);
        ov112_021EC8A4(p, W(p, 0x1ec54), W(p, 0x1ec50));
    } else if (gSystem.newKeys & 0x80) {
        if (W(p, 0x1ec50) < 5) {
            W(p, 0x1ec50)
            ++;
            PlaySE(0x5dc);
        }
        ov112_021EA51C(p, ov112_021FF0C4[W(p, 0x1ec50)]);
        ov112_021EC8A4(p, W(p, 0x1ec54), W(p, 0x1ec50));
    } else if (gSystem.newKeys & 0x20) {
        ov112_021ECDA0(p, -1);
        ov112_021ECA18(p, W(p, 0x1ec54));
        ov112_021EA6A0(p, 3, 2);
    } else if (gSystem.newKeys & 0x10) {
        ov112_021ECDA0(p, 1);
        ov112_021ECA18(p, W(p, 0x1ec54));
        ov112_021EA6A0(p, 4, 3);
    } else if (gSystem.newKeys & 1) {
        if (W(p, 0x1ec50) > 4) {
            ov112_021EC440(p, W(p, 0x1ec50), ov112_021FF0C4[W(p, 0x1ec50)], 11);
            PlaySE(0x5dd);
            return 0x41;
        }
        if (ov112_021ECA88(p) >= 0) {
            ov112_021EC440(p, W(p, 0x1ec50), ov112_021FF0C4[W(p, 0x1ec50)], 11);
            PlaySE(0x5dd);
            return 0x41;
        }
        PlaySE(0x5dc);
    } else if (gSystem.newKeys & 2) {
        ov112_021EC440(p, 5, 3, 11);
        PlaySE(0x5dc);
        return 0x41;
    }
    return 10;
}

int ov112_021ECCC4(Ov112State *p) {
    ov112_021EA570(p);
    switch (W(p, 0x1ec50)) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        ov112_021EA51C(p, ov112_021FF0C4[W(p, 0x1ec50)]);
        if (ov112_021ECA88(p) < 0) {
            break;
        }
        ov112_021EA570(p);
        FillBgTilemapRect(P(p, 0x18), 1, 0, 0, 0x10, 0x20, 8, 0x10);
        W(p, 0x1d798) = ov112_021ECA88(p);
        return 12;
    case 5:
        ov112_021E9C10(p, 2);
        ov112_021E9C10(p, 3);
        ov112_021E9A78(p, 1);
        ov112_021E9A78(p, 0);
        H(p, 0x1d77c) = 0;
        ov112_021EA570(p);
        FillBgTilemapRect(P(p, 0x18), 1, 0, 0, 0x10, 0x20, 8, 0x10);
        return 0;
    default:
        GF_ASSERT(W(p, 0x1ec50) < 6);
        break;
    }
    return 10;
}

void ov112_021ECDA0(Ov112State *p, int dir) {
    if (dir < 0) {
        if (W(p, 0x1ec50) != 5 && (s8)W(p, 0x1ec54) > 0) {
            W(p, 0x1ec54)
            --;
            W(p, 0x1ec50) = 0;
            PlaySE(0x5dc);
        }
    } else {
        if (W(p, 0x1ec50) != 5) {
            int pos = (s8)W(p, 0x1ec54);
            if (pos < ov112_021EC7B8(p) - 1) {
                W(p, 0x1ec54)
                ++;
                W(p, 0x1ec50) = 0;
                PlaySE(0x5dc);
            }
        }
    }
    ov112_021EA51C(p, ov112_021FF0C4[W(p, 0x1ec50)]);
    ov112_021EC7D0(p);
}

int ov112_021ECE28(Ov112State *p) {
    FillBgTilemapRect(P(p, 0x18), 1, 0, 0, 0, 0x20, 0x18, 0x10);
    FillBgTilemapRect(P(p, 0x18), 2, 0, 0, 0, 0x20, 0x18, 0x10);
    BgCommitTilemapBufferToVram(P(p, 0x18), 2);
    ov112_021EA688(p, 3);
    ov112_021EA688(p, 4);
    ov112_021E9C10(p, 2);
    ov112_021E9C10(p, 3);
    ov112_021E9A78(p, 1);
    ov112_021E9A78(p, 0);
    BufferPokewalkerCourseName(P(p, 0x1e448), 0, (u8)W(p, 0x1d798));
    P(p, 0x1e524) = ov112_021EA08C(p, 2, 0x15);
    return 13;
}
