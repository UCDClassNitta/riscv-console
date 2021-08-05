AR=ar
AS=as
CC=gcc
CPP=cpp
CXX=g++
LD=ld
OBJCOPY = objcopy
OBJDUMP = objdump
STRIP = strip

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin

#DEBUG_MODE=TRUE

PKGS = gtk+-3.0 

ifdef DEBUG_MODE
DEFINES  += -DDEBUG
CFLAGS   += -g -ggdb -D_GLIBCXX_DEBUG
#LDFLAGS  += -Wl,-no_pie
else 
CFLAGS   += -O3
endif

INCLUDE  += -I $(INC_DIR)
CFLAGS   +=  -Wall `pkg-config --cflags $(PKGS)`
LDFLAGS  +=`pkg-config --libs $(PKGS)` 
#LDFLAGS += -lgdk_imlib
CPPFLAGS += -std=c++14 

SIM_APP_NAME = riscv-console-sim
AUTO_RUN_APP_NAME = riscv-console-autorunner

SIM_APP_OBJS = $(OBJ_DIR)/main.o                \
    $(OBJ_DIR)/AutoRunner.o                     \
    $(OBJ_DIR)/AutoRecorder.o                   \
    $(OBJ_DIR)/GraphicFactoryCairo.o            \
    $(OBJ_DIR)/GUIFactoryGTK3.o                 \
    $(OBJ_DIR)/GUIScrollableLabelBox.o          \
    $(OBJ_DIR)/GUIScrollableMemoryLabelBox.o    \
    $(OBJ_DIR)/RISCVConsoleApplication.o        \
    $(OBJ_DIR)/RISCVConsoleApplicationConfig.o  \
    $(OBJ_DIR)/PeriodicTimeout.o                \
    $(OBJ_DIR)/FileDataContainer.o              \
    $(OBJ_DIR)/FileDataSink.o                   \
    $(OBJ_DIR)/FileDataSource.o                 \
    $(OBJ_DIR)/LineDataSource.o                 \
    $(OBJ_DIR)/CommentSkipLineDataSource.o      \
    $(OBJ_DIR)/Path.o                           \
    $(OBJ_DIR)/MemoryControllerDevice.o         \
    $(OBJ_DIR)/MemoryDevice.o                   \
    $(OBJ_DIR)/FlashMemoryDevice.o              \
    $(OBJ_DIR)/RAMMemoryDevice.o                \
    $(OBJ_DIR)/RegisterBlockMemoryDevice.o      \
    $(OBJ_DIR)/ElfLoad.o                        \
    $(OBJ_DIR)/RISCVConsoleChipset.o            \
    $(OBJ_DIR)/RISCVConsole.o                   \
    $(OBJ_DIR)/RISCVCPU.o                       \
    $(OBJ_DIR)/RISCVRTypeInstruction.o          \
    $(OBJ_DIR)/RISCVITypeInstruction.o          \
    $(OBJ_DIR)/RISCVLTypeInstruction.o          \
    $(OBJ_DIR)/RISCVSTypeInstruction.o          \
    $(OBJ_DIR)/RISCVBTypeInstruction.o          \
    $(OBJ_DIR)/RISCVUTypeInstruction.o          \
    $(OBJ_DIR)/RISCVJTypeInstruction.o          \
    $(OBJ_DIR)/RISCVSYSTypeInstruction.o        \
    $(OBJ_DIR)/RISCVBlockInstructionCache.o     \
    $(OBJ_DIR)/MSXFont.o                        \
    $(OBJ_DIR)/VideoController.o

AUTO_RUN_APP_OBJS = $(OBJ_DIR)/autorunmain.o    \
    $(OBJ_DIR)/AutoRunner.o                     \
    $(OBJ_DIR)/GraphicFactoryCairo.o            \
    $(OBJ_DIR)/FileDataContainer.o              \
    $(OBJ_DIR)/FileDataSink.o                   \
    $(OBJ_DIR)/FileDataSource.o                 \
    $(OBJ_DIR)/Path.o                           \
    $(OBJ_DIR)/MemoryControllerDevice.o         \
    $(OBJ_DIR)/MemoryDevice.o                   \
    $(OBJ_DIR)/FlashMemoryDevice.o              \
    $(OBJ_DIR)/RAMMemoryDevice.o                \
    $(OBJ_DIR)/RegisterBlockMemoryDevice.o      \
    $(OBJ_DIR)/ElfLoad.o                        \
    $(OBJ_DIR)/RISCVConsoleChipset.o            \
    $(OBJ_DIR)/RISCVConsole.o                   \
    $(OBJ_DIR)/RISCVCPU.o                       \
    $(OBJ_DIR)/RISCVRTypeInstruction.o          \
    $(OBJ_DIR)/RISCVITypeInstruction.o          \
    $(OBJ_DIR)/RISCVLTypeInstruction.o          \
    $(OBJ_DIR)/RISCVSTypeInstruction.o          \
    $(OBJ_DIR)/RISCVBTypeInstruction.o          \
    $(OBJ_DIR)/RISCVUTypeInstruction.o          \
    $(OBJ_DIR)/RISCVJTypeInstruction.o          \
    $(OBJ_DIR)/RISCVSYSTypeInstruction.o        \
    $(OBJ_DIR)/RISCVBlockInstructionCache.o     \
    $(OBJ_DIR)/MSXFont.o                        \
    $(OBJ_DIR)/VideoController.o


all: directories $(BIN_DIR)/$(SIM_APP_NAME) $(BIN_DIR)/$(AUTO_RUN_APP_NAME)

$(BIN_DIR)/$(SIM_APP_NAME): $(SIM_APP_OBJS)
	$(CXX) $(SIM_APP_OBJS) -o $(BIN_DIR)/$(SIM_APP_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(LDFLAGS) 

$(BIN_DIR)/$(AUTO_RUN_APP_NAME): $(AUTO_RUN_APP_OBJS)
	$(CXX) $(AUTO_RUN_APP_OBJS) -o $(BIN_DIR)/$(AUTO_RUN_APP_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(LDFLAGS) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@
	
directories: $(OBJ_DIR) $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)
	
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)