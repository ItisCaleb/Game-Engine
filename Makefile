SRC_DIR=src
BUILD_DIR=build
PRECOMPILE_DIR=precompiled

SRC_FILES=$(wildcard $(SRC_DIR)/*.cc) $(wildcard $(SRC_DIR)/*/*.cc)
OBJ_FILES=$(addprefix $(PRECOMPILE_DIR)/,$(notdir $(SRC_FILES:.cc=.o)))
HEADER_FILES=$(wildcard $(SRC_DIR)/**/*.h)
SRC_DIRS=$(sort $(dir $(SRC_FILES)))

OUT_FILE=game.exe

OUT_PATH=$(BUILD_DIR)/$(OUT_FILE)

CC=g++

CPP_FLAG=-w -g -std=c++17

VCPKG_INSTALL=./vcpkg/installed/x64-mingw-dynamic

LIBRARY=-L$(VCPKG_INSTALL)/lib

LINKER=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image

INCLUDES=-I$(VCPKG_INSTALL)/include -Isrc

VPATH = $(SRC_DIR) $(SRC_DIRS)

$(PRECOMPILE_DIR)/%.o: %.cc | $(PRECOMPILE_DIR)
	$(CC) -c $< -o $@ $(CPP_FLAG) $(INCLUDES)

$(OUT_PATH): $(OBJ_FILES) $(HEADER_FILES) $(PRECOMP_LIB)
	$(CC) $(OBJ_FILES) -o $(OUT_PATH) $(CPP_FLAG) $(LIBRARY) $(LINKER) $(INCLUDES)


run: $(OUT_PATH)
	cd $(BUILD_DIR) && $(OUT_FILE)

clean:
ifeq ($(OS),Windows_NT)
	del $(PRECOMPILE_DIR)\*.o
	del $(BUILD_DIR)\$(OUT_FILE)
endif

lint:
	cpplint --filter=-legal/copyright,-build/include_subdir,-whitespace/line_length $(SRC_FILES)
	