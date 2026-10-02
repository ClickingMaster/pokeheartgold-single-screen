	.extern ov01_021EDC28
	.extern ov01_021EE7B8
	.extern ov01_021EE934
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
	.include "overlay_01_021EDAFC.inc"
	.include "global.inc"
	.extern ov01_021EE754

.text
	thumb_func_start PrintCurFloorInNewWindow
PrintCurFloorInNewWindow: ; 0x021EE664
	push {r4, r5, r6, lr}
	sub sp, #0x18
	mov r4, #0
	str r4, [sp]
	str r3, [sp, #4]
	ldr r3, [sp, #0x28]
	add r6, r0, #0
	str r3, [sp, #8]
	str r4, [sp, #0xc]
	add r3, r4, #0
	str r4, [sp, #0x10]
	bl ov01_021EDC28
	add r5, r0, #0
	add r0, r4, #0
	add r1, r0, #0
	bl GetFontAttribute
	lsl r1, r0, #3
	mov r0, #7
	tst r0, r1
	bne _021EE694
	lsr r4, r1, #3
	b _021EE698
_021EE694:
	lsr r0, r1, #3
	add r4, r0, #1
_021EE698:
	add r0, r5, #0
	add r0, #0x99
	ldrb r0, [r0]
	add r3, r5, #0
	add r1, r5, #0
	str r0, [sp]
	lsl r0, r4, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #0xd
	str r0, [sp, #0xc]
	mov r0, #0xdd
	str r0, [sp, #0x10]
	ldr r0, [r5]
	add r3, #0x98
	ldrb r3, [r3]
	ldr r0, [r0, #8]
	add r1, #8
	mov r2, #3
	bl AddWindowParameterized
	mov r0, #0
	str r0, [sp]
	mov r0, #4
	str r0, [sp, #4]
	ldr r0, [r5]
	ldr r2, _021EE74C ; =0x000003D9
	ldr r0, [r0, #8]
	mov r1, #3
	mov r3, #0xb
	bl LoadUserFrameGfx1
	add r0, r5, #0
	ldr r2, _021EE74C ; =0x000003D9
	add r0, #8
	mov r1, #1
	mov r3, #0xb
	bl DrawFrameAndWindow1
	lsl r0, r4, #0x13
	lsr r0, r0, #0x10
	str r0, [sp]
	mov r0, #0x20
	str r0, [sp, #4]
	add r0, r5, #0
	mov r2, #0
	add r0, #8
	mov r1, #0xf
	add r3, r2, #0
	bl FillWindowPixelRect
	mov r2, #0
	add r0, r5, #0
	mov r1, #0x10
	add r3, r2, #0
	bl ov01_021EE754
	ldr r0, [r6, #0x20]
	add r1, sp, #0x18
	ldrh r1, [r1, #0x14]
	ldr r0, [r0]
	add r2, sp, #0x14
	bl ov01_021EE934
	add r1, r0, #0
	add r2, sp, #0x14
	lsl r1, r1, #0x10
	ldrb r2, [r2]
	add r0, r5, #0
	lsr r1, r1, #0x10
	mov r3, #0x10
	bl ov01_021EE754
	add r0, r5, #0
	add r1, r5, #0
	add r0, #8
	add r1, #0xb0
	str r0, [r1]
	bl CopyWindowToVram
	ldr r0, _021EE750 ; =ov01_021EE7B8
	add r1, r5, #0
	mov r2, #0
	bl SysTask_CreateOnMainQueue
	str r0, [r5, #4]
	add sp, #0x18
	pop {r4, r5, r6, pc}
	.balign 4, 0
_021EE74C: .word 0x000003D9
_021EE750: .word ov01_021EE7B8
	thumb_func_end PrintCurFloorInNewWindow
