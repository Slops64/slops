CSOURCES 	:= $(shell find . -type f -name "*.c")
OBJ 		:= $(CSOURCES:.c=.o)
BUILTIN		:= built-in.o

.PHONY: all clean

all: $(BUILTIN)

$(BUILTIN): $(OBJ)
	ld -relocatable $^ -o $@

clean:
	-rm $(BUILTIN) $(OBJ)