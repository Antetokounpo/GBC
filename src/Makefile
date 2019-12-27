CC=g++
DEPS = Z80/z80.hpp gbc.hpp

ODIR=obj

_OBJ = Z80/z80.o gbc.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $<

gbemu: $(OBJ)
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -rf obj