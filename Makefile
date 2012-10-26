SRC := voyageur.c
LIBS := vector.c graph.c

SRC := $(patsubst %,src/%,$(SRC))
LIBS := $(patsubst %,lib/%,$(LIBS))
OBJ := $(patsubst %.c,%.o,$(SRC) $(LIBS))
DEP := ${OBJ:.o=.dep}

SRC_TEST := vector.c graph.c ftree.c
LIBS_TEST := vector.c graph.c ftree.c

SRC_TEST := $(patsubst %,tests/%,$(SRC_TEST))
LIBS_TEST := $(patsubst %,lib/%,$(LIBS_TEST))
BIN_TEST := $(patsubst %.c,%,$(SRC_TEST))
LIBS_OBJ_TEST := $(patsubst %.c,%.o,$(LIBS_TEST))
OBJ_TEST := $(patsubst %.c,%.o,$(SRC_TEST)) $(LIBS_OBJ_TEST)
DEP_TEST := ${OBJ_TEST:.o=.dep}

CC := gcc
CFLAGS := -Wall -Wextra -Wunused -Wconversion -Wno-sign-conversion
CFLAGS += -Ilib -Itests

.PHONY: all build-tests tests clean mproper

all: voyageur

voyageur: $(OBJ)
	$(CC) $(CFLAGS) -lm -o $@ $(OBJ)

build-tests: $(BIN_TEST)

$(BIN_TEST): $(DEP_TEST) $(OBJ_TEST)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIBS_OBJ_TEST)

tests: build-tests
	@(echo)
	@(for test in $(BIN_TEST); do \
		echo "=> Running $$test"; \
		$$test; \
	done)

clean: 
	rm -rf $(OBJ) $(OBJ_TEST)
	rm -rf $(DEP) $(DEP_TEST)

mproper: clean
	rm -f voyageur
	for test in $(BIN_TEST); do \
		rm -f $$test; \
	done

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.dep: %.c
	./depend `dirname $*` $(CFLAGS) $*.c > $@

-include $(DEP) $(DEP_TEST)
