.PHONY : all clean

default : all

TARGETS=examples
TARGETS+=boost
TARGETS+=fiber


all:   ${TARGETS:%=%.build}
clean: ${TARGETS:%=%.clean}

%.build:
	${MAKE} -C $* build

%.clean:
	${MAKE} -C $* clean
