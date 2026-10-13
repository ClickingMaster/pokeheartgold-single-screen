#ifndef POKEHEARTGOLD_FIELD_WEATHER_MANAGER_H
#define POKEHEARTGOLD_FIELD_WEATHER_MANAGER_H

#include "global.h"

#include "field/field_texture_manager.h"

#include "field_fog.h"
#include "sprite.h"
#include "task.h"

enum WeatherSystemProcess {
    WEATHER_SYS_PROC_INIT = 0,
};

typedef struct WeatherSpriteResources {
    SpriteResource *charResObj[4];
    SpriteTemplate spriteTemplate;
    SpriteResourcesHeader spriteResHeader;
} WeatherSpriteResources;

typedef struct WeatherGfxNarcData {
    u32 paletteId;
    u32 charId;
    u32 screenId;
} WeatherGfxNarcData;

typedef struct WeatherDraw {
    GF_2DGfxResMan *resMan[4];
    GF_2DGfxResHeader *header;
    NNSG2dRendererInstance instance;
    NNSG2dRenderSurface surface;
    SpriteList *spriteList;
    SysTask *task;
} WeatherDraw;

typedef struct WeatherBgGfxFiles {
    void *paletteFile;
    void *charFile;
    void *screenFile;
    NNSG2dScreenData *screenData;
    NNSG2dCharacterData *charData;
    NNSG2dPaletteData *paletteData;
} WeatherBgGfxFiles;

typedef struct WeatherSystem WeatherSystem;
typedef struct WeatherEntry WeatherEntry;
typedef struct WeatherWork WeatherWork;
typedef struct WeatherObject WeatherObject;
struct WeatherObject {
    WeatherSystem *weatherSystem;
    Sprite *sprite;
    u32 *data;
    u32 dataBuffer[10];
    WeatherObject *next;
    WeatherObject *prev;
};

typedef struct WeatherManager {
    WeatherSystem *weatherSystem;
    u32 weather;
    u32 nextWeather;
    u32 state;
    u32 queuedWeather;
    SysTask *task;
} WeatherManager;

typedef struct WeatherLerp {
    u32 value;
    u32 start;
    u32 delta;
    int frame;
    int duration;
} WeatherLerp;
typedef void (*WeatherSpawnFunc)(WeatherWork *, int);
typedef struct WeatherSpawnRamp {
    WeatherWork *work;
    s16 count;
    s16 timer;
    s16 interval;
    s16 countTimer;
    s16 targetCount;
    s16 targetInterval;
    s16 intervalStep;
    s16 countStepSpawns;
    s32 countStep;
    WeatherSpawnFunc spawnFunc;
} WeatherSpawnRamp;

typedef void (*WeatherObjectFunc)(WeatherObject *);

typedef struct WeatherFogChange {
    FogData *fog;
    WeatherLerp offset;
    WeatherLerp red;
    WeatherLerp green;
    WeatherLerp blue;
    WeatherLerp slope;
} WeatherFogChange;

typedef struct WeatherFogTableFade {
    FogData *fog;
    u8 table[0x20];
    s32 stepsLeft;
    s32 frameTimer;
    s16 framesPerStep;
    s16 direction;
} WeatherFogTableFade;

typedef struct WeatherParticleTaskData {
    WeatherSpawnRamp spawnRamp;
    WeatherFogTableFade fogTableFade;
    WeatherFogChange fogChange;
    s32 data[10];
} WeatherParticleTaskData;

typedef struct WeatherFogTaskData {
    WeatherFogTableFade fogTableFade;
    WeatherFogChange fogChange;
    s32 data[10];
} WeatherFogTaskData;

typedef struct WeatherMistTaskData {
    WeatherLerp blendLerp;
    WeatherFogTableFade fogTableFade;
    WeatherFogChange fogChange;
} WeatherMistTaskData;

typedef struct UnkStruct_021ED710_Sub_Sub4 {
    fx32 unk0;
    fx32 unk4;
    fx32 unk8;
    s16 unkC;
    s16 unkE;
} UnkStruct_021ED710_Sub_Sub4;

typedef struct UnkStruct_021ED710_Sub {
    s16 x;
    s16 z;
    UnkStruct_021ED710_Sub_Sub4 unk4;
    void *unk14;
    void *unk18;
    void *unk1C[2];
    SysTask *task;
    u32 unk2C;
    u16 unk30[4][192];
} UnkStruct_021ED710_Sub;

typedef struct WeatherFlashTaskData {
    UnkStruct_021ED710_Sub unk0;
    void *darkness;
    u16 scaleFrame;
    u16 endStep;
} WeatherFlashTaskData;

void FieldTextureManager_KeepTextures(FieldTextureManager *textureManager);
WeatherManager *WeatherManager_New(FieldSystem *fieldSystem);
void WeatherManager_Delete(WeatherManager *weatherManager);
void WeatherManager_SetWeather(WeatherManager *weatherManager, int a1);
BOOL WeatherManager_ChangeWeather(WeatherManager *weatherManager, s32 weather);
u32 WeatherManager_GetWeather(WeatherManager *weatherManager);

#endif // POKEHEARTGOLD_FIELD_WEATHER_MANAGER_H
