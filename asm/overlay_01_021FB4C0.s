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
	.include "overlay_01_021FB4C0.inc"
	.include "global.inc"

	.text

	thumb_func_start HBlankSystem_New
HBlankSystem_New: ; 0x021FB4C0
	push {r4, lr}
	mov r1, #0x1c
	bl Heap_Alloc
	add r4, r0, #0
	bl HBlankSystem_Init
	add r0, r4, #0
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end HBlankSystem_New

	thumb_func_start HBlankSystem_Delete
HBlankSystem_Delete: ; 0x021FB4D4
	push {r4, lr}
	add r4, r0, #0
	beq _021FB4F0
	ldr r1, [r4]
	cmp r1, #1
	bne _021FB4E4
	bl HBlankSystem_Stop
_021FB4E4:
	add r0, r4, #0
	bl HBlankSystem_Init
	add r0, r4, #0
	bl Heap_Free
_021FB4F0:
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end HBlankSystem_Delete

	thumb_func_start HBlankSystem_Start
HBlankSystem_Start: ; 0x021FB4F4
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _021FB510 ; =HBlankSystem_Callback
	add r1, r4, #0
	bl Main_SetHBlankIntrCB
	cmp r0, #1
	beq _021FB508
	bl GF_AssertFail
_021FB508:
	mov r0, #1
	str r0, [r4]
	pop {r4, pc}
	nop
_021FB510: .word HBlankSystem_Callback
	thumb_func_end HBlankSystem_Start

	thumb_func_start HBlankSystem_Stop
HBlankSystem_Stop: ; 0x021FB514
	push {r4, lr}
	add r4, r0, #0
	mov r0, #0
	add r1, r0, #0
	bl Main_SetHBlankIntrCB
	cmp r0, #1
	beq _021FB528
	bl GF_AssertFail
_021FB528:
	mov r0, #0
	str r0, [r4]
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end HBlankSystem_Stop

	thumb_func_start HBlankSystem_StartTask
HBlankSystem_StartTask: ; 0x021FB530
	push {r4, r5, r6, lr}
	add r5, r1, #0
	add r6, r2, #0
	bl HBlankSystem_GetFreeTask
	add r4, r0, #0
	bne _021FB542
	bl GF_AssertFail
_021FB542:
	cmp r4, #0
	beq _021FB54E
	str r6, [r4, #4]
	str r5, [r4, #8]
	mov r0, #1
	str r0, [r4]
_021FB54E:
	add r0, r4, #0
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end HBlankSystem_StartTask

	thumb_func_start HBlankTask_Delete
HBlankTask_Delete: ; 0x021FB554
	ldr r3, _021FB558 ; =HBlankTask_Reset
	bx r3
	.balign 4, 0
_021FB558: .word HBlankTask_Reset
	thumb_func_end HBlankTask_Delete

	thumb_func_start HBlankSystem_Init
HBlankSystem_Init: ; 0x021FB55C
	push {r3, r4, r5, lr}
	add r3, r0, #0
	mov r2, #0x1c
	mov r1, #0
_021FB564:
	strb r1, [r3]
	add r3, r3, #1
	sub r2, r2, #1
	bne _021FB564
	mov r4, #0
	str r4, [r0]
	add r5, r0, #4
_021FB572:
	add r0, r5, #0
	bl HBlankTask_Reset
	add r4, r4, #1
	add r5, #0xc
	cmp r4, #2
	blt _021FB572
	pop {r3, r4, r5, pc}
	.balign 4, 0
	thumb_func_end HBlankSystem_Init

	thumb_func_start HBlankTask_Reset
HBlankTask_Reset: ; 0x021FB584
	mov r2, #0
	ldr r1, _021FB590 ; =HBlankTask_Dummy
	str r2, [r0]
	str r1, [r0, #8]
	str r2, [r0, #4]
	bx lr
	.balign 4, 0
_021FB590: .word HBlankTask_Dummy
	thumb_func_end HBlankTask_Reset

	thumb_func_start HBlankSystem_Callback
HBlankSystem_Callback: ; 0x021FB594
	push {r4, r5, r6, lr}
	add r5, r0, #0
	mov r6, #0
	add r4, r5, #4
_021FB59C:
	ldr r1, [r5, #8]
	ldr r2, [r5, #0xc]
	add r0, r4, #0
	blx r2
	add r6, r6, #1
	add r5, #0xc
	add r4, #0xc
	cmp r6, #2
	blt _021FB59C
	pop {r4, r5, r6, pc}
	thumb_func_end HBlankSystem_Callback

	thumb_func_start HBlankTask_Dummy
HBlankTask_Dummy: ; 0x021FB5B0
	bx lr
	.balign 4, 0
	thumb_func_end HBlankTask_Dummy

	thumb_func_start HBlankSystem_GetFreeTask
HBlankSystem_GetFreeTask: ; 0x021FB5B4
	mov r2, #0
	add r3, r0, #0
_021FB5B8:
	ldr r1, [r3, #4]
	cmp r1, #0
	bne _021FB5C8
	add r1, r0, #4
	mov r0, #0xc
	mul r0, r2
	add r0, r1, r0
	bx lr
_021FB5C8:
	add r2, r2, #1
	add r3, #0xc
	cmp r2, #2
	blt _021FB5B8
	mov r0, #0
	bx lr
	thumb_func_end HBlankSystem_GetFreeTask
