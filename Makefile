SRC_DIR=src
BUILD_DIR=build

SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp)

OUT_FILE=$(BUILD_DIR)/game.exe

CC=g++

CPP_FLAG=-w -g

LIBRARY=-LSDL2/lib

INCLUDES=-ISDL2/include -lmingw32 -lSDL2main -lSDL2

$(OUT_FILE): $(SRC_FILES) 
	$(CC) $(SRC_FILES) -o $(OUT_FILE) $(CPP_FLAG) $(INCLUDES)

run: $(OUT_FILE)
	./$(OUT_FILE)
