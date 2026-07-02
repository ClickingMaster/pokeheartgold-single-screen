#include "constants/sndseq.h"
#include "constants/moves.h"
#include "constants/std_script.h"
#include "constants/species.h"
#include "constants/sprites.h"
#include "constants/maps.h"
#include "constants/flags.h"
#include "constants/mmodel.h"
#include "constants/items.h"
#include "constants/phone_constants.h"
#include "msgdata/msg/msg_0096_D31R0201.h"
#include "msgdata/msg/msg_0066_D23R0102.h"
	.include "asm/macros.inc"
	.include "overlay_01_021E6880.inc"
	.include "global.inc"

	.text

	thumb_func_start ov01_021E7D00
ov01_021E7D00: ; 0x021E7D00
	push {r3, r4, r5, r6}
	add r5, r0, #0
	lsr r4, r1, #0x1f
	lsl r0, r1, #0x1b
	sub r0, r0, r4
	mov r6, #0x1b
	ror r0, r6
	add r1, r4, r0
	lsr r4, r2, #0x1f
	lsl r0, r2, #0x1b
	sub r0, r0, r4
	ror r0, r6
	add r2, r4, r0
	ldrh r4, [r5, #2]
	mov r3, #0
	mvn r3, r3
	lsr r4, r4, #3
	mov r0, #0
	cmp r4, #0
	ble _021E7D50
	add r4, r5, #0
_021E7D2A:
	ldrb r6, [r4, #8]
	cmp r6, r1
	bgt _021E7D44
	ldrb r6, [r4, #0xa]
	cmp r1, r6
	bgt _021E7D44
	ldrb r6, [r4, #9]
	cmp r6, r2
	bgt _021E7D44
	ldrb r6, [r4, #0xb]
	cmp r2, r6
	bgt _021E7D44
	add r3, r0, #0
_021E7D44:
	ldrh r6, [r5, #2]
	add r0, r0, #1
	add r4, #8
	lsr r6, r6, #3
	cmp r0, r6
	blt _021E7D2A
_021E7D50:
	add r0, r3, #0
	pop {r3, r4, r5, r6}
	bx lr
	.balign 4, 0
	thumb_func_end ov01_021E7D00

	thumb_func_start ov01_021E7D58
ov01_021E7D58: ; 0x021E7D58
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	ldr r0, [r5, #0xc]
	add r7, r2, #0
	add r4, r1, #0
	bl Save_VarsFlags_Get
	add r6, r0, #0
	lsl r0, r7, #3
	add r0, r4, r0
	ldrb r0, [r0, #4]
	lsl r1, r0, #2
	ldr r0, _021E7DE4 ; =ov01_022063BC
	ldrh r4, [r0, r1]
	ldr r0, [r5, #0xc]
	bl Save_LocalFieldData_Get
	bl LocalFieldData_GetCurrentPosition
	add r5, r0, #0
	ldr r0, [r5]
	cmp r0, #0x8b
	bne _021E7D9A
	add r0, r6, #0
	bl CheckDisabledCianwoodWaterfall
	cmp r0, #0
	beq _021E7D9A
	ldr r0, _021E7DE8 ; =0x0000085F
	cmp r4, r0
	bne _021E7D9A
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_021E7D9A:
	ldr r1, [r5]
	ldr r0, _021E7DEC ; =0x0000016D
	cmp r1, r0
	bne _021E7DB6
	add r0, r6, #0
	bl CheckSolvedLtSurgeGym
	cmp r0, #0
	beq _021E7DB6
	ldr r0, _021E7DF0 ; =0x00000866
	cmp r4, r0
	bne _021E7DB6
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_021E7DB6:
	add r0, r6, #0
	bl CheckBattledSnorlax
	cmp r0, #1
	bne _021E7DCA
	ldr r0, _021E7DF4 ; =0x00000865
	cmp r4, r0
	bne _021E7DCA
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_021E7DCA:
	add r0, r6, #0
	bl CheckBattledRedGyarados
	cmp r0, #0
	beq _021E7DDE
	ldr r0, _021E7DF8 ; =0x0000085D
	cmp r4, r0
	bne _021E7DDE
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_021E7DDE:
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021E7DE4: .word ov01_022063BC
_021E7DE8: .word 0x0000085F
_021E7DEC: .word 0x0000016D
_021E7DF0: .word 0x00000866
_021E7DF4: .word 0x00000865
_021E7DF8: .word 0x0000085D
	thumb_func_end ov01_021E7D58

	thumb_func_start ov01_021E7DFC
ov01_021E7DFC: ; 0x021E7DFC
	push {r3, r4, r5, r6, r7, lr}
	add r5, r0, #0
	add r4, r1, #0
	add r7, r2, #0
	bl sub_02054874
	add r6, r0, #0
	add r0, r5, #0
	add r0, #0xc4
	mov r1, #1
	ldr r2, [r0]
	mvn r1, r1
	cmp r2, r1
	bne _021E7E22
	add r0, r5, #0
	add r1, r1, #1
	add r0, #0xc4
	str r1, [r0]
	b _021E7E30
_021E7E22:
	sub r0, r1, #1
	cmp r2, r0
	bne _021E7E30
	add r0, r5, #0
	add r1, r1, #1
	add r0, #0xc4
	str r1, [r0]
_021E7E30:
	add r0, r6, #0
	add r1, r4, #0
	add r2, r7, #0
	bl ov01_021E7D00
	mov r1, #0
	add r7, r0, #0
	mvn r1, r1
	cmp r7, r1
	beq _021E7EC6
	add r0, r5, #0
	add r1, r6, #0
	add r2, r7, #0
	bl ov01_021E7D58
	cmp r0, #0
	beq _021E7EEC
	add r0, r6, #4
	lsl r4, r7, #3
	str r0, [sp]
	ldrb r0, [r0, r4]
	cmp r0, #0x10
	bhs _021E7EBC
	lsl r1, r0, #2
	ldr r0, _021E7EF0 ; =ov01_022063BC
	add r2, r5, #0
	add r2, #0xc4
	ldrh r0, [r0, r1]
	ldr r2, [r2]
	cmp r2, r0
	beq _021E7E80
	ldr r2, _021E7EF4 ; =ov01_022063BC + 2
	ldrh r1, [r2, r1]
	cmp r1, #1
	bne _021E7E7C
	bl sub_02006088
	b _021E7E80
_021E7E7C:
	bl PlaySE
_021E7E80:
	ldr r0, [sp]
	add r5, #0xc4
	ldrb r0, [r0, r4]
	lsl r1, r0, #2
	ldr r0, _021E7EF0 ; =ov01_022063BC
	ldrh r0, [r0, r1]
	str r0, [r5]
	add r5, r6, #5
	ldrb r2, [r5, r4]
	cmp r2, #3
	bhs _021E7EEC
	ldr r1, _021E7EF8 ; =ov01_02206388
	mov r0, #0
	ldrb r1, [r1, r2]
	mov r2, #0xf
	bl GF_SndHandleMoveVolume
	ldr r1, [sp]
	mov r0, #5
	ldrb r2, [r1, r4]
	lsl r1, r2, #1
	add r3, r2, r1
	ldr r1, _021E7EFC ; =ov01_0220638C
	ldrb r2, [r5, r4]
	add r1, r1, r3
	ldrb r1, [r2, r1]
	add r2, r0, #0
	bl GF_SndHandleMoveVolume
	pop {r3, r4, r5, r6, r7, pc}
_021E7EBC:
	cmp r7, #0x10
	blt _021E7EEC
	bl GF_AssertFail
	pop {r3, r4, r5, r6, r7, pc}
_021E7EC6:
	add r0, r5, #0
	add r0, #0xc4
	ldr r0, [r0]
	cmp r0, r1
	beq _021E7EEC
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, #0xa
	bl StopSE
	mov r0, #0
	mov r1, #0x80
	mov r2, #0xf
	bl GF_SndHandleMoveVolume
	mov r0, #0
	mvn r0, r0
	add r5, #0xc4
	str r0, [r5]
_021E7EEC:
	pop {r3, r4, r5, r6, r7, pc}
	nop
_021E7EF0: .word ov01_022063BC
_021E7EF4: .word ov01_022063BC + 2
_021E7EF8: .word ov01_02206388
_021E7EFC: .word ov01_0220638C
	thumb_func_end ov01_021E7DFC

	thumb_func_start ov01_021E7F00
ov01_021E7F00: ; 0x021E7F00
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r0, #0xac
	ldr r0, [r0]
	add r4, r1, #0
	cmp r0, #0
	bne _021E7F34
	ldr r0, [r5, #0x40]
	bl PlayerAvatar_GetXCoord
	add r6, r0, #0
	ldr r0, [r5, #0x40]
	bl PlayerAvatar_GetZCoord
	add r2, r0, #0
	cmp r4, #0
	beq _021E7F2C
	mov r1, #0
	add r0, r5, #0
	mvn r1, r1
	add r0, #0xc4
	str r1, [r0]
_021E7F2C:
	add r0, r5, #0
	add r1, r6, #0
	bl ov01_021E7DFC
_021E7F34:
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov01_021E7F00

	thumb_func_start ov01_021E7F38
ov01_021E7F38: ; 0x021E7F38
	push {r3, lr}
	bl FieldSystem_FacingModelIsHeadbuttTree
	cmp r0, #0
	beq _021E7F46
	ldr r0, _021E7F4C ; =std_field_headbutt
	pop {r3, pc}
_021E7F46:
	ldr r0, _021E7F50 ; =0x0000FFFF
	pop {r3, pc}
	nop
_021E7F4C: .word std_field_headbutt
_021E7F50: .word 0x0000FFFF
	thumb_func_end ov01_021E7F38

	thumb_func_start ov01_021E7F54
ov01_021E7F54: ; 0x021E7F54
	push {r3, r4, r5, lr}
	add r1, sp, #0
	add r5, r0, #0
	bl sub_0203DC64
	cmp r0, #1
	bne _021E7F7C
	ldr r0, [sp]
	bl MapObject_GetType
	cmp r0, #0
	beq _021E7F74
	cmp r0, #3
	bne _021E7F78
	mov r0, #0
	pop {r3, r4, r5, pc}
_021E7F74:
	mov r0, #1
	pop {r3, r4, r5, pc}
_021E7F78:
	mov r0, #1
	pop {r3, r4, r5, pc}
_021E7F7C:
	add r0, r5, #0
	bl Field_GetBgEvents
	add r4, r0, #0
	add r0, r5, #0
	bl Field_GetNumBgEvents
	add r2, r0, #0
	add r0, r5, #0
	add r1, r4, #0
	bl GetInteractedBackgroundEventScript
	ldr r1, _021E7FA4 ; =0x0000FFFF
	cmp r0, r1
	beq _021E7F9E
	mov r0, #2
	pop {r3, r4, r5, pc}
_021E7F9E:
	mov r0, #0
	pop {r3, r4, r5, pc}
	nop
_021E7FA4: .word 0x0000FFFF
	thumb_func_end ov01_021E7F54

	thumb_func_start FieldSystem_FacingModelIsHeadbuttTree
FieldSystem_FacingModelIsHeadbuttTree: ; 0x021E7FA8
	push {r4, lr}
	sub sp, #8
	add r1, sp, #4
	add r2, sp, #0
	add r4, r0, #0
	bl PlayerAvatar_GetFacingTileCoords
	ldr r1, [sp, #4]
	ldr r2, [sp]
	add r0, r4, #0
	bl MapCoordToMatrixIndex
	ldr r1, [r4, #0x30]
	bl GetMapModelNo
	bl MapModel_IsHeadbuttTree
	cmp r0, #0
	beq _021E7FD4
	add sp, #8
	mov r0, #1
	pop {r4, pc}
_021E7FD4:
	mov r0, #0
	add sp, #8
	pop {r4, pc}
	.balign 4, 0
	thumb_func_end FieldSystem_FacingModelIsHeadbuttTree

	.rodata

	.public ov01_02206378
ov01_02206378:
	.word ov01_021E5924, ov01_021E5BE4, ov01_021E5C24, 0xFFFFFFFF
ov01_02206388:
	.byte 0x60, 0x40, 0x20, 0x00

ov01_0220638C: ; 0x0220638C
	.byte 0x40, 0x60, 0x7F
	.byte 0x2E, 0x60, 0x7F
	.byte 0x2E, 0x60, 0x7F
	.byte 0x40, 0x60, 0x7F
	.byte 0x2E, 0x40, 0x60
	.byte 0x40, 0x60, 0x6C
	.byte 0x2E, 0x60, 0x6C
	.byte 0x2E, 0x60, 0x7F
	.byte 0x2E, 0x60, 0x7F
	.byte 0x2E, 0x60, 0x7F
	.byte 0x2E, 0x60, 0x7F
	.byte 0x2E, 0x48, 0x6C
	.byte 0x2E, 0x60, 0x7F
	.byte 0x2E, 0x60, 0x7F
	.byte 0x40, 0x60, 0x7F
	.byte 0x2E, 0x60, 0x7F

ov01_022063BC: ; 0x022063BC
	.short SEQ_SE_GS_N_SESERAGI, 0x0001
	.short SEQ_SE_GS_N_HUUSHA, 0x0000
	.short SEQ_SE_GS_N_UMIBE, 0x0000
	.short SEQ_SE_GS_N_HASHIRA, 0x0001
	.short SEQ_SE_GS_N_UZUSIO, 0x0000
	.short SEQ_SE_GS_N_TAKI, 0x0000
	.short SEQ_SE_GS_N_YOUGAN, 0x0001
	.short SEQ_SE_GS_N_KANSEI, 0x0000
	.short SEQ_SE_GS_N_KITEKI, 0x0000
	.short SEQ_SE_GS_KABIGON_IBIKI, 0x0001
	.short SEQ_SE_GS_N_MOTER, 0x0001
	.short SEQ_SE_GS_N_KANE, 0x0001
	.short SEQ_SE_GS_KYOUHUU, 0x0001
	.short SEQ_SE_GS_N_ENGINE, 0x0001
	.short SEQ_SE_GS_N_HUNSUI, 0x0000
	.short SEQ_SE_GS_DENGEKIBARIA, 0x0000
