SRC_DIR=src
BUILD_DIR=build
DEBUG_DIR=debug
PRECOMPILE_DIR=precompiled
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))


SRC_FILES=$(call rwildcard,src/,*.cc)
OBJ_FILES=$(addprefix $(PRECOMPILE_DIR)/,$(notdir $(SRC_FILES:.cc=.o)))
HEADER_FILES=$(wildcard $(SRC_DIR)/**/*.h)
SRC_DIRS=$(sort $(dir $(SRC_FILES)))

OUT_FILE=game.exe

OUT_PATH=$(BUILD_DIR)/$(OUT_FILE)

CC=g++

CPP_FLAG=-w -std=c++17

VCPKG_INSTALL=./vcpkg/installed/x64-mingw-dynamic

LIBRARY=-L$(VCPKG_INSTALL)/lib
DEBUG=-L$(VCPKG_INSTALL)/debug

LINKER=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
DEBUG_LINKER=-lmingw32 -lSDL2maind -lSDL2d -lSDL2_imaged -lSDL2_ttf

ifeq ($(OS),Windows_NT)
LINKER+= -luser32 -lImm32
DEBUG_LINKER += -luser32 -lImm32
endif



INCLUDES=-I$(VCPKG_INSTALL)/include -Isrc

VPATH = $(SRC_DIR) $(SRC_DIRS)

$(PRECOMPILE_DIR)/%.o: %.cc | $(PRECOMPILE_DIR)
	$(CC) -c $< -o $@ $(CPP_FLAG) $(INCLUDES)

$(OUT_PATH): $(OBJ_FILES) $(HEADER_FILES) $(PRECOMP_LIB)
ifeq ($(OS),Windows_NT)
	if not exist "$(BUILD_DIR)" mkdir $(BUILD_DIR)
	xcopy "$(VCPKG_INSTALL)/bin" $(BUILD_DIR) /h /c /i /y /q
	xcopy "$(SRC_DIR)/assets" "$(BUILD_DIR)/assets"  /e /h /c /i /y /q
endif
	$(CC) $(OBJ_FILES) -o $(OUT_PATH) $(CPP_FLAG) $(LIBRARY) $(LINKER) $(INCLUDES)


run: $(OUT_PATH)
	cd $(BUILD_DIR) && $(OUT_FILE)

debug: $(OUT_PATH)
ifeq ($(OS),Windows_NT)
	if not exist "$(DEBUG_DIR)" mkdir $(DEBUG_DIR)
	xcopy "$(VCPKG_INSTALL)/debug/bin" $(DEBUG_DIR) /e /h /c /i /y
	xcopy "$(SRC_DIR)/assets" "$(DEBUG_DIR)/assets"  /e /h /c /i /y
endif
	$(CC) $(OBJ_FILES) -o $(OUT_PATH) $(CPP_FLAG) -g $(DEBUG)/lib $(DEBUG_LINKER) $(INCLUDES)


clean:
ifeq ($(OS),Windows_NT)
	del $(PRECOMPILE_DIR)\*.o
	del $(BUILD_DIR)\$(OUT_FILE)
endif



lint:
	cpplint --filter=-legal/copyright,-build/include_subdir,-whitespace/line_length $(SRC_FILES)
	