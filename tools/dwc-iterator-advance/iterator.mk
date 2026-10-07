# Actual four-byte tree iterator member; only its global binding is projected.
.PHONY: to91_ov45_iterator_advance108_rebuild
to91_ov45_iterator_advance108_rebuild:

$(BUILD_DIR)/lib/NitroDWC/src/to91_ov45_iterator_advance108.o: private MWCCVER := 2.0/sp2p2
$(BUILD_DIR)/lib/NitroDWC/src/to91_ov45_iterator_advance108.o: private EXCCFLAGS := -Cpp_exceptions off
$(BUILD_DIR)/lib/NitroDWC/src/to91_ov45_iterator_advance108.o: lib/NitroDWC/src/to91_ov45_iterator_advance108.cpp lib/NitroDWC/src/tree-iterator4.h tools/dwc-iterator-advance/build.py tools/dwc-iterator-advance/shape.py tools/dwc-iterator-advance/rename.py tools/dwc-iterator-advance/producer-shape.json tools/dwc-iterator-advance/profile.json to91_ov45_iterator_advance108_rebuild
	@mkdir -p $(dir $@)
	python3 tools/dwc-iterator-advance/build.py --source $< --object $@ --game $(patsubst %.us,%,$(buildname)) --nodep "$(NODEP)" --command "$(BUILD_CPP)"
