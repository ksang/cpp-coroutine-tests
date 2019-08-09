VERBOSE ?= 0
DEBUG ?= 0

CLANG := clang++

LDFLAGS     :=
CXXFLAGS	:= --std=c++17 -fcoroutines-ts -stdlib=libc++
CXXFLAGS	+= -I../include

ifeq ($(DEBUG), 0)
CXXFLAGS  += -O3 -g
else
CXXFLAGS  += -O0 -g -ggdb3
endif

ifneq ($(VERBOSE), 0)
CXXFLAGS += -Xcompiler -Wall,-Wextra,-Wno-unused-parameter
else
.SILENT:
endif

.PHONY: build clean

BUILDDIR ?= ../build

DST_DIR := $(BUILDDIR)
SRC_FILES := $(wildcard *.cc)
OBJ_FILES := $(SRC_FILES:%.cc=${DST_DIR}/%.o)
BIN_FILES_LIST := generator
BIN_FILES_LIST += async_count
BIN_FILES := $(BIN_FILES_LIST:%=${DST_DIR}/%)

build: ${BIN_FILES}

clean:
	rm -rf ${DST_DIR}

${DST_DIR}/%.o: %.cc
	@printf "Compiling  %-35s > %s\n" $< $@
	@mkdir -p ${DST_DIR}
	$(CLANG) -o $@ $(CXXFLAGS) -c $<

${DST_DIR}/%:${DST_DIR}/%.o
	@printf "Linking    %-35s > %s\n" $< $@
	@mkdir -p ${DST_DIR}
	$(CLANG) -o $@ $(LDFLAGS) $^
