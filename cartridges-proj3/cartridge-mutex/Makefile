TOOLPREFIX=riscv32-unknown-elf-
AR=$(TOOLPREFIX)ar
AS=$(TOOLPREFIX)as
CC=$(TOOLPREFIX)gcc
CPP=$(TOOLPREFIX)cpp
CXX=$(TOOLPREFIX)g++
LD=$(TOOLPREFIX)ld
OBJCOPY=$(TOOLPREFIX)objcopy
OBJDUMP=$(TOOLPREFIX)objdump
STRIP=$(TOOLPREFIX)strip

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin

DEBUG_MODE=TRUE

ifdef DEBUG_MODE
DEFINES  += -DDEBUG
CFLAGS   += -O0 -g -ggdb
#LDFLAGS  += -Wl,-no_pie
else 
CFLAGS   += -O3
endif

LDFILE   = riscv32-console.ld
INCLUDE  += -I $(INC_DIR)
CFLAGS   += -ffreestanding  -nostartfiles -nostdlib -nodefaultlibs
LDFLAGS  +=
LDFLAGS  += -Wl,--gc-sections -Wl,-T,$(LDFILE)

TARGET_NAME = cartridge-mutex
TARGET_OBJS =   $(OBJ_DIR)/main.o               \
                $(OBJ_DIR)/crt0.o               \
                $(OBJ_DIR)/startup.o

all: directories $(BIN_DIR)/$(TARGET_NAME)

$(BIN_DIR)/$(TARGET_NAME): $(TARGET_OBJS)
	$(CC) $(TARGET_OBJS) -o $(BIN_DIR)/$(TARGET_NAME) $(CFLAGS) $(DEFINES) $(LDFLAGS) 
	$(STRIP) $(BIN_DIR)/$(TARGET_NAME) -o $(BIN_DIR)/$(TARGET_NAME).strip

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@
	
directories: $(OBJ_DIR) $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)
	
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)