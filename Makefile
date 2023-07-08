SUBDIRS := $(wildcard enigma_v5/.) && $(wildcard enigma2_v5/.) && $(wildcard break_enigma_v5/.)

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
