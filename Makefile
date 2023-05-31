SOURCEDIR    = src
TESTSDIR     = tests
BUILDDIR     = build
BUILDOBJDIR  = build/objects
BUILDTESTDIR = build/tests
BINARY       = build/sc

CC         = gcc -std=c11
CFLAGS     = -Wall -g
CTESTFLAGS = -Wall -g

ENTRYPOINT := $(SOURCEDIR)/main.c
SOURCES    := $(shell find $(SOURCEDIR) -name '*.c' -not -name 'main.c')
OBJECTS    := $(patsubst $(SOURCEDIR)/%.c, $(BUILDOBJDIR)/%.o, $(SOURCES))
TESTSRCS   := $(shell find $(TESTSDIR) -name '*.c')
TESTS      := $(patsubst $(TESTSDIR)/%.c, $(BUILDTESTDIR)/%.bin, $(TESTSRCS))

.PHONY: all compile compile-tests clean

all: compile compile-tests

compile: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(ENTRYPOINT) $(OBJECTS) $(LDFLAGS) -o $(BINARY)

$(BUILDOBJDIR)/%.o: $(SOURCEDIR)/%.c $(BUILDOBJDIR)
	$(CC) $(CFLAGS) -I$(SOURCEDIR) -c $< -o $@

$(BUILDOBJDIR):
	mkdir -p $@

compile-tests: $(TESTS)

$(BUILDTESTDIR)/%.bin: $(TESTSDIR)/%.c $(BUILDTESTDIR)
	$(CC) $(CFLAGS) -I$(SOURCEDIR) $< -o $(basename $@)

$(BUILDTESTDIR):
	mkdir -p $@

clean:
	rm -rf $(BUILDDIR)
