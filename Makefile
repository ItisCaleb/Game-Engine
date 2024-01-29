SRC_DIR=src
BUILD_DIR=build
DEBUG_DIR=debug
PRECOMPILE_DIR=precompiled
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))


SRC_FILES=$(call rwildcard,src/,*.cc)
OBJ_FILES=$(addprefix $(PRECOMPILE_DIR)/,$(notdir $(SRC_FILES:.cc=.o)))
HEADER_FILES=$(wildcard $(SRC_DIR)/**/*.h)
SRC_DIRS=$(sort $(dir $(SRC_FILES)))

ifeq ($(OS),Windows_NT)
	OUT_FILE=game.exe
else
	OUT_FILE=game
endif

OUT_PATH=$(BUILD_DIR)/$(OUT_FILE)

CC=g++

CPP_FLAG=-w -std=c++17
debug: CPP_FLAG+= -g

ifeq ($(OS),Windows_NT)
VCPKG_INSTALL=./vcpkg/installed/x64-mingw-dynamic
else
	ifeq ($(shell uname -s),Darwin)
		CPP_FLAG += -Wno-narrowing -Wl,-rpath,"./lib"
        VCPKG_INSTALL=./vcpkg/installed/arm64-osx-dynamic
	endif
endif

LIBRARY=-L$(VCPKG_INSTALL)/lib
DEBUG=-L$(VCPKG_INSTALL)/debug

ifeq ($(OS),Windows_NT)
LINKER=-luser32 -lImm32 -lmingw32 
DEBUG_LINKER=-luser32 -lImm32 -lmingw32 
else
LINKER=
DEBUG_LINKER=
endif

LINKER+= -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
DEBUG_LINKER+= -lSDL2maind -lSDL2d -lSDL2_imaged -lSDL2_ttf





INCLUDES=-I$(VCPKG_INSTALL)/include -Isrc

VPATH = $(SRC_DIR) $(SRC_DIRS)

$(PRECOMPILE_DIR)/%.o: %.cc | $(PRECOMPILE_DIR)
	$(CC) -c $< -o $@ $(CPP_FLAG) $(INCLUDES)

$(OUT_PATH): $(OBJ_FILES) $(HEADER_FILES) $(PRECOMP_LIB)
ifeq ($(OS),Windows_NT)
	if not exist "$(BUILD_DIR)" mkdir $(BUILD_DIR)
	xcopy "$(VCPKG_INSTALL)/bin" $(BUILD_DIR) /h /c /i /y /q
	xcopy "$(SRC_DIR)/assets" "$(BUILD_DIR)/assets"  /e /h /c /i /y /q
else
	mkdir -p $(BUILD_DIR)
	cp -a $(VCPKG_INSTALL)/lib/. $(BUILD_DIR)/lib/
	cp -a $(SRC_DIR)/assets/. $(BUILD_DIR)/assets/
endif
	$(CC) $(OBJ_FILES) -o $(OUT_PATH) $(CPP_FLAG) $(LIBRARY) $(LINKER) $(INCLUDES)


run: $(OUT_PATH)
ifeq ($(OS),Windows_NT)
	cd $(BUILD_DIR) && $(OUT_FILE)
else
	cd $(BUILD_DIR) && ./$(OUT_FILE)
endif

debug: $(OUT_PATH)
ifeq ($(OS),Windows_NT)
	if not exist "$(DEBUG_DIR)" mkdir $(DEBUG_DIR)
	xcopy "$(VCPKG_INSTALL)/debug/bin" $(DEBUG_DIR) /e /h /c /i /y /q
	xcopy "$(SRC_DIR)/assets" "$(DEBUG_DIR)/assets"  /e /h /c /i /y /q
endif
	$(CC) $(OBJ_FILES) -o $(DEBUG_DIR)/$(OUT_FILE) $(CPP_FLAG) $(DEBUG)/lib $(DEBUG_LINKER) $(INCLUDES)


clean:
ifeq ($(OS),Windows_NT)
	del $(PRECOMPILE_DIR)\*.o
	del $(BUILD_DIR)\$(OUT_FILE)
	del $(DEBUG_DIR)\$(OUT_FILE)
else
	rm $(precompiled)\*.o
	rm -rf $(BUILD_DIR)
endif



lint:
	cpplint --filter=-legal/copyright,-build/include_subdir,-whitespace/line_length $(SRC_FILES)
	