MWCCVER        := 2.0/sp2p2
PROC           := arm946e
PROC_S         := arm5te
PROC_LD        := v5te
LCF_TEMPLATE   := ARM9-TS.lcf.template
LIBS           := -Llib -lsyscall -nostdlib
OPTFLAGS       := -O4,p

include config.mk

ALL_BUILDDIRS  := $(BUILD_DIR)/lib
include common.mk
include filesystem.mk

$(ASM_OBJS): MWASFLAGS += -DPM_ASM

$(BUILD_DIR)/asm/nitrocrypto.o:  MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/nitrocrypto_run01.o $(BUILD_DIR)/src/nitrocrypto_run02.o $(BUILD_DIR)/src/nitrocrypto_run03.o $(BUILD_DIR)/src/nitrocrypto_run04.o $(BUILD_DIR)/src/nitrocrypto_run05.o $(BUILD_DIR)/src/nitrocrypto_run06.o $(BUILD_DIR)/src/nitrocrypto_run07.o $(BUILD_DIR)/src/nitrocrypto_run08.o: MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/nitrocrypto_run01.o $(BUILD_DIR)/src/nitrocrypto_run02.o $(BUILD_DIR)/src/nitrocrypto_run03.o $(BUILD_DIR)/src/nitrocrypto_run04.o $(BUILD_DIR)/src/nitrocrypto_run05.o $(BUILD_DIR)/src/nitrocrypto_run06.o $(BUILD_DIR)/src/nitrocrypto_run07.o $(BUILD_DIR)/src/nitrocrypto_run08.o: MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/src/to55_nitrocrypto_residual01_sign.o $(BUILD_DIR)/src/to55_nitrocrypto_residual01_bignum.o $(BUILD_DIR)/src/to55_nitrocrypto_residual02.o: MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to55_nitrocrypto_residual01_sign.o $(BUILD_DIR)/src/to55_nitrocrypto_residual01_bignum.o $(BUILD_DIR)/src/to55_nitrocrypto_residual02.o: MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
$(BUILD_DIR)/lib/msl/src/*.o:    EXCCFLAGS := -Cpp_exceptions on

$(ASM_OBJS): $(WORK_DIR)/include/config.h
$(C_OBJS):   $(WORK_DIR)/include/global.h

ROM             := $(BUILD_DIR)/poke$(buildname).nds
BANNER          := $(ROM:%.nds=%.bnr)
BANNER_SPEC     := $(buildname)/banner.bsf
ICON_PNG        := $(buildname)/icon.png
HEADER_TEMPLATE := $(buildname)/rom_header_template.sbin

.PHONY: main sub dsprot libsyscall sdk sdk9 sdk7
.PRECIOUS: $(ROM)

MAKEFLAGS += --no-print-directory

all:
	$(MAKE) tools
	$(MAKE) patch_mwasmarm
	$(MAKE) $(ROM)

tidy:
	@$(MAKE) -C lib/dsprot tidy
	@$(MAKE) -C lib/syscall tidy
	@$(MAKE) -C sub tidy
	$(RM) -r build
	$(RM) -r $(PROJECT_CLEAN_TARGETS)
	$(RM) $(ROM)

clean: tidy clean-filesystem clean-tools
	@$(MAKE) -C lib/dsprot clean
	@$(MAKE) -C lib/syscall clean
	@$(MAKE) -C sub clean
	$(RM) $(foreach bn,$(SUPPORTED_ROMS),$(bn)/icon.nbf[pc])

SBIN_LZ        := $(SBIN)_LZ
.PHONY: main_lz

sdk9 sdk7: sdk
main files_for_compile: | sdk9
sub: | sdk7

main: $(SBIN) $(ELF)
main_lz: $(SBIN_LZ)
sub: ; @$(MAKE) -C sub

ROMSPEC        := rom.rsf
MAKEROM_FLAGS  := $(DEFINES)

$(ALL_GAME_OBJS): files_for_compile
$(ELF): files_for_compile dsprot libsyscall

dsprot:
	$(MAKE) -C lib/dsprot all install INSTALL_PREFIX=$(abspath $(WORK_DIR)/$(BUILD_DIR))

libsyscall: files_for_compile
	$(MAKE) -C lib/syscall all install INSTALL_PREFIX=$(abspath $(WORK_DIR)/$(BUILD_DIR)) GAME_CODE=$(GAME_CODE)

$(SBIN_LZ): $(BUILD_DIR)/component.files
	$(COMPSTATIC) -9 -c -f $<

$(BUILD_DIR)/component.files: main ;

$(HEADER_TEMPLATE): ;

$(ROM): $(ROMSPEC) filesystem main_lz sub $(BANNER)
	$(WINE) $(MAKEROM) $(MAKEROM_FLAGS) -DBUILD_DIR=$(BUILD_DIR) -DNITROFS_FILES="$(NITROFS_FILES:files/%=%)" -DTITLE_NAME="$(TITLE_NAME)" -DBNR="$(BANNER)" -DHEADER_TEMPLATE="$(HEADER_TEMPLATE)" $< $@
	$(FIXROM) $@ --secure-crc $(SECURE_CRC) --game-code $(GAME_CODE)
ifeq ($(COMPARE),1)
	$(SHA1SUM) -c $(buildname)/rom.sha1
endif

$(BANNER): $(BANNER_SPEC) $(ICON_PNG:%.png=%.nbfp) $(ICON_PNG:%.png=%.nbfc)
	$(WINE) $(MAKEBNR) $< $@

# TODO: move to NitroSDK makefile
FX_CONST_H := $(WORK_DIR)/lib/include/nitro/fx/fx_const.h
PROJECT_CLEAN_TARGETS += $(FX_CONST_H)
$(FX_CONST_H): $(MKFXCONST) $(TOOLSDIR)/gen_fx_consts/fx_const.csv
	$(MKFXCONST) $@

$(ALL_LIB_OBJS): $(FX_CONST_H)
sdk9: $(ALL_LIB_OBJS)
$(WORK_DIR)/include/global.h: $(FX_CONST_H) ;

# Convenience targets
heartgold:          ; @$(MAKE) GAME_VERSION=HEARTGOLD
soulsilver:         ; @$(MAKE) GAME_VERSION=SOULSILVER
compare_heartgold:  ; @$(MAKE) GAME_VERSION=HEARTGOLD  COMPARE=1
compare_soulsilver: ; @$(MAKE) GAME_VERSION=SOULSILVER COMPARE=1
clean_heartgold:    ; @$(MAKE) GAME_VERSION=HEARTGOLD clean
clean_soulsilver:   ; @$(MAKE) GAME_VERSION=SOULSILVER clean

compare:             compare_heartgold

.PHONY: heartgold soulsilver compare compare_heartgold compare_soulsilver clean_heartgold clean_soulsilver

# Fork progress: first refresh and verify the selected ROM, then report.
.PHONY: progress
progress:
	$(MAKE) COMPARE=1 all
	python3 tools/progress/progress.py --game $(buildname)

# to54 atomic NitroCrypto compiler override
$(BUILD_DIR)/src/to54_atomic_w17_01_nitrocrypto_residual03_run01.o $(BUILD_DIR)/src/to54_atomic_w17_02_nitrocrypto_residual03_run02.o $(BUILD_DIR)/src/to54_atomic_w17_03_nitrocrypto_residual03_run03.o $(BUILD_DIR)/src/to54_atomic_w17_04_nitrocrypto_residual03_run04.o $(BUILD_DIR)/src/to54_atomic_w17_05_nitrocrypto_residual03_run05.o $(BUILD_DIR)/src/to54_atomic_w17_06_nitrocrypto_residual03_run06.o $(BUILD_DIR)/src/to54_atomic_w17_07_nitrocrypto_residual03_run07.o $(BUILD_DIR)/src/to54_atomic_w17_08_nitrocrypto_residual03_run08.o $(BUILD_DIR)/src/to54_atomic_w17_09_nitrocrypto_residual03_run09.o $(BUILD_DIR)/src/to54_atomic_w17_10_nitrocrypto_residual03_run10.o $(BUILD_DIR)/src/to54_atomic_w17_11_nitrocrypto_residual03_run11.o $(BUILD_DIR)/src/to54_atomic_w17_12_nitrocrypto_residual03_run12.o $(BUILD_DIR)/src/to54_atomic_w17_13_nitrocrypto_residual03_run13.o $(BUILD_DIR)/src/to54_atomic_w17_14_nitrocrypto_residual03_run14.o $(BUILD_DIR)/src/to54_atomic_w17_15_nitrocrypto_residual03_run15.o $(BUILD_DIR)/src/to54_atomic_w17_16_nitrocrypto_residual03_run16.o: MWCCVER := 1.2/sp2p3
$(BUILD_DIR)/src/to54_atomic_w17_01_nitrocrypto_residual03_run01.o $(BUILD_DIR)/src/to54_atomic_w17_02_nitrocrypto_residual03_run02.o $(BUILD_DIR)/src/to54_atomic_w17_03_nitrocrypto_residual03_run03.o $(BUILD_DIR)/src/to54_atomic_w17_04_nitrocrypto_residual03_run04.o $(BUILD_DIR)/src/to54_atomic_w17_05_nitrocrypto_residual03_run05.o $(BUILD_DIR)/src/to54_atomic_w17_06_nitrocrypto_residual03_run06.o $(BUILD_DIR)/src/to54_atomic_w17_07_nitrocrypto_residual03_run07.o $(BUILD_DIR)/src/to54_atomic_w17_08_nitrocrypto_residual03_run08.o $(BUILD_DIR)/src/to54_atomic_w17_09_nitrocrypto_residual03_run09.o $(BUILD_DIR)/src/to54_atomic_w17_10_nitrocrypto_residual03_run10.o $(BUILD_DIR)/src/to54_atomic_w17_11_nitrocrypto_residual03_run11.o $(BUILD_DIR)/src/to54_atomic_w17_12_nitrocrypto_residual03_run12.o $(BUILD_DIR)/src/to54_atomic_w17_13_nitrocrypto_residual03_run13.o $(BUILD_DIR)/src/to54_atomic_w17_14_nitrocrypto_residual03_run14.o $(BUILD_DIR)/src/to54_atomic_w17_15_nitrocrypto_residual03_run15.o $(BUILD_DIR)/src/to54_atomic_w17_16_nitrocrypto_residual03_run16.o: MWCFLAGS := $(filter-out -ipa file,$(MWCFLAGS))
