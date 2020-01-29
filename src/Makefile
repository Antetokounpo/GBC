CC=g++
DEPS = Z80/z80.hpp gbc.hpp cartridge.hpp ppu.hpp
CFLAGS=-Wall

ODIR=obj

_OBJ = Z80/z80.o main.o gbc.o cartridge.o ppu.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

LIBS=-lSDL2

all: dist/gbemu

debug: CFLAGS += -DGBC_DEBUG -g
debug: dist/gbemu

$(ODIR)/%.o: %.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

dist/gbemu: $(OBJ)
	@mkdir -p $(@D)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -rf obj