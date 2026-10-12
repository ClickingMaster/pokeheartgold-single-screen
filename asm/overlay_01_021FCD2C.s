#include "constants/sndseq.h"
#include "constants/moves.h"
#include "constants/std_script.h"
#include "constants/species.h"
#include "constants/sprites.h"
#include "constants/maps.h"
#include "constants/mmodel.h"
#include "constants/items.h"
#include "msgdata/msg/msg_0096_D31R0201.h"
#include "msgdata/msg/msg_0066_D23R0102.h"
	.include "asm/macros.inc"
	.public FieldCameraZoom_Start
	.public FieldCameraZoom_IsDone
	.public FieldCameraZoom_End
	.public FieldCameraZoom_Set
	.public Task_FieldCameraZoom
	.public FieldCameraZoom_Idle
	.public FieldCameraZoom_ZoomBy
	.public FieldCameraZoom_ZoomBack
	.public FieldCameraZoom_ApplyDistance
	.public FieldCameraZoom_InitZoomBy
	.public FieldCameraZoom_InitZoomBack
	.public FieldCameraZoom_Step
	.public ov01_02208E0C
	.include "overlay_01_021FCD2C.inc"
	.include "global.inc"

	.text

	thumb_func_start FieldCameraZoom_Start
FieldCameraZoom_Start: ; 0x021FCD2C
	push {r4, r5, r6, lr}
	add r6, r1, #0
	add r5, r0, #0
	add r0, r6, #0
	mov r1, #0x34
	bl Heap_AllocAtEnd
	add r4, r0, #0
	mov r1, #0
	mov r2, #0x34
	bl memset
	str r6, [r4]
	mov r0, #0
	str r0, [r4, #0xc]
	str r5, [r4, #0x2c]
	ldr r0, [r5, #0x24]
	str r0, [r4, #0x30]
	bl Camera_GetDistance
	str r0, [r4, #0x10]
	str r0, [r4, #0x20]
	ldr r0, _021FCD64 ; =Task_FieldCameraZoom
	ldr r2, _021FCD68 ; =0x0000FFFF
	add r1, r4, #0
	bl SysTask_CreateOnMainQueue
	pop {r4, r5, r6, pc}
	.balign 4, 0
_021FCD64: .word Task_FieldCameraZoom
_021FCD68: .word 0x0000FFFF
	thumb_func_end FieldCameraZoom_Start

	thumb_func_start FieldCameraZoom_IsDone
FieldCameraZoom_IsDone: ; 0x021FCD6C
	push {r3, lr}
	bl SysTask_GetData
	ldr r0, [r0, #8]
	pop {r3, pc}
	.balign 4, 0
	thumb_func_end FieldCameraZoom_IsDone

	thumb_func_start FieldCameraZoom_End
FieldCameraZoom_End: ; 0x021FCD78
	push {r4, lr}
	add r4, r0, #0
	bl SysTask_GetData
	bl Heap_Free
	add r0, r4, #0
	bl SysTask_Destroy
	pop {r4, pc}
	thumb_func_end FieldCameraZoom_End

	thumb_func_start FieldCameraZoom_Set
FieldCameraZoom_Set: ; 0x021FCD8C
	push {r4, r5, r6, lr}
	add r5, r1, #0
	add r4, r2, #0
	add r6, r3, #0
	bl SysTask_GetData
	mov r1, #0
	str r1, [r0, #4]
	str r1, [r0, #8]
	str r5, [r0, #0xc]
	str r4, [r0, #0x14]
	str r6, [r0, #0x1c]
	str r1, [r0, #0x24]
	pop {r4, r5, r6, pc}
	thumb_func_end FieldCameraZoom_Set

	thumb_func_start Task_FieldCameraZoom
Task_FieldCameraZoom: ; 0x021FCDA8
	push {r3, lr}
	add r0, r1, #0
	ldr r1, [r1, #0xc]
	lsl r2, r1, #2
	ldr r1, _021FCDB8 ; =ov01_02208E0C
	ldr r1, [r1, r2]
	blx r1
	pop {r3, pc}
	.balign 4, 0
_021FCDB8: .word ov01_02208E0C
	thumb_func_end Task_FieldCameraZoom

	thumb_func_start FieldCameraZoom_Idle
FieldCameraZoom_Idle: ; 0x021FCDBC
	mov r1, #1
	str r1, [r0, #8]
	bx lr
	.balign 4, 0
	thumb_func_end FieldCameraZoom_Idle

	thumb_func_start FieldCameraZoom_ZoomBy
FieldCameraZoom_ZoomBy: ; 0x021FCDC4
	push {r4, lr}
	add r4, r0, #0
	ldr r1, [r4, #4]
	cmp r1, #0
	beq _021FCDD4
	cmp r1, #1
	beq _021FCDDE
	pop {r4, pc}
_021FCDD4:
	bl FieldCameraZoom_InitZoomBy
	ldr r0, [r4, #4]
	add r0, r0, #1
	str r0, [r4, #4]
_021FCDDE:
	add r0, r4, #0
	bl FieldCameraZoom_Step
	cmp r0, #1
	bne _021FCDF2
	ldr r0, [r4, #4]
	add r0, r0, #1
	str r0, [r4, #4]
	mov r0, #1
	str r0, [r4, #8]
_021FCDF2:
	add r0, r4, #0
	bl FieldCameraZoom_ApplyDistance
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end FieldCameraZoom_ZoomBy

	thumb_func_start FieldCameraZoom_ZoomBack
FieldCameraZoom_ZoomBack: ; 0x021FCDFC
	push {r4, lr}
	add r4, r0, #0
	ldr r1, [r4, #4]
	cmp r1, #0
	beq _021FCE0C
	cmp r1, #1
	beq _021FCE16
	pop {r4, pc}
_021FCE0C:
	bl FieldCameraZoom_InitZoomBack
	ldr r0, [r4, #4]
	add r0, r0, #1
	str r0, [r4, #4]
_021FCE16:
	add r0, r4, #0
	bl FieldCameraZoom_Step
	cmp r0, #1
	bne _021FCE2A
	ldr r0, [r4, #4]
	add r0, r0, #1
	str r0, [r4, #4]
	mov r0, #1
	str r0, [r4, #8]
_021FCE2A:
	add r0, r4, #0
	bl FieldCameraZoom_ApplyDistance
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end FieldCameraZoom_ZoomBack

	thumb_func_start FieldCameraZoom_ApplyDistance
FieldCameraZoom_ApplyDistance: ; 0x021FCE34
	ldr r3, _021FCE40 ; =Camera_SetDistance
	add r1, r0, #0
	ldr r0, [r1, #0x20]
	ldr r1, [r1, #0x30]
	bx r3
	nop
_021FCE40: .word Camera_SetDistance
	thumb_func_end FieldCameraZoom_ApplyDistance

	thumb_func_start FieldCameraZoom_InitZoomBy
FieldCameraZoom_InitZoomBy: ; 0x021FCE44
	push {r4, lr}
	add r4, r0, #0
	ldr r0, [r4, #0x14]
	ldr r1, [r4, #0x1c]
	bl _s32_div_f
	str r0, [r4, #0x28]
	ldr r1, [r4, #0x20]
	ldr r0, [r4, #0x14]
	add r0, r1, r0
	str r0, [r4, #0x18]
	pop {r4, pc}
	thumb_func_end FieldCameraZoom_InitZoomBy

	thumb_func_start FieldCameraZoom_InitZoomBack
FieldCameraZoom_InitZoomBack: ; 0x021FCE5C
	push {r4, lr}
	add r4, r0, #0
	ldr r1, [r4, #0x10]
	ldr r0, [r4, #0x20]
	sub r0, r1, r0
	ldr r1, [r4, #0x1c]
	bl _s32_div_f
	str r0, [r4, #0x28]
	ldr r0, [r4, #0x10]
	str r0, [r4, #0x18]
	pop {r4, pc}
	thumb_func_end FieldCameraZoom_InitZoomBack

	thumb_func_start FieldCameraZoom_Step
FieldCameraZoom_Step: ; 0x021FCE74
	ldr r2, [r0, #0x20]
	ldr r1, [r0, #0x28]
	add r1, r2, r1
	str r1, [r0, #0x20]
	ldr r1, [r0, #0x24]
	add r2, r1, #1
	str r2, [r0, #0x24]
	ldr r1, [r0, #0x1c]
	cmp r2, r1
	blo _021FCE92
	str r1, [r0, #0x24]
	ldr r1, [r0, #0x18]
	str r1, [r0, #0x20]
	mov r0, #1
	bx lr
_021FCE92:
	mov r0, #0
	bx lr
	.balign 4, 0
	thumb_func_end FieldCameraZoom_Step

	.rodata

ov01_02208E0C: ; 0x02208E0C
	.word FieldCameraZoom_Idle
	.word FieldCameraZoom_ZoomBy
	.word FieldCameraZoom_ZoomBack
	.word 0x00000000
