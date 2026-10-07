# Exact full-source producers for two physical contiguous alarm C children.
ALARM_ROUTE_TOOLS := tools/alarm-partition
ALARM_ROUTE_PYTHON := $(if $(PYTHON),$(PYTHON),python3)
ALARM_ROUTE_HELPERS := $(wildcard $(ALARM_ROUTE_TOOLS)/*.py) $(ALARM_ROUTE_TOOLS)/profile.json
ALARM_ROUTE_C_PREFIX := $(BUILD_DIR)/lib/NitroSDK/src/os/to90_os_alarm_prefix.o
ALARM_ROUTE_C_SUFFIX := $(BUILD_DIR)/lib/NitroSDK/src/os/to90_os_alarm_suffix.o
ALARM_ROUTE_ASM := $(BUILD_DIR)/lib/NitroSDK/asm/to90_os_alarm_handler.o
ALARM_ROUTE_GAME := $(if $(filter HEARTGOLD,$(GAME_VERSION)),hg,ss)
$(ALARM_ROUTE_C_PREFIX) $(ALARM_ROUTE_C_SUFFIX) $(ALARM_ROUTE_ASM): MWCCVER := 2.0/sp2p3
$(ALARM_ROUTE_C_PREFIX) $(ALARM_ROUTE_C_SUFFIX) $(ALARM_ROUTE_ASM): DEFINES = $(GLB_DEFINES)
$(ALARM_ROUTE_C_PREFIX): lib/NitroSDK/src/os/to90_os_alarm_prefix.c $(ALARM_ROUTE_HELPERS) $(WORK_DIR)/include/global.h
	@$(ALARM_ROUTE_PYTHON) $(ALARM_ROUTE_TOOLS)/build.py --role prefix --game $(ALARM_ROUTE_GAME) --source $< --object $@ --gnu-profile "$(MWCC) $(MWCFLAGS) $(DEPFLAGS)"
	@$(call fixdep,$(ALARM_ROUTE_C_PREFIX:.o=.d))
$(ALARM_ROUTE_C_SUFFIX): lib/NitroSDK/src/os/to90_os_alarm_suffix.c $(ALARM_ROUTE_HELPERS) $(WORK_DIR)/include/global.h
	@$(ALARM_ROUTE_PYTHON) $(ALARM_ROUTE_TOOLS)/build.py --role suffix --game $(ALARM_ROUTE_GAME) --source $< --object $@ --gnu-profile "$(MWCC) $(MWCFLAGS) $(DEPFLAGS)"
	@$(call fixdep,$(ALARM_ROUTE_C_SUFFIX:.o=.d))
$(ALARM_ROUTE_ASM): lib/NitroSDK/asm/to90_os_alarm_handler.s $(ALARM_ROUTE_HELPERS)
	@$(ALARM_ROUTE_PYTHON) $(ALARM_ROUTE_TOOLS)/build.py --role handler --game $(ALARM_ROUTE_GAME) --source $< --object $@ --gnu-profile "$(MWAS) $(MWASFLAGS) $(DEPFLAGS)"
	@$(call fixdep,$(ALARM_ROUTE_ASM:.o=.d))

# Retire the original mixed producer from the response, retaining its source for provenance.
ALL_OBJS := $(filter-out $(BUILD_DIR)/lib/NitroSDK/src/os/os_alarm.o,$(ALL_OBJS))
