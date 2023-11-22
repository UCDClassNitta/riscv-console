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

INCLUDE  += -I $(INC_DIR) -I $(INC_DIR)/datautils -I $(INC_DIR)/gui -I $(INC_DIR)/gtk3 
CFLAGS   += -Wall `pkg-config --cflags $(PKGS)`
LDFLAGS  +=`pkg-config --libs $(PKGS)` 
#LDFLAGS += -lgdk_imlib
CPPFLAGS += -std=c++14 

SIM_APP_NAME = riscv-console-sim
AUTO_RUN_APP_NAME = riscv-console-autorunner

GUI_OBJS =              $(OBJ_DIR)/GUIEvent.o                       \
                        $(OBJ_DIR)/GUIJustification.o               \
                        $(OBJ_DIR)/GUIFileFilter.o                  \
                        $(OBJ_DIR)/GUIScrollableLabelBox.o          \
                        $(OBJ_DIR)/GUIScrollableMemoryLabelBox.o    \
                        $(OBJ_DIR)/GUIScrollableTextViewLineBox.o   \
                        $(OBJ_DIR)/GUITextViewTreeNodeView.o 

GTK3_GRAPHICS_OBJS =    $(OBJ_DIR)/GraphicFactoryCairo.o            \
                        $(OBJ_DIR)/GraphicResourceContextCairo.o    \
                        $(OBJ_DIR)/GraphicSurfaceCairo.o

GTK3_OBJS =             $(OBJ_DIR)/GUIApplicationGTK3.o             \
                        $(OBJ_DIR)/GUIBoxGTK3.o                     \
                        $(OBJ_DIR)/GUIButtonGTK3.o                  \
                        $(OBJ_DIR)/GUIComboBoxGTK3.o                \
                        $(OBJ_DIR)/GUIContainerGTK3.o               \
                        $(OBJ_DIR)/GUICursorGTK3.o                  \
                        $(OBJ_DIR)/GUIDisplayGTK3.o                 \
                        $(OBJ_DIR)/GUIDrawingAreaGTK3.o             \
                        $(OBJ_DIR)/GUIEventBoxGTK3.o                \
                        $(OBJ_DIR)/GUIEventGTK3.o                   \
                        $(OBJ_DIR)/GUIFactoryGTK3.o                 \
                        $(OBJ_DIR)/GUIFileChooserDialogGTK3.o       \
                        $(OBJ_DIR)/GUIFrameGTK3.o                   \
                        $(OBJ_DIR)/GUIGridGTK3.o                    \
                        $(OBJ_DIR)/GUILabelGTK3.o                   \
                        $(OBJ_DIR)/GUIMenuBarGTK3.o                 \
                        $(OBJ_DIR)/GUIMenuGTK3.o                    \
                        $(OBJ_DIR)/GUIMenuItemGTK3.o                \
                        $(OBJ_DIR)/GUIMenuShellGTK3.o               \
                        $(OBJ_DIR)/GUIScrollBarGTK3.o               \
                        $(OBJ_DIR)/GUIScrollWindowGTK3.o            \
                        $(OBJ_DIR)/GUITextBufferGTK3.o              \
                        $(OBJ_DIR)/GUITextIterGTK3.o                \
                        $(OBJ_DIR)/GUITextTagGTK3.o                 \
                        $(OBJ_DIR)/GUITextViewGTK3.o                \
                        $(OBJ_DIR)/GUIToggleButtonGTK3.o            \
                        $(OBJ_DIR)/GUIWidgetGTK3.o                  \
                        $(OBJ_DIR)/GUIWindowGTK3.o

GRAPHICS_OBJS =         $(GTK3_GRAPHICS_OBJS)
GUI_FRAMEWORK =         $(GUI_OBJS) $(GRAPHICS_OBJS)
GUI_FRAMEWORK +=        $(GTK3_OBJS)

RISCV_CONSOLE_OBJS =    $(OBJ_DIR)/MemoryControllerDevice.o         \
                        $(OBJ_DIR)/MemoryDevice.o                   \
                        $(OBJ_DIR)/FlashMemoryDevice.o              \
                        $(OBJ_DIR)/RAMMemoryDevice.o                \
                        $(OBJ_DIR)/RegisterBlockMemoryDevice.o      \
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
                        $(OBJ_DIR)/Model01VideoController.o         \
                        $(OBJ_DIR)/Model02VideoController.o         \
                        $(OBJ_DIR)/VideoControllerAllocator.o

ELF_DWARF_OBJS =        $(OBJ_DIR)/DwarfStructures.o                \
                        $(OBJ_DIR)/ElfLoad.o

DATA_UTIL_OBJS =        $(OBJ_DIR)/BufferedSeekableDataSource.o     \
                        $(OBJ_DIR)/CommentSkipLineDataSource.o      \
                        $(OBJ_DIR)/FileDataContainer.o              \
                        $(OBJ_DIR)/FileDataSink.o                   \
                        $(OBJ_DIR)/FileDataSource.o                 \
                        $(OBJ_DIR)/LineDataSource.o                 \
                        $(OBJ_DIR)/Path.o                           \
                        $(OBJ_DIR)/SeekableDataSourceConverter.o    \
                        $(OBJ_DIR)/SeekableDataSourceSandbox.o

SIM_APP_OBJS =          $(OBJ_DIR)/main.o                           \
                        $(OBJ_DIR)/AutoRunner.o                     \
                        $(OBJ_DIR)/AutoRecorder.o                   \
                        $(OBJ_DIR)/RISCVConsoleApplication.o        \
                        $(OBJ_DIR)/RISCVConsoleApplicationConfig.o  \
                        $(OBJ_DIR)/PeriodicTimeout.o                \
                        $(OBJ_DIR)/VariableTranslator.o             \
                        $(OBJ_DIR)/VariableTreeViewDecorator.o      \
                        $(GUI_FRAMEWORK)                            \
                        $(DATA_UTIL_OBJS)                           \
                        $(ELF_DWARF_OBJS)                           \
                        $(RISCV_CONSOLE_OBJS)    

AUTO_RUN_APP_OBJS =     $(OBJ_DIR)/autorunmain.o                    \
                        $(OBJ_DIR)/AutoRunner.o                     \
                        $(GRAPHICS_OBJS)                            \
                        $(DATA_UTIL_OBJS)                           \
                        $(ELF_DWARF_OBJS)                           \
                        $(RISCV_CONSOLE_OBJS)

all: directories $(BIN_DIR)/$(SIM_APP_NAME) $(BIN_DIR)/$(AUTO_RUN_APP_NAME)

$(BIN_DIR)/$(SIM_APP_NAME): $(SIM_APP_OBJS)
	$(CXX) $(SIM_APP_OBJS) -o $(BIN_DIR)/$(SIM_APP_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(LDFLAGS) 

$(BIN_DIR)/$(AUTO_RUN_APP_NAME): $(AUTO_RUN_APP_OBJS)
	$(CXX) $(AUTO_RUN_APP_OBJS) -o $(BIN_DIR)/$(AUTO_RUN_APP_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(LDFLAGS) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/datautils/%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/gui/%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/gtk3/%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@

directories: $(OBJ_DIR) $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)
	
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
