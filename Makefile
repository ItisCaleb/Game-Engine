SRC_DIR=src
BUILD_DIR=build

SRC_FILES=$(wildcard $(SRC_DIR)/*.cc) $(wildcard $(SRC_DIR)/*/*.cc)

OUT_FILE=$(BUILD_DIR)/game.exe

CC=g++

CPP_FLAG=-w -g

LIBRARY=-LSDL2/lib

Linker=-lmingw32 -lSDL2main -lSDL2

INCLUDES=-ISDL2/include -Isrc

$(OUT_FILE): $(SRC_FILES) 
	$(CC) $(SRC_FILES) -o $(OUT_FILE) $(CPP_FLAG) $(LIBRARY) $(Linker) $(INCLUDES)

run: $(OUT_FILE)
	./$(OUT_FILE)

lint:
	cpplint --filter=-legal/copyright,-build/include_subdir,-whitespace/line_length $(SRC_FILES)