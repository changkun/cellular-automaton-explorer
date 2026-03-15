# Life-like Cellular Automaton Explorer
# https://github.com/changkun/null-claude3

PREFIX   ?= /usr/local
BINDIR   ?= $(PREFIX)/bin

CC       ?= gcc
CFLAGS   ?= -O2 -Wall -Wextra -pedantic -std=c99
LDFLAGS  ?=
LIBS     := -lm

BIN      := life
SRC      := life.c

# ── Build targets ──────────────────────────────────────────────

.PHONY: all clean install uninstall debug sanitize help

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

# Debug build (symbols, no optimisation, address sanitizer)
debug: CFLAGS := -g -O0 -Wall -Wextra -pedantic -std=c99 -DDEBUG
debug: $(BIN)

# Sanitizer build (ASan + UBSan)
sanitize: CFLAGS := -g -O1 -Wall -Wextra -pedantic -std=c99 \
	-fsanitize=address,undefined -fno-omit-frame-pointer
sanitize: LDFLAGS := -fsanitize=address,undefined
sanitize: $(BIN)

# ── Install / Uninstall ───────────────────────────────────────

install: $(BIN)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(BIN) $(DESTDIR)$(BINDIR)/$(BIN)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(BIN)

# ── Housekeeping ──────────────────────────────────────────────

clean:
	rm -f $(BIN)

# ── Help ──────────────────────────────────────────────────────

help:
	@echo "Life — Cellular Automaton Explorer"
	@echo ""
	@echo "Targets:"
	@echo "  make            Build optimised binary"
	@echo "  make debug      Build with debug symbols, no optimisation"
	@echo "  make sanitize   Build with AddressSanitizer + UBSan"
	@echo "  make install    Install to $(BINDIR)"
	@echo "  make uninstall  Remove from $(BINDIR)"
	@echo "  make clean      Remove build artifacts"
	@echo ""
	@echo "Variables:"
	@echo "  CC=clang        Use a different compiler"
	@echo "  PREFIX=/opt     Change install prefix (default: /usr/local)"
