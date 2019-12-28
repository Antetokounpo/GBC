CC=g++
DEPS = Z80/z80.hpp gbc.hpp cartridge.hpp

ODIR=obj

_OBJ = Z80/z80.o main.o gbc.o cartridge.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< -Wall

gbemu: $(OBJ)
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -rf obj