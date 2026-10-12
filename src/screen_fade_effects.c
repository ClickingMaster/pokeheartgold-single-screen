#include "screen_fade_effects.h"

#include <nitro/fx/fx_trig.h>
#include <nitro/gx/g2.h>
#include <string.h>

#include "screen_wipe_effects.h"

// The fade effects (FadeFunc_NN runs enum FadeType NN), the fixed-point helpers of the window wipes, the master
// brightness fade, and the scanline buffers and line masks the shaped wipes draw into (ARM9 static
// 0x0201010C-0x0201164C).

typedef struct BrightnessTransition {
    int stepsLeft;
    int framesPerStep;
    int frameCounter;
    int brightness; // in 1/128 units
    int target;
    int delta;
    int screen;
} BrightnessTransition;

BOOL BrightnessTransition_Step(BrightnessTransition *transition);

// The configurations of the fade functions below. MWCC lays out a TU's data by size, ordering each size class
// by the position of the definitions; the order here reproduces the original layout.
static const LineShutterRange sFadeFunc04ShutterRanges[] = {
    { 192, 0, 1 },
};

static const LineShutterRange sFadeFunc05ShutterRanges[] = {
    { 192, 0, 0 },
};

static const LineShutterRange sFadeFunc03ShutterRanges[] = {
    { 0, 192, 0 },
};

static const LineShutterRange sFadeFunc02ShutterRanges[] = {
    { 0, 192, 1 },
};

static const LineShutterRange sFadeFunc09ShutterRanges[] = {
    { 96, 0,   0 },
    { 96, 192, 0 },
};

static const SectorWipeConfig sFadeFunc35Sector = { 0, 0x1FFF, 0x20, 0x3F, 0 };

static const LineShutterRange sFadeFunc40ShutterRanges[] = {
    { 0,   94, 1 },
    { 192, 98, 1 },
};

static const LineShutterRange sFadeFunc11ShutterRanges[] = {
    { 0,   96, 0 },
    { 192, 96, 0 },
};

static const SectorWipeConfig sFadeFunc33Sector = { 0, 0x7F49, 0x3F, 0x20, 0 };

static const AngleWipeConfig sFadeFunc27Slope = { 0, 0x3FFF, 0, 0x3F, 0x20, 0 };

static const AngleWipeConfig sFadeFunc20Wedge = { 0x3FFF, 0, 0, 0x3F, 0x20, 1 };

static const LineShutterRange sFadeFunc10ShutterRanges[] = {
    { 96, 0,   1 },
    { 96, 192, 1 },
};

static const AngleWipeConfig sFadeFunc26Slope = { 0x3FFF, 0, 0, 0x3F, 0x20, 1 };

static const LineShutterRange sFadeFunc08ShutterRanges[] = {
    { 0,   96, 1 },
    { 192, 96, 1 },
};

static const AngleWipeConfig sFadeFunc21Wedge = { 0, 0x3FFF, 0, 0x3F, 0x20, 0 };

static const SectorWipeConfig sFadeFunc34Sector = { 0, 0x1FFF, 0x3F, 0x20, 1 };

static const LineShutterRange sFadeFunc41ShutterRanges[] = {
    { 94, 0,   0 },
    { 98, 192, 0 },
};

static const SectorWipeConfig sFadeFunc32Sector = { 0, 0x7F49, 0x20, 0x3F, 1 };

static const RectWipeConfig sFadeFunc13Rect = {
    { 128, 0, 128, 192 },
    { 0,   0, 255, 192 },
    0,
    0x3F,
    0x20,
    0
};

static const ClockWipeConfig sFadeFunc42Clock = { 128, 96, 0x8E38, 0, 0x20, 0x3F, 1 };

static const RectWipeConfig sFadeFunc14RightRect = {
    { 128, 0, 128, 192 },
    { 128, 0, 255, 192 },
    1,
    0x20,
    0x3F,
    1
};

static const CircleWipeConfig sFadeFunc36Circle = { 0x200, 0, 128, -80, 0, 0x3F, 0x20, 1 };

static const RectWipeConfig sFadeFunc15RightRect = {
    { 128, 0, 255, 192 },
    { 128, 0, 128, 192 },
    1,
    0x20,
    0x3F,
    0
};

static const LineShutterRange sFadeFunc30ShutterRanges[] = {
    { 0,   64,  1 },
    { 64,  128, 1 },
    { 128, 192, 1 },
};

static const CircleWipeConfig sFadeFunc17Circle = { 0, 0x100, 128, 96, 0, 0x3F, 0x20, 0 };

