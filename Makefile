SRC_DIR=src
BUILD_DIR=build

SRC_FILES=$(wildcard $(SRC_DIR)/*.cc) $(wildcard $(SRC_DIR)/*/*.cc)
HEADER_FILES=$(wildcard $(SRC_DIR)/*/*.h)

OUT_FILE=game.exe

OUT_PATH=$(BUILD_DIR)/$(OUT_FILE)

CC=g++

CPP_FLAG=-w -g -std=c++17

VCPKG_INSTALL=./vcpkg/installed/x64-mingw-dynamic

LIBRARY=-L$(VCPKG_INSTALL)/lib

LINKER=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image

INCLUDES=-I$(VCPKG_INSTALL)/include -Isrc

INCLUDE_LIB=precompiled/lib.h
PRECOMP_LIB=precompiled/lib.h.gch

$(OUT_PATH): $(SRC_FILES) $(HEADER_FILES) $(PRECOMP_LIB)
	$(CC) $(SRC_FILES) -o $(OUT_PATH) $(CPP_FLAG) $(LIBRARY) $(LINKER) $(INCLUDES) -include $(INCLUDE_LIB)

$(PRECOMP_LIB):$(INCLUDE_LIB)
	$(CC) $(INCLUDE_LIB) $(CPP_FLAG) $(INCLUDES)

run: $(OUT_PATH)
	cd $(BUILD_DIR) && $(OUT_FILE)

lint:
	cpplint --filter=-legal/copyright,-build/include_subdir,-whitespace/line_length $(SRC_FILES)
	