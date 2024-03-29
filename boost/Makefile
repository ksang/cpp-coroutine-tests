VERBOSE ?= 0
DEBUG ?= 0
BOOST_ROOT ?= /usr/local/Cellar/boost/1.70.0/

CLANG := g++

LDFLAGS     := -L$(BOOST_ROOT)/lib -lboost_coroutine
CXXFLAGS	:= --std=c++11
CXXFLAGS	+= -I../include
CXXFLAGS    += -I$(BOOST_ROOT)/include

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
BIN_FILES_LIST := coro_boost
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
