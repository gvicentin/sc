# ========================
# 	DIRECTORIES
# ========================
SOURCEDIR	:= src
TESTSDIR	:= tests
BUILDDIR	:= build
BINDIR		:= bin

# ========================
# 	COMPILER
# ========================
CC      := gcc -std=c11
CFLAGS	:= -Wall -g

# ========================
# 	BINARY
# ========================
NAME	:= sc
VERSION	:= 0.1.0
BINARY	:= $(BINDIR)/$(NAME)-$(VERSION)

SOURCES		:= $(shell find $(SOURCEDIR) -name '*.c')
OBJECTS 	:= $(patsubst $(SOURCEDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))
TESTS		:= $(shell find $(TESTSDIR) -name '*.c')
TESTSBIN	:= $(patsubst $(TESTSDIR)/%.c, $(BINDIR)/%, $(TESTS))

.PHONY: compile compile-tests test clean

compile: $(BINARY)

compile-tests: $(BINDIR) $(TESTSBIN)

test: compile-tests
	./runtests.sh

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

$(BINARY): $(BUILDDIR) $(BINDIR) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(BINARY)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) -I$(dir $<) -c $< -o $@

$(BINDIR)/%: $(TESTSDIR)/%.c
	$(CC) -I$(SOURCEDIR) $< -o $@

$(BUILDDIR):
	mkdir -p $@

$(BINDIR):
	mkdir -p $@
