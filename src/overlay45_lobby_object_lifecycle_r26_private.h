#ifndef POKEHEARTGOLD_OVERLAY45_LOBBY_OBJECT_LIFECYCLE_R26_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY45_LOBBY_OBJECT_LIFECYCLE_R26_PRIVATE_H
extern const u16 ov45_02254A84[][2];
void *Heap_Alloc(u32, u32);
void Heap_Free(void *);
void *memset(void *, int, u32);
void ov45_0222CDE4(void *, int, u32);
void ov45_0222CE0C(void *, int);
int ov45_0222CE2C(void *, int);
int OverlayManager_Run(void *);
void OverlayManager_Delete(void *);
int ov45_0222CE54(void *, u8);
int ov45_0222CCE4(u16);
u16 ov45_0222CD04(u32);
struct Obj;
struct Obj *WiFiPlazaApps_New(int, u32, u32, void *, u32);
void WiFiPlazaApps_Delete(void *);
int WiFiPlazaApps_StartFirst(void *);
int WiFiPlazaApps_Run(void *);

#endif // POKEHEARTGOLD_OVERLAY45_LOBBY_OBJECT_LIFECYCLE_R26_PRIVATE_H
