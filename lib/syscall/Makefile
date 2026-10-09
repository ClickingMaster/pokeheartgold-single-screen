GAME_CODE      ?= arm9
MWCCVER        := 2.0/sp2p3
PROC           := arm946e
PROC_S         := arm5te
PROC_LD        := v5te
BUILD_DIR      := build/$(GAME_CODE)

include ../../common.mk

MWLDFLAGS      := -library -nodead -w off -proc $(PROC_LD)
LIB            := $(BUILD_DIR)/libsyscall.a

.PHONY: all install

INSTALL_PREFIX ?= $(WORK_DIR)/lib/ARM9-TS
INSTALL_LIBDIR := $(INSTALL_PREFIX)/lib

all:
	$(MAKE) patch_mwasmarm
	$(MAKE) $(LIB)

# The secure area is one asm function per game (src/_secure_<GAME_CODE>.c); its SVC stub
# entry points are added by the alias-map hook in common.mk.
C_SRCS      := src/_secure_$(GAME_CODE).c
ASM_SRCS    :=
# Only this object goes into the archive (common.mk adds main-ELF-only objects for arm946e).
LIB_OBJS    := $(C_OBJS)

$(LIB): $(LIB_OBJS)
	cd $(BUILD_DIR) && LM_LICENSE_FILE=$(BACK_REL)/$(LM_LICENSE_FILE) $(WINE) $(MWLD) $(MWLDFLAGS) -o $(BACK_REL)/$(LIB) $(LIB_OBJS:$(BUILD_DIR)/%=%)

install: all
	$(shell mkdir -p $(INSTALL_LIBDIR))
	cp $(LIB) $(INSTALL_LIBDIR)/

tidy:
	$(RM) -r build

clean: tidy
