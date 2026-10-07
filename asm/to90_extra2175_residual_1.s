	.extern ov01_021F4704
	.extern ov01_021F4728
	.extern ov01_021F54AC
	.extern ov01_021F55F4
	.extern ov01_021F5FB8
	.extern ov01_02206BD8
	.extern ov01_02206BE4
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
	.include "overlay_01_021F4704.inc"
	.include "global.inc"

.text
	thumb_func_start ov01_021F562C
ov01_021F562C: ; 0x021F562C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	str r1, [sp, #8]
	add r7, r0, #0
	ldr r0, [sp, #8]
	add r6, r2, #0
	cmp r0, #4
	bls _021F563E
	b _021F5952
_021F563E:
	add r0, r0, r0
	add r0, pc
	ldrh r0, [r0, #6]
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	add pc, r0
_021F564A: ; jump table
	.short _021F5952 - _021F564A - 2 ; case 0
	.short _021F57C4 - _021F564A - 2 ; case 1
	.short _021F58A0 - _021F564A - 2 ; case 2
	.short _021F56F2 - _021F564A - 2 ; case 3
	.short _021F5654 - _021F564A - 2 ; case 4
_021F5654:
	mov r0, #2
	mov r1, #3
	mov r2, #0
	mov r3, #1
	str r6, [sp]
	bl ov01_021F55F4
	add r0, r6, #0
	add r0, #0xad
	ldrb r0, [r0]
	cmp r0, #2
	bne _021F569E
	add r0, r6, #0
	add r0, #0xc4
	ldr r2, [r0]
	mov r0, #1
	sub r5, r7, r2
	str r0, [sp, #0x10]
	mov r0, #0
	sub r4, r5, #1
	str r0, [sp, #0xc]
	cmp r5, #0
	bge _021F5688
	sub r5, r0, #1
	add r4, r5, #0
	b _021F56D4
_021F5688:
	cmp r4, #0
	blt _021F5698
	add r0, r5, #0
	add r1, r4, #0
	bl ov01_021F4728
	cmp r0, #0
	bne _021F56D4
_021F5698:
	mov r4, #0
	mvn r4, r4
	b _021F56D4
_021F569E:
	cmp r0, #3
	bne _021F56D0
	add r0, r6, #0
	add r0, #0xc4
	ldr r2, [r0]
	mov r0, #0
	sub r5, r7, r2
	str r0, [sp, #0x10]
	mov r0, #1
	add r4, r5, #1
	str r0, [sp, #0xc]
	cmp r5, #0
	bge _021F56BE
	sub r5, r0, #2
	add r4, r5, #0
	b _021F56D4
_021F56BE:
	add r0, r5, #0
	add r1, r4, #0
	bl ov01_021F4728
	cmp r0, #0
	bne _021F56D4
	mov r4, #0
	mvn r4, r4
	b _021F56D4
_021F56D0:
	bl GF_AssertFail
_021F56D4:
	ldr r2, [sp, #0x10]
	ldr r3, [sp, #0xc]
	ldr r0, [sp, #8]
	lsl r2, r2, #0x18
	str r0, [sp]
	lsl r3, r3, #0x18
	add r0, r5, #0
	add r1, r4, #0
	lsr r2, r2, #0x18
	lsr r3, r3, #0x18
	str r6, [sp, #4]
	bl ov01_021F54AC
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
_021F56F2:
	mov r0, #1
	mov r1, #3
	mov r2, #0
	mov r3, #2
	str r6, [sp]
	bl ov01_021F55F4
	add r0, r6, #0
	add r0, #0xad
	ldrb r0, [r0]
	cmp r0, #1
	bne _021F5750
	add r0, r6, #0
	add r0, #0xc4
	ldr r2, [r0]
	sub r5, r7, #1
	sub r0, r7, r2
	sub r4, r0, #1
	mov r0, #2
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0xc]
	cmp r5, #0
	blt _021F572E
	add r0, r5, #0
	add r1, r7, #0
	bl ov01_021F4728
	cmp r0, #0
	bne _021F5734
_021F572E:
	mov r5, #0
	mvn r5, r5
	add r4, r5, #0
_021F5734:
	cmp r4, #0
	blt _021F574A
	add r2, r6, #0
	add r2, #0xc4
	ldr r2, [r2]
	add r0, r5, #0
	add r1, r4, #0
	bl ov01_021F4704
	cmp r0, #0
	bne _021F57A6
_021F574A:
	mov r4, #0
	mvn r4, r4
	b _021F57A6
_021F5750:
	cmp r0, #3
	bne _021F57A2
	add r0, r6, #0
	add r0, #0xc4
	ldr r2, [r0]
	sub r5, r7, #1
	add r0, r7, r2
	sub r4, r0, #1
	mov r0, #0
	str r0, [sp, #0x10]
	mov r0, #2
	str r0, [sp, #0xc]
	cmp r5, #0
	blt _021F5778
	add r0, r5, #0
	add r1, r7, #0
	bl ov01_021F4728
	cmp r0, #0
	bne _021F577E
_021F5778:
	mov r5, #0
	mvn r5, r5
	add r4, r5, #0
_021F577E:
	add r0, r6, #0
	add r0, #0xc4
	ldr r2, [r0]
	add r0, r6, #0
	add r0, #0xc8
	ldr r0, [r0]
	mul r0, r2
	cmp r0, r4
	ble _021F579C
	add r0, r5, #0
	add r1, r4, #0
	bl ov01_021F4704
	cmp r0, #0
	bne _021F57A6
_021F579C:
	mov r4, #0
	mvn r4, r4
	b _021F57A6
_021F57A2:
	bl GF_AssertFail
_021F57A6:
	ldr r2, [sp, #0x10]
	ldr r3, [sp, #0xc]
	ldr r0, [sp, #8]
	lsl r2, r2, #0x18
	str r0, [sp]
	lsl r3, r3, #0x18
	add r0, r5, #0
	add r1, r4, #0
	lsr r2, r2, #0x18
	lsr r3, r3, #0x18
	str r6, [sp, #4]
	bl ov01_021F54AC
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
_021F57C4:
	mov r0, #0
	mov r1, #2
	mov r2, #1
	mov r3, #3
	str r6, [sp]
	bl ov01_021F55F4
	add r0, r6, #0
	add r0, #0xad
	ldrb r0, [r0]
	cmp r0, #0
	bne _021F5832
	add r0, r6, #0
	add r0, #0xc4
	ldr r2, [r0]
	add r5, r7, #1
	sub r0, r7, r2
	add r4, r0, #1
	mov r0, #3
	str r0, [sp, #0x10]
	mov r0, #1
	str r0, [sp, #0xc]
	add r0, r6, #0
	add r0, #0xc8
	ldr r0, [r0]
	mul r0, r2
	cmp r0, r5
	ble _021F5808
	add r0, r5, #0
	add r1, r7, #0
	bl ov01_021F4728
	cmp r0, #0
	bne _021F580E
_021F5808:
	mov r5, #0
	mvn r5, r5
	add r4, r5, #0
_021F580E:
	add r0, r6, #0
	add r0, #0xc4
	ldr r2, [r0]
	add r0, r6, #0
	add r0, #0xc8
	ldr r0, [r0]
	mul r0, r2
	cmp r0, r4
	ble _021F582C
	add r0, r5, #0
	add r1, r4, #0
	bl ov01_021F4704
	cmp r0, #0
	bne _021F5882
_021F582C:
	mov r4, #0
	mvn r4, r4
	b _021F5882
_021F5832:
	cmp r0, #2
	bne _021F5882
	add r0, r6, #0
	add r0, #0xc4
	ldr r2, [r0]
	add r5, r7, #1
	add r0, r7, r2
	add r4, r0, #1
	mov r0, #1
	str r0, [sp, #0x10]
	mov r0, #3
	str r0, [sp, #0xc]
	add r0, r6, #0
	add r0, #0xc8
	ldr r0, [r0]
	mul r0, r2
	cmp r0, r5
	ble _021F5862
	add r0, r5, #0
	add r1, r7, #0
	bl ov01_021F4728
	cmp r0, #0
	bne _021F5868
_021F5862:
	mov r5, #0
	mvn r5, r5
	add r4, r5, #0
_021F5868:
	cmp r4, #0
	blt _021F587E
	add r2, r6, #0
	add r2, #0xc4
	ldr r2, [r2]
	add r0, r5, #0
	add r1, r4, #0
	bl ov01_021F4704
	cmp r0, #0
	bne _021F5882
_021F587E:
	mov r4, #0
	mvn r4, r4
_021F5882:
	ldr r2, [sp, #0x10]
	ldr r3, [sp, #0xc]
	ldr r0, [sp, #8]
	lsl r2, r2, #0x18
	str r0, [sp]
	lsl r3, r3, #0x18
	add r0, r5, #0
	add r1, r4, #0
	lsr r2, r2, #0x18
	lsr r3, r3, #0x18
	str r6, [sp, #4]
	bl ov01_021F54AC
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
_021F58A0:
	mov r0, #0
	mov r1, #1
	mov r2, #2
	mov r3, #3
	str r6, [sp]
	bl ov01_021F55F4
	add r0, r6, #0
	add r0, #0xad
	ldrb r0, [r0]
	cmp r0, #0
	bne _021F58F0
	add r0, r6, #0
	add r0, #0xc4
	ldr r2, [r0]
	mov r0, #3
	str r0, [sp, #0x10]
	mov r0, #2
	str r0, [sp, #0xc]
	add r0, r6, #0
	add r0, #0xc8
	ldr r0, [r0]
	add r5, r7, r2
	mul r0, r2
	sub r4, r5, #1
	cmp r0, r5
	bgt _021F58DE
	ldr r0, [sp, #0xc]
	sub r5, r0, #3
	add r4, r5, #0
	b _021F5934
_021F58DE:
	add r0, r5, #0
	add r1, r4, #0
	bl ov01_021F4728
	cmp r0, #0
	bne _021F5934
	mov r4, #0
	mvn r4, r4
	b _021F5934
_021F58F0:
	cmp r0, #1
	bne _021F5930
	add r0, r6, #0
	add r0, #0xc4
	ldr r2, [r0]
	mov r0, #2
	str r0, [sp, #0x10]
	mov r0, #3
	str r0, [sp, #0xc]
	add r0, r6, #0
	add r0, #0xc8
	ldr r0, [r0]
	add r5, r7, r2
	mul r0, r2
	add r4, r5, #1
	cmp r0, r5
	bgt _021F591A
	ldr r0, [sp, #0xc]
	sub r5, r0, #4
	add r4, r5, #0
	b _021F5934
_021F591A:
	cmp r0, r4
	ble _021F592A
	add r0, r5, #0
	add r1, r4, #0
	bl ov01_021F4728
	cmp r0, #0
	bne _021F5934
_021F592A:
	mov r4, #0
	mvn r4, r4
	b _021F5934
_021F5930:
	bl GF_AssertFail
_021F5934:
	ldr r2, [sp, #0x10]
	ldr r3, [sp, #0xc]
	ldr r0, [sp, #8]
	lsl r2, r2, #0x18
	str r0, [sp]
	lsl r3, r3, #0x18
	add r0, r5, #0
	add r1, r4, #0
	lsr r2, r2, #0x18
	lsr r3, r3, #0x18
	str r6, [sp, #4]
	bl ov01_021F54AC
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
_021F5952:
	bl GF_AssertFail
	add sp, #0x14
	pop {r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov01_021F562C
