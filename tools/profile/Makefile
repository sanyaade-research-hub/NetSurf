# Component settings
COMPONENT := prof
COMPONENT_VERSION := 0.0.1
# Default to a static library
COMPONENT_TYPE ?= lib-static

# Setup the tooling
include build/makefiles/Makefile.tools

# Toolchain flags
WARNFLAGS := -Wall -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes \
	-Wmissing-declarations -Wnested-externs -Werror -pedantic
ifneq ($(GCCVER),2)
  WARNFLAGS := $(WARNFLAGS) -Wextra
endif
CFLAGS := -D_BSD_SOURCE -I$(CURDIR)/include/ \
	-I$(CURDIR)/src $(WARNFLAGS) $(CFLAGS)
ifneq ($(GCCVER),2)
  CFLAGS := $(CFLAGS) -std=c99
else
  # __inline__ is a GCCism
  CFLAGS := $(CFLAGS) -Dinline="__inline__"
endif

include build/makefiles/Makefile.top

# Extra installation rules
INSTALL_ITEMS := $(INSTALL_ITEMS) /lib:$(OUTPUT)
