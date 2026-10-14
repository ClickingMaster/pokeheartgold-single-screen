	.include "asm/macros.inc"
	.include "particle_system_data.inc"
	.include "global.inc"

	.public NNS_G3dGlb

	.rodata

	.public gParticleCameraUp
gParticleCameraUp:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00
	.public gParticleCameraTarget
gParticleCameraTarget:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.public gParticleCameraPos
gParticleCameraPos:
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00
	.public gParticleSystemAllocFuncs
gParticleSystemAllocFuncs:
	.word ParticleSystem_AllocSlot0
	.word ParticleSystem_AllocSlot1
	.word ParticleSystem_AllocSlot2
	.word ParticleSystem_AllocSlot3
	.word ParticleSystem_AllocSlot4
	.word ParticleSystem_AllocSlot5
	.word ParticleSystem_AllocSlot6
	.word ParticleSystem_AllocSlot7
	.word ParticleSystem_AllocSlot8
	.word ParticleSystem_AllocSlot9
	.word ParticleSystem_AllocSlot10
	.word ParticleSystem_AllocSlot11
	.word ParticleSystem_AllocSlot12
	.word ParticleSystem_AllocSlot13
	.word ParticleSystem_AllocSlot14
	.word ParticleSystem_AllocSlot15
	.public gListMenuCursorChar
gListMenuCursorChar:
	.short 0x011F, 0xFFFF ; ‣

	.bss

	.public gParticleSystemCallbackState
gParticleSystemCallbackState:
	.space 8

	.public gParticleSystems
gParticleSystems:
	.space 0x40

	.text