static const CircleWipeConfig sFadeFunc37Circle = { 0, 0x200, 128, -80, 0, 0x3F, 0x20, 0 };

static const CircleWipeConfig sFadeFunc18Circle = { 0x200, 0, 128, 288, 0, 0x3F, 0x20, 1 };

static const CircleWipeConfig sFadeFunc19Circle = { 0, 0x200, 128, 288, 0, 0x3F, 0x20, 0 };

static const LineShutterRange sFadeFunc31ShutterRanges[] = {
    { 64,  0,   0 },
    { 128, 64,  0 },
    { 192, 128, 0 },
};

static const RectWipeConfig sFadeFunc38Rect = {
    { 0, 0, 0,   192 },
    { 0, 0, 255, 192 },
    0,
    0x20,
    0x3F,
    1
};

static const RectWipeConfig sFadeFunc22Rect = {
    { 0,   0,  255, 192 },
    { 128, 96, 128, 96  },
    0,
    0x3F,
    0x20,
    1
};

static const RectWipeConfig sFadeFunc23Rect = {
    { 128, 96, 128, 96  },
    { 0,   0,  255, 192 },
    0,
    0x3F,
    0x20,
    0
};

static const RectWipeConfig sFadeFunc24Rect = {
    { 128, 96, 128, 96  },
    { 0,   0,  255, 192 },
    0,
    0x20,
    0x3F,
    1
};

static const RectWipeConfig sFadeFunc06Rect = {
    { 0, 0, 255, 192 },
    { 0, 0, 0,   192 },
    0,
    0x3F,
    0x20,
    1
};

static const RectWipeConfig sFadeFunc07Rect = {
    { 0, 0, 0,   192 },
    { 0, 0, 255, 192 },
    0,
    0x3F,
    0x20,
    0
};

static const RectWipeConfig sFadeFunc25Rect = {
    { 0,   0,  255, 192 },
    { 128, 96, 128, 96  },
    0,
    0x20,
    0x3F,
    0
};

static const RectWipeConfig sFadeFunc39Rect = {
    { 0, 0, 255, 192 },
    { 0, 0, 0,   192 },
    0,
    0x20,
    0x3F,
    0
};

static const RectWipeConfig sFadeFunc12Rect = {
    { 0,   0, 255, 192 },
    { 128, 0, 128, 192 },
    0,
    0x3F,
    0x20,
    1
};

static const RectWipeConfig sFadeFunc14LeftRect = {
    { 128, 0, 128, 192 },
    { 0,   0, 128, 192 },
    0,
    0x20,
    0x3F,
    1
};

static const RectWipeConfig sFadeFunc15LeftRect = {
    { 0,   0, 128, 192 },
    { 128, 0, 128, 192 },
    0,
    0x20,
    0x3F,
    0
};

static const CircleWipeConfig sFadeFunc16Circle = { 0x100, 0, 128, 96, 0, 0x3F, 0x20, 1 };

static const u8 sFadeFunc29RectEnds[][4] = {
    { 0, 0,   255, 48  },
    { 0, 47,  255, 96  },
    { 0, 96,  255, 144 },
    { 0, 144, 255, 192 },
};

static const u8 sFadeFunc28RectStarts[][4] = {
    { 0, 0,   255, 48  },
    { 0, 47,  255, 96  },
    { 0, 96,  255, 144 },
    { 0, 144, 255, 192 },
};

static const u8 sFadeFunc28RectEnds[][4] = {
    { 0,   0,   0,   48  },
    { 255, 47,  255, 96  },
    { 0,   96,  0,   144 },
    { 255, 144, 255, 192 },
};

static const u8 sFadeFunc29RectStarts[][4] = {
    { 255, 0,   255, 48  },
    { 0,   47,  0,   96  },
    { 255, 96,  255, 144 },
    { 0,   144, 0,   192 },
};

// The shutter configs get their ranges when the fade starts
static LineShutterConfig sFadeFunc08Shutter = { NULL, 2, 1 };

static LineShutterConfig sFadeFunc09Shutter = { NULL, 2, 0 };

static LineShutterConfig sFadeFunc05Shutter = { NULL, 1, 0 };

static LineShutterConfig sFadeFunc03Shutter = { NULL, 1, 0 };

static LineShutterConfig sFadeFunc04Shutter = { NULL, 1, 1 };

static LineShutterConfig sFadeFunc02Shutter = { NULL, 1, 1 };

static LineShutterConfig sFadeFunc10Shutter = { NULL, 2, 1 };

