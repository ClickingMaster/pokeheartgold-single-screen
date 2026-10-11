#include "overlay59_result_fade_task_gap3_private.h"

void ov59_0223C16C(SysTask *task, Ov59R13G3TaskData *data) {
    Ov59R13G3Work *work = data->work;

    switch (data->state) {
    case 0:
        ov59_0223C3C0(work->scene, 2);
        PlaySE(0x923);
        data->timer = 20;
        data->state++;
        return;
    case 1:
        if (data->timer-- == 0) {
            Pokepic_StartPaletteFade(work->scene->pic, 0, 16, 4, 0x7FFF);
            ov59_0223C3C0(work->scene, 1);
            ov59_0223C3C0(work->scene, 0);
            data->timer = 25;
            data->state++;
            return;
        }
        break;
    case 2:
        if (data->timer-- == 0) {
            PlaySE(0x924);
            data->state++;
            return;
        }
        break;
    case 3:
        if (!Pokepic_ResumePaletteFade(work->scene->pic) && !ov59_0223C3DC(work->scene)) {
            ParticleSystem_DeleteAllEmitters(work->scene->particle);
            data->state++;
            return;
        }
        break;
    case 4:
        BeginNormalPaletteFade((enum FadeMode)0, (enum FadeType)1, (enum FadeType)1, 0x7FFF, 6, 6, work->heap);
        Pokepic_StartPaletteFade(work->scene->pic, 16, 0, 4, 0x7FFF);
        data->state++;
        return;
    default:
        if (IsPaletteFadeFinished() && !Pokepic_ResumePaletteFade(work->scene->pic)) {
            work->activeCount--;
            Heap_Free(data);
            SysTask_Destroy(task);
        }
        break;
    }
}
