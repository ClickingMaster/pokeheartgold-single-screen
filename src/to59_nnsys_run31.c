typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef struct ImagePaletteProxy {
    u32 format;
    u32 extended;
    u32 address[3];
} ImagePaletteProxy;

typedef struct OamManager {
    u32 screen;
    u16 oamStart;
    u16 oamEnd;
    u16 oamCurrent;
    u16 affineStart;
    u16 affineEnd;
    u16 affineCurrent;
    u16 id;
    u16 pad;
    u32 useFastTransfer;
} OamManager;

void NNS_G2dApplyOamManagerToHW(OamManager *manager);
void NNS_G2dResetOamManagerBuffer(OamManager *manager);
void NNS_G2dApplyAndResetOamManagerBuffer(OamManager *manager);
u16 NNS_G2dGetPaletteTableValue(const u16 *table, int index);
void NNS_G2dInitImageProxy(u32 *proxy);
void NNS_G2dSetImageLocation(u32 *proxy, int type, u32 address);
u32 NNS_G2dGetImageLocation(const u32 *proxy, int type);
int NNS_G2dIsImageReadyToUse(const u32 *proxy, int type);
void NNS_G2dInitImagePaletteProxy(ImagePaletteProxy *proxy);
void NNS_G2dSetImagePaletteLocation(ImagePaletteProxy *proxy, int type, u32 address);
u32 NNS_G2dGetImagePaletteLocation(const ImagePaletteProxy *proxy, int type);
int NNS_G2dIsImagePaletteReadyToUse(const ImagePaletteProxy *proxy, int type);

void MIi_CpuClear16(u32 value, void *dest, u32 size);
extern u8 _021D8618[];
extern u8 _021D8B58[];
extern u8 _021D9098[];
extern u8 _021D8518[];
extern u8 _021D8A58[];
extern u8 _021D8F98[];
extern u8 _021D8A18[];
extern u8 _021D8F58[];
extern u8 _021D9498[];
void NNS_G2dInitOamManagerModule(void);
u16 NNS_G2dEntryOamManagerAffine(OamManager *manager, const int *matrix);

void NNS_G2dApplyAndResetOamManagerBuffer(OamManager *manager) {
    NNS_G2dApplyOamManagerToHW(manager);
    NNS_G2dResetOamManagerBuffer(manager);
}

u16 NNS_G2dGetPaletteTableValue(const u16 *table, int index) {
    return table[index];
}

void NNS_G2dInitImageProxy(u32 *proxy) {
    int i;
    for (i = 0; i < 3; i++) {
        proxy[i] = (u32)-1;
    }
}

void NNS_G2dSetImageLocation(u32 *proxy, int type, u32 address) {
    proxy[type] = address;
}

u32 NNS_G2dGetImageLocation(const u32 *proxy, int type) {
    return proxy[type];
}

int NNS_G2dIsImageReadyToUse(const u32 *proxy, int type) {
    return proxy[type] != (u32)-1;
}

void NNS_G2dInitImagePaletteProxy(ImagePaletteProxy *proxy) {
    int i;
    for (i = 0; i < 3; i++) {
        proxy->address[i] = (u32)-1;
    }
}

void NNS_G2dSetImagePaletteLocation(ImagePaletteProxy *proxy, int type, u32 address) {
    proxy->address[type] = address;
}

u32 NNS_G2dGetImagePaletteLocation(const ImagePaletteProxy *proxy, int type) {
    return proxy->address[type];
}

int NNS_G2dIsImagePaletteReadyToUse(const ImagePaletteProxy *proxy, int type) {
    return proxy->address[type] != (u32)-1;
}
