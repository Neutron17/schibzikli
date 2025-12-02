CC = cc

PNAME = spiel

SRC  = $(shell find src -name "*.c")
OBJ  = $(SRC:.c=.o)
BIN = build

EXEC = $(BIN)/$(PNAME)
INCFLAGS  = -Isrc/

CCFLAGS += $(INCFLAGS)
CCFLAGS += -O0
CCFLAGS += -Wall
CCFLAGS += -pedantic
CCFLAGS += -fsanitize=address

LDFLAGS  = $(INCFLAGS)
LDFLAGS += -lpthread
LDFLAGS += -lSDL2
LDFLAGS += -lSDL2_image
LDFLAGS += -lrt
LDFLAGS += -lm

INSTALL_PATH = /usr/local/bin

all: build

run: build
	$(BIN)/$(PNAME) $*

build: $(OBJ)
	$(CC) $(CCFLAGS) -ggdb -o $(BIN)/$(PNAME) $(filter %.o,$^) $(LDFLAGS)

clean:
	rm $(BIN)/* $(OBJ) logs.txt

install: build
	cp $(BIN)/$(PNAME) $(INSTALL_PATH)

%.o: %.c
	$(CC) -ggdb -o $@ -c $< $(CCFLAGS)

