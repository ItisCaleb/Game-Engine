SRC_DIR=src
BUILD_DIR=build

SRC_FILES=$(wildcard $(SRC_DIR)/*.cc) $(wildcard $(SRC_DIR)/*/*.cc)
HEADER_FILES=$(wildcard $(SRC_DIR)/*/*.h)

OUT_FILE=game.exe

CC=g++

CPP_FLAG=-w -g -std=c++17

VCPKG_INSTALL=./vcpkg/installed/x64-mingw-dynamic

LIBRARY=-L$(VCPKG_INSTALL)/lib

Linker=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image

INCLUDES=-I$(VCPKG_INSTALL)/include -Isrc

$(OUT_FILE): $(SRC_FILES) $(HEADER_FILES)
	$(CC) $(SRC_FILES) -o $(BUILD_DIR)/$(OUT_FILE) $(CPP_FLAG) $(LIBRARY) $(Linker) $(INCLUDES)

run: $(OUT_FILE)
	cd $(BUILD_DIR) && $(OUT_FILE)

lint:
	cpplint --filter=-legal/copyright,-build/include_subdir,-whitespace/line_length $(SRC_FILES)