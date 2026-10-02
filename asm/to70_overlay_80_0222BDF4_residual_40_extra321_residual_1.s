	.extern FrtCmd_000
	.extern FrtCmd_001
	.extern FrtCmd_002
	.extern FrtCmd_003
	.extern FrtCmd_004
	.extern FrtCmd_005
	.extern FrtCmd_006
	.extern FrtCmd_007
	.extern FrtCmd_008
	.extern FrtCmd_009
	.extern FrtCmd_010
	.extern FrtCmd_011
	.extern FrtCmd_012
	.extern FrtCmd_013
	.extern FrtCmd_014
	.extern FrtCmd_015
	.extern FrtCmd_016
	.extern FrtCmd_017
	.extern FrtCmd_018
	.extern FrtCmd_019
	.extern FrtCmd_020
	.extern FrtCmd_021
	.extern FrtCmd_022
	.extern FrtCmd_023
	.extern FrtCmd_024
	.extern FrtCmd_025
	.extern FrtCmd_026
	.extern FrtCmd_027
	.extern FrtCmd_028
	.extern FrtCmd_029
	.extern FrtCmd_030
	.extern FrtCmd_031
	.extern FrtCmd_032
	.extern FrtCmd_033
	.extern FrtCmd_034
	.extern FrtCmd_035
	.extern FrtCmd_036
	.extern FrtCmd_037
	.extern FrtCmd_038
	.extern FrtCmd_039
	.extern FrtCmd_040
	.extern FrtCmd_041
	.extern FrtCmd_042
	.extern FrtCmd_043
	.extern FrtCmd_044
	.extern FrtCmd_045
	.extern FrtCmd_046
	.extern FrtCmd_047
	.extern FrtCmd_048
	.extern FrtCmd_049
	.extern FrtCmd_050
	.extern FrtCmd_051
	.extern FrtCmd_052
	.extern FrtCmd_053
	.extern FrtCmd_054
	.extern FrtCmd_055
	.extern FrtCmd_056
	.extern FrtCmd_057
	.extern FrtCmd_058
	.extern FrtCmd_059
	.extern FrtCmd_060
	.extern FrtCmd_061
	.extern FrtCmd_062
	.extern FrtCmd_063
	.extern FrtCmd_064
	.extern FrtCmd_065
	.extern FrtCmd_066
	.extern FrtCmd_067
	.extern FrtCmd_068
	.extern FrtCmd_069
	.extern FrtCmd_070
	.extern FrtCmd_071
	.extern FrtCmd_072
	.extern FrtCmd_073
	.extern FrtCmd_074
	.extern FrtCmd_075
	.extern FrtCmd_076
	.extern FrtCmd_077
	.extern FrtCmd_078
	.extern FrtCmd_079
	.extern FrtCmd_083
	.extern FrtCmd_084
	.extern FrtCmd_108
	.extern FrtCmd_109
	.extern FrtCmd_110
	.extern FrtCmd_111
	.extern FrtCmd_112
	.extern FrtCmd_113
	.extern FrtCmd_114
	.extern FrtCmd_115
	.extern FrtCmd_116
	.extern FrtCmd_117
	.extern FrtCmd_119
	.extern FrtCmd_120
	.extern FrtCmd_121
	.extern FrtCmd_122
	.extern FrtCmd_123
	.extern FrtCmd_124
	.extern FrtCmd_125
	.extern FrtCmd_126
	.extern FrtCmd_127
	.extern FrtCmd_128
	.extern FrtCmd_129
	.extern FrtCmd_130
	.extern FrtCmd_131
	.extern FrtCmd_150
	.extern FrtCmd_183
	.extern FrtCmd_200
	.extern FrtCmd_201
	.extern FrtCmd_203
	.extern FrtCmd_204
	.extern FrtCmd_205
	.extern FrtCmd_206
	.extern FrtCmd_207
	.extern ov80_0222D390
	.extern ov80_0222D404
	.extern ov80_0222D520
	.extern ov80_0222D968
	.extern ov80_0222DAAC
	.extern ov80_0222DC14
	.include "asm/macros.inc"
	.include "overlay_80_0222BDF4.inc"
	.include "global.inc"

.text
	thumb_func_start ov80_0222F53C
ov80_0222F53C: ; 0x0222F53C
	push {r3, r4, r5, r6, r7, lr}
	add r4, r1, #0
	mov r1, #8
	add r7, r0, #0
	ldrsh r0, [r4, r1]
	cmp r0, #0
	bgt _0222F5C2
	mov r0, #0xa
	ldrsb r0, [r4, r0]
	cmp r0, #1
	bne _0222F5A6
	ldr r2, _0222F5C8 ; =0x04000048
	mov r1, #0x3f
	ldrh r3, [r2]
	mov r0, #0xf
	mov r5, #0x20
	bic r3, r1
	orr r0, r3
	orr r0, r5
	strh r0, [r2]
	ldrh r3, [r2, #2]
	mov r0, #0x1f
	bic r3, r1
	orr r0, r3
	orr r0, r5
	strh r0, [r2, #2]
	ldrb r3, [r4, #4]
	ldrb r1, [r4, #7]
	ldrb r0, [r4, #5]
	lsl r6, r3, #8
	mov r3, #0xff
	lsl r3, r3, #8
	and r6, r3
	ldrb r3, [r4, #6]
	orr r6, r3
	add r3, r2, #0
	sub r3, #8
	strh r6, [r3]
	lsl r3, r0, #8
	mov r0, #0xff
	lsl r0, r0, #8
	and r0, r3
	orr r1, r0
	sub r0, r2, #4
	strh r1, [r0]
	lsl r2, r5, #0x15
	ldr r1, [r2]
	ldr r0, _0222F5CC ; =0xFFFF1FFF
	and r1, r0
	lsl r0, r5, #8
	orr r0, r1
	str r0, [r2]
	b _0222F5B0
_0222F5A6:
	lsl r2, r1, #0x17
	ldr r1, [r2]
	ldr r0, _0222F5CC ; =0xFFFF1FFF
	and r0, r1
	str r0, [r2]
_0222F5B0:
	add r0, r7, #0
	bl SysTask_Destroy
	add r0, r4, #0
	mov r1, #0
	mov r2, #0xc
	bl MI_CpuFill8
	pop {r3, r4, r5, r6, r7, pc}
_0222F5C2:
	sub r0, r0, #1
	strh r0, [r4, #8]
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_0222F5C8: .word 0x04000048
_0222F5CC: .word 0xFFFF1FFF
	thumb_func_end ov80_0222F53C