static LineShutterConfig sFadeFunc30Shutter = { NULL, 3, 1 };

static LineShutterConfig sFadeFunc31Shutter = { NULL, 3, 0 };

static LineShutterConfig sFadeFunc11Shutter = { NULL, 2, 0 };

static RectShutterWipeConfig sFadeFunc40RectShutter = {
    { { 0, 94, 255, 98 }, { 128, 96, 128, 96 }, 0, 0x3F, 0x20, 1 },
    { NULL, 2, 1 },
    0xB33,
};

static RectShutterWipeConfig sFadeFunc41RectShutter = {
    { { 128, 96, 128, 96 }, { 0, 94, 255, 98 }, 0, 0x3F, 0x20, 0 },
    { NULL, 2, 0 },
    0xB33,
};

BOOL FadeFunc_00(ScreenFade *fade) {
    if (fade->state == 0) {
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = TRUE;
        BrightnessTransition_Start(fade, TRUE);
        return FALSE;
    }
    return BrightnessTransition_Run(fade);
}

BOOL FadeFunc_01(ScreenFade *fade) {
    if (fade->state == 0) {
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = TRUE;
        BrightnessTransition_Start(fade, FALSE);
        return FALSE;
    }
    return BrightnessTransition_Run(fade);
}

BOOL FadeFunc_02(ScreenFade *fade) {
    if (fade->state == 0) {
        sFadeFunc02Shutter.ranges = sFadeFunc02ShutterRanges;
        SetBothBackdropColors(fade->color);
        LineShutterWipe_Begin(fade, &sFadeFunc02Shutter);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return LineShutterWipe_Update(fade);
}

BOOL FadeFunc_03(ScreenFade *fade) {
    if (fade->state == 0) {
        sFadeFunc03Shutter.ranges = sFadeFunc03ShutterRanges;
        SetBothBackdropColors(fade->color);
        LineShutterWipe_Begin(fade, &sFadeFunc03Shutter);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return LineShutterWipe_Update(fade);
}

BOOL FadeFunc_04(ScreenFade *fade) {
    if (fade->state == 0) {
        sFadeFunc04Shutter.ranges = sFadeFunc04ShutterRanges;
        SetBothBackdropColors(fade->color);
        LineShutterWipe_Begin(fade, &sFadeFunc04Shutter);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return LineShutterWipe_Update(fade);
}

BOOL FadeFunc_05(ScreenFade *fade) {
    if (fade->state == 0) {
        sFadeFunc05Shutter.ranges = sFadeFunc05ShutterRanges;
        SetBothBackdropColors(fade->color);
        LineShutterWipe_Begin(fade, &sFadeFunc05Shutter);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return LineShutterWipe_Update(fade);
}

BOOL FadeFunc_06(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        RectWipe_Begin(fade, &sFadeFunc06Rect);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return RectWipe_Update(fade);
}

BOOL FadeFunc_07(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        RectWipe_Begin(fade, &sFadeFunc07Rect);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return RectWipe_Update(fade);
}

BOOL FadeFunc_08(ScreenFade *fade) {
    if (fade->state == 0) {
        sFadeFunc08Shutter.ranges = sFadeFunc08ShutterRanges;
        SetBothBackdropColors(fade->color);
        LineShutterWipe_Begin(fade, &sFadeFunc08Shutter);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return LineShutterWipe_Update(fade);
}

BOOL FadeFunc_09(ScreenFade *fade) {
    if (fade->state == 0) {
        sFadeFunc09Shutter.ranges = sFadeFunc09ShutterRanges;
        SetBothBackdropColors(fade->color);
        LineShutterWipe_Begin(fade, &sFadeFunc09Shutter);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return LineShutterWipe_Update(fade);
}

BOOL FadeFunc_10(ScreenFade *fade) {
    if (fade->state == 0) {
        sFadeFunc10Shutter.ranges = sFadeFunc10ShutterRanges;
        SetBothBackdropColors(fade->color);
        LineShutterWipe_Begin(fade, &sFadeFunc10Shutter);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return LineShutterWipe_Update(fade);
}

BOOL FadeFunc_11(ScreenFade *fade) {
    if (fade->state == 0) {
        sFadeFunc11Shutter.ranges = sFadeFunc11ShutterRanges;
        SetBothBackdropColors(fade->color);
        LineShutterWipe_Begin(fade, &sFadeFunc11Shutter);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return LineShutterWipe_Update(fade);
}

BOOL FadeFunc_12(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        RectWipe_Begin(fade, &sFadeFunc12Rect);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return RectWipe_Update(fade);
}

BOOL FadeFunc_13(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        RectWipe_Begin(fade, &sFadeFunc13Rect);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return RectWipe_Update(fade);
}

BOOL FadeFunc_14(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        DoubleRectWipe_Begin(fade, &sFadeFunc14LeftRect, &sFadeFunc14RightRect);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return DoubleRectWipe_Update(fade);
}

BOOL FadeFunc_15(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        DoubleRectWipe_Begin(fade, &sFadeFunc15LeftRect, &sFadeFunc15RightRect);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return DoubleRectWipe_Update(fade);
}

BOOL FadeFunc_16(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        CircleWipe_Begin(fade, &sFadeFunc16Circle);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return CircleWipe_Update(fade);
}

BOOL FadeFunc_17(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        CircleWipe_Begin(fade, &sFadeFunc17Circle);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return CircleWipe_Update(fade);
}

BOOL FadeFunc_18(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        CircleWipe_Begin(fade, &sFadeFunc18Circle);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return CircleWipe_Update(fade);
}

BOOL FadeFunc_19(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        CircleWipe_Begin(fade, &sFadeFunc19Circle);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return CircleWipe_Update(fade);
}

BOOL FadeFunc_20(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        WedgeWipe_Begin(fade, &sFadeFunc20Wedge);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return WedgeWipe_Update(fade);
}

BOOL FadeFunc_21(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        WedgeWipe_Begin(fade, &sFadeFunc21Wedge);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return WedgeWipe_Update(fade);
}

BOOL FadeFunc_22(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        RectWipe_Begin(fade, &sFadeFunc22Rect);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return RectWipe_Update(fade);
}

BOOL FadeFunc_23(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        RectWipe_Begin(fade, &sFadeFunc23Rect);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return RectWipe_Update(fade);
}

BOOL FadeFunc_24(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        RectWipe_Begin(fade, &sFadeFunc24Rect);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return RectWipe_Update(fade);
}

BOOL FadeFunc_25(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        RectWipe_Begin(fade, &sFadeFunc25Rect);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return RectWipe_Update(fade);
}

BOOL FadeFunc_26(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        SlopeWipe_Begin(fade, &sFadeFunc26Slope);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return SlopeWipe_Update(fade);
}

BOOL FadeFunc_27(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        SlopeWipe_Begin(fade, &sFadeFunc27Slope);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return SlopeWipe_Update(fade);
}

BOOL FadeFunc_28(ScreenFade *fade) {
    MultiRectWipeConfig config;

    if (fade->state == 0) {
        config.starts = sFadeFunc28RectStarts;
        config.ends = sFadeFunc28RectEnds;
        config.count = 4;
        config.window = 0;
        config.insideMask = 0x3F;
        config.outsideMask = 0x20;
        config.queued = 1;
        SetBothBackdropColors(fade->color);
        MultiRectWipe_Begin(fade, &config);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return MultiRectWipe_Update(fade);
}

BOOL FadeFunc_29(ScreenFade *fade) {
    MultiRectWipeConfig config;

    if (fade->state == 0) {
        config.starts = sFadeFunc29RectStarts;
        config.ends = sFadeFunc29RectEnds;
        config.count = 4;
        config.window = 0;
        config.insideMask = 0x3F;
        config.outsideMask = 0x20;
        config.queued = 0;
        SetBothBackdropColors(fade->color);
        MultiRectWipe_Begin(fade, &config);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return MultiRectWipe_Update(fade);
}

BOOL FadeFunc_30(ScreenFade *fade) {
    if (fade->state == 0) {
        sFadeFunc30Shutter.ranges = sFadeFunc30ShutterRanges;
        SetBothBackdropColors(fade->color);
        LineShutterWipe_Begin(fade, &sFadeFunc30Shutter);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return LineShutterWipe_Update(fade);
}

BOOL FadeFunc_31(ScreenFade *fade) {
    if (fade->state == 0) {
        sFadeFunc31Shutter.ranges = sFadeFunc31ShutterRanges;
        SetBothBackdropColors(fade->color);
        LineShutterWipe_Begin(fade, &sFadeFunc31Shutter);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return LineShutterWipe_Update(fade);
}

BOOL FadeFunc_32(ScreenFade *fade) {
    SectorWipeConfig config;

    if (fade->state == 0) {
        config = sFadeFunc32Sector;
        SetBothBackdropColors(fade->color);
        SectorWipe_Begin(fade, &config);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return SectorWipe_Update(fade);
}

BOOL FadeFunc_33(ScreenFade *fade) {
    SectorWipeConfig config;

    if (fade->state == 0) {
        config = sFadeFunc33Sector;
        SetBothBackdropColors(fade->color);
        SectorWipe_Begin(fade, &config);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return SectorWipe_Update(fade);
}

BOOL FadeFunc_34(ScreenFade *fade) {
    SectorWipeConfig config;

    if (fade->state == 0) {
        config = sFadeFunc34Sector;
        SetBothBackdropColors(fade->color);
        MirroredSectorWipe_Begin(fade, &config);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return MirroredSectorWipe_Update(fade);
}

BOOL FadeFunc_35(ScreenFade *fade) {
    SectorWipeConfig config;

    if (fade->state == 0) {
        config = sFadeFunc35Sector;
        SetBothBackdropColors(fade->color);
        MirroredSectorWipe_Begin(fade, &config);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return MirroredSectorWipe_Update(fade);
}

BOOL FadeFunc_36(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        CircleWipe_Begin(fade, &sFadeFunc36Circle);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return CircleWipe_Update(fade);
}

BOOL FadeFunc_37(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        CircleWipe_Begin(fade, &sFadeFunc37Circle);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return CircleWipe_Update(fade);
}

BOOL FadeFunc_38(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        RectWipe_Begin(fade, &sFadeFunc38Rect);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return RectWipe_Update(fade);
}

BOOL FadeFunc_39(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        RectWipe_Begin(fade, &sFadeFunc39Rect);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return RectWipe_Update(fade);
}

BOOL FadeFunc_40(ScreenFade *fade) {
    if (fade->state == 0) {
        sFadeFunc40RectShutter.shutter.ranges = sFadeFunc40ShutterRanges;
        SetBothBackdropColors(fade->color);
        RectShutterWipe_Begin(fade, &sFadeFunc40RectShutter);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return RectShutterWipe_Update(fade);
}

BOOL FadeFunc_41(ScreenFade *fade) {
    if (fade->state == 0) {
        sFadeFunc41RectShutter.shutter.ranges = sFadeFunc41ShutterRanges;
        SetBothBackdropColors(fade->color);
        RectShutterWipe_Begin(fade, &sFadeFunc41RectShutter);
        fade->fadeOut = FALSE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return RectShutterWipe_Update(fade);
}

BOOL FadeFunc_42(ScreenFade *fade) {
    if (fade->state == 0) {
        SetBothBackdropColors(fade->color);
        ClockWipe_Begin(fade, &sFadeFunc42Clock);
        fade->fadeOut = TRUE;
        fade->isBrightnessFade = FALSE;
        return FALSE;
    }
    return ClockWipe_Update(fade);
}

// tan(angle), angle in 1/65536 turns
fx32 ScreenFade_Tan(int angle) {
    int index = angle >> 4;
    return FX_Div(FX_SinCosTable_[index * 2], FX_SinCosTable_[index * 2 + 1]);
}

int ScreenFade_MulTan(int angle, int value) {
    return FX_Mul(ScreenFade_Tan(angle), value << FX32_SHIFT) >> FX32_SHIFT;
}

// dest[i] = tan(angle) * i for start <= i < end
void ScreenFade_FillMulTan(int angle, s32 *dest, int end, int start) {
    int i;
    fx32 tangent = ScreenFade_Tan(angle);

    for (i = start; i < end; i++) {
        dest[i] = FX_Mul(tangent, i << FX32_SHIFT) >> FX32_SHIFT;
    }
}

// (value / 2) / tan(angle)
fx32 ScreenFade_HalfDivTan(int angle, int value) {
    return FX_Div((value / 2) << FX32_SHIFT, ScreenFade_Tan(angle));
}

// Per-step change, in 1/128 units, from start to end over `steps` steps
int ScreenFade_StepDelta(int start, int end, int steps) {
    return ((end - start) << 7) / steps;
}

int ScreenFade_AddClamped(int value, int increment) {
    value += increment;
    if (value < 0) {
        value = 0;
    }
    if (value > 255) {
        value = 255;
    }
    return value;
}

void ScreenFade_AddRect(s32 *values, const s32 *deltas) {
    values[0] += deltas[0];
    values[1] += deltas[1];
    values[2] += deltas[2];
    values[3] += deltas[3];
}

// Rectangle (left, top, right, bottom) moving from start to end: values in 1/128 pixels, ends in pixels
void ScreenFade_InitRectLerp(s32 *values, s32 *ends, s32 *deltas, const u8 *start, const u8 *end, int steps) {
    values[0] = start[0] << 7;
    values[1] = start[1] << 7;
    values[2] = start[2] << 7;
    values[3] = start[3] << 7;
    ends[0] = end[0];
    ends[1] = end[1];
    ends[2] = end[2];
    ends[3] = end[3];
    deltas[0] = ScreenFade_StepDelta(start[0], end[0], steps);
    deltas[1] = ScreenFade_StepDelta(start[1], end[1], steps);
    deltas[2] = ScreenFade_StepDelta(start[2], end[2], steps);
    deltas[3] = ScreenFade_StepDelta(start[3], end[3], steps);
}

// Fades the master brightness from neutral to the fade color (fade out) or back (fade in); the color must be
// white or black
void BrightnessTransition_Start(ScreenFade *fade, BOOL fadeOut) {
    int start;
    int end;
    BrightnessTransition *transition = Heap_Alloc(fade->heapId, sizeof(BrightnessTransition));

    fade->effect = transition;
    memset(transition, 0, sizeof(BrightnessTransition));
    transition = fade->effect;
    if (!fadeOut) {
        if (fade->color == 0x7FFF) {
            start = 16;
            end = 0;
        } else if (fade->color == 0) {
            start = -16;
            end = 0;
        } else {
            start = -16;
            end = 0;
            GF_AssertFail();
        }
    } else {
        if (fade->color == 0x7FFF) {
            start = 0;
            end = 16;
        } else if (fade->color == 0) {
            start = 0;
            end = -16;
        } else {
            start = 0;
            end = -16;
            GF_AssertFail();
        }
    }
    SetMasterBrightness(fade->screen, start);
    transition->stepsLeft = fade->steps;
    transition->framesPerStep = fade->framesPerStep;
    transition->frameCounter = 0;
    transition->brightness = start << 7;
    transition->target = end << 7;
    transition->delta = ScreenFade_StepDelta(start, end, fade->steps);
    transition->screen = fade->screen;
    fade->state++;
}

BOOL BrightnessTransition_Run(ScreenFade *fade) {
    BOOL done = FALSE;
    BrightnessTransition *transition = fade->effect;

    switch (fade->state) {
    case 1:
        if (BrightnessTransition_Step(transition) == TRUE) {
            fade->state++;
        }
        break;
    case 2:
        Heap_Free(transition);
        fade->effect = NULL;
        done = TRUE;
        fade->state++;
        break;
    case 3:
        done = TRUE;
        break;
    }
    return done;
}

#pragma force_active on

BOOL BrightnessTransition_Step(BrightnessTransition *transition) {
    BOOL done = FALSE;

    if (++transition->frameCounter >= transition->framesPerStep) {
        transition->frameCounter = 0;
        if (transition->stepsLeft - 1 > 0) {
            transition->stepsLeft--;
            transition->brightness += transition->delta;
        } else {
            transition->brightness = transition->target;
            done = TRUE;
        }
        SetMasterBrightness(transition->screen, transition->brightness / 128);
    }
    return done;
}

// Writes one scanline's span of the buffer's window (main or sub screen) during HBlank
static inline void ApplyScanlineSpan(WipeScanlineBuffer *buffer, int line, ScreenWipeScanlines *scanlines) {
    u32 screen = scanlines->screen;
    s16 right = buffer->right[line];
    s16 left = buffer->left[line];

    if (!buffer->window) {
        if (!screen) {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                reg_G2_WIN0H = ((left << 8) & 0xFF00) | (u8)right;
                reg_G2_WIN0V = 192;
            }
        } else {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                reg_G2S_DB_WIN0H = ((left << 8) & 0xFF00) | (u8)right;
                reg_G2S_DB_WIN0V = 192;
            }
        }
    } else {
        if (!screen) {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                reg_G2_WIN1H = ((left << 8) & 0xFF00) | (u8)right;
                reg_G2_WIN1V = 192;
            }
        } else {
            if (reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK) {
                reg_G2S_DB_WIN1H = ((left << 8) & 0xFF00) | (u8)right;
                reg_G2S_DB_WIN1V = 192;
            }
        }
    }
}

// HBlank callback: applies the next scanline's span of one or both buffers
void ScreenWipeScanlines_HBlankCallback(void *data) {
    ScreenWipeScanlines *scanlines = data;
    int line;

    if (scanlines == NULL) {
        GF_AssertFail();
    }
    line = reg_GX_VCOUNT;
    if (line < 192) {
        line++;
        if (line > 191) {
            line -= 192;
        }
        if (scanlines->count == 1) {
            ApplyScanlineSpan(ScreenWipeScanlines_GetBuffer(scanlines, 0), line, scanlines);
        } else {
            ApplyScanlineSpan(ScreenWipeScanlines_GetBuffer(scanlines, 0), line, scanlines);
            ApplyScanlineSpan(ScreenWipeScanlines_GetBuffer(scanlines, 1), line, scanlines);
        }
    }
}

#pragma force_active off

// window: 0 or 1 for one buffer of that window, 2 for one buffer per window
void ScreenWipeScanlines_Init(ScreenWipeScanlines *scanlines, int window, int screen, enum HeapID heapId) {
    int i;

    switch (window) {
    case 0:
    case 1:
        scanlines->buffer = Heap_Alloc(heapId, sizeof(WipeScanlineBuffer));
        scanlines->count = 1;
        scanlines->screen = screen;
        scanlines->buffer->window = window;
        break;
    case 2:
        scanlines->buffer = Heap_Alloc(heapId, sizeof(WipeScanlineBuffer) * 2);
        scanlines->count = 2;
        scanlines->screen = screen;
        for (i = 0; i < 2; i++) {
            scanlines->buffer[i].window = i;
        }
        break;
    }
}

void ScreenWipeScanlines_Free(ScreenWipeScanlines *scanlines) {
    ScreenWipeScanlines_FreeBuffer(scanlines);
}

void ScreenWipeScanlines_FreeBuffer(ScreenWipeScanlines *scanlines) {
    Heap_Free(scanlines->buffer);
    scanlines->buffer = NULL;
}

WipeScanlineBuffer *ScreenWipeScanlines_GetBuffer(ScreenWipeScanlines *scanlines, int index) {
    if (scanlines->count <= index) {
        GF_AssertFail();
    }
    return &scanlines->buffer[index];
}

// VBlank task: makes the spans built for the next frame current
void ScreenWipeScanlines_CommitTask(SysTask *task, void *data) {
    ScreenWipeScanlines *scanlines = data;
    int i;

    for (i = 0; i < scanlines->count; i++) {
        WipeScanlineBuffer *buffer = ScreenWipeScanlines_GetBuffer(scanlines, i);
        memcpy(buffer->left, buffer->nextLeft, sizeof(buffer->left) + sizeof(buffer->right));
    }
    SysTask_Destroy(task);
}

// At the end of a wipe: either window 0 shrunk to nothing with only the backdrop outside it, so the whole screen
// shows the fade color, or the windows switched off
void ScreenFade_SetEndWindows(BOOL cover, WindowRegisterQueue *regQueue, int screen) {
    if (cover == FALSE) {
        ScreenWindow_QueueSetVisible(regQueue, 0, screen);
        return;
    }
    ScreenWindow_QueueSetVisible(regQueue, 1, screen);
    ScreenWindow_QueueSetInside(regQueue, 0x3F, FALSE, 0, screen);
    ScreenWindow_QueueSetRect(regQueue, 0, 0, 0, 0, 0, screen);
    ScreenWindow_QueueSetOutside(regQueue, 0x20, FALSE, screen);
}

void ScreenFade_SetWindow(WindowRegisterQueue *regQueue, int insideMask, int outsideMask, int window, int screen, int left, int top, int right, int bottom, BOOL queued) {
    if (queued == FALSE) {
        ScreenWindow_SetInside(insideMask, FALSE, window, screen);
        ScreenWindow_SetOutside(outsideMask, FALSE, screen);
        ScreenWindow_SetRect(left, top, right, bottom, window, screen);
        return;
    }
    ScreenWindow_QueueSetInside(regQueue, insideMask, FALSE, window, screen);
    ScreenWindow_QueueSetOutside(regQueue, outsideMask, FALSE, screen);
    ScreenWindow_QueueSetRect(regQueue, left, top, right, bottom, window, screen);
}

// Swaps the planes shown inside and outside the window
void ScreenFade_SwapWindowMasks(WindowRegisterQueue *regQueue, int window, int screen, BOOL queued) {
    volatile u8 planes[4]; // outside, inside, and copies of them

    planes[1] = ScreenWindow_GetInside(window, screen);
    planes[3] = planes[1];
    planes[0] = ScreenWindow_GetOutside(screen);
    planes[2] = planes[0];
    if (queued == FALSE) {
        ScreenWindow_SetInside((u32)(planes[2] << 27) >> 27, FALSE, window, screen);
        ScreenWindow_SetOutside((u32)(planes[3] << 27) >> 27, FALSE, screen);
        return;
    }
    ScreenWindow_QueueSetInside(regQueue, (u32)(planes[2] << 27) >> 27, FALSE, window, screen);
    ScreenWindow_QueueSetOutside(regQueue, (u32)(planes[3] << 27) >> 27, FALSE, screen);
}

void ScreenFade_SetWindowVisible(WindowRegisterQueue *regQueue, u32 mask, int screen, BOOL queued) {
    if (queued == FALSE) {
        ScreenWindow_SetVisible(mask, screen);
    } else {
        ScreenWindow_QueueSetVisible(regQueue, mask, screen);
    }
}

// count 1: window0 only, otherwise both windows
void LineMask_Init(LineMask *mask, int screen, int count, int window0, int window1) {
    memset(mask, 0, sizeof(LineMask));
    if (count == 1) {
        mask->windows[0].window = window0;
        mask->count = count;
        mask->screen = screen;
        return;
    }
    mask->windows[0].window = window0;
    mask->windows[1].window = window1;
    mask->count = count;
    mask->screen = screen;
}

SysTask *LineMask_StartCommitTask(LineMask *mask) {
    return SysTask_CreateOnVWaitQueue(LineMask_CommitTask, mask, 0x3FF);
}

void LineMask_StartHBlank(void *blankPair, LineMask *mask, enum HeapID heapId) {
    FadeBlankPair_StartOnVBlank(blankPair, mask, LineMask_HBlankCallback, mask->screen, heapId);
}

void LineMask_StopHBlank(void *blankPair, LineMask *mask, enum HeapID heapId) {
    FadeBlankPair_StopOnVBlank(blankPair, mask->screen, heapId);
}

// VBlank task: makes the lines built for the next frame current (both windows, used or not)
void LineMask_CommitTask(SysTask *task, void *data) {
    LineMaskWindow *window = data;
    int i;

    for (i = 0; i < 2; i++) {
        memcpy(window->latched, window->lines, sizeof(window->lines));
        window++;
    }
    SysTask_Destroy(task);
}

#define GX_BLEND_ALL 0x3F

static inline int GX_IsHBlank(void) {
    return reg_GX_DISPSTAT & REG_GX_DISPSTAT_HBLK_MASK;
}

static inline void SetOutsidePlane(int planes, BOOL effect, int screen) {
    if (screen == 0) {
        if (GX_IsHBlank()) {
            G2_SetWndOutsidePlane(planes, effect);
        }
    } else {
        if (GX_IsHBlank()) {
            G2S_SetWndOutsidePlane(planes, effect);
        }
    }
}

static inline void SetInsidePlane(int planes, BOOL effect, int window, int screen) {
    if (window == 0) {
        if (screen == 0) {
            if (GX_IsHBlank()) {
                G2_SetWnd0InsidePlane(planes, effect);
            }
        } else {
            if (GX_IsHBlank()) {
                G2S_SetWnd0InsidePlane(planes, effect);
            }
        }
    } else {
        if (screen == 0) {
            if (GX_IsHBlank()) {
                G2_SetWnd1InsidePlane(planes, effect);
            }
        } else {
            if (GX_IsHBlank()) {
                G2S_SetWnd1InsidePlane(planes, effect);
            }
        }
    }
}

// A clear line shows only the backdrop (the fade color) inside the window and everything outside it; a set line
// the opposite
static inline void ApplyLine(LineMask *mask, int line, int index) {
    LineMaskWindow *window = &mask->windows[index];

    if (window->latched[line] == 0) {
        SetOutsidePlane(GX_BLEND_ALL, TRUE, mask->screen);
        SetInsidePlane(GX_BLEND_PLANEMASK_BD, TRUE, window->window, mask->screen);
    } else {
        SetOutsidePlane(GX_BLEND_PLANEMASK_BD, TRUE, mask->screen);
        SetInsidePlane(GX_BLEND_ALL, TRUE, window->window, mask->screen);
    }
}

// HBlank callback: applies the next scanline's line flags of one or both windows
void LineMask_HBlankCallback(void *data) {
    LineMask *mask = data;
    int line;

    if (data == NULL) {
        GF_AssertFail();
    }
    line = GX_GetVCount();
    if (line < 192) {
        line++;
        if (line > 191) {
            line -= 192;
        }
        if (mask->count == 1) {
            ApplyLine(mask, line, 0);
        } else {
            ApplyLine(mask, line, 0);
            ApplyLine(mask, line, 1);
        }
    }
}
