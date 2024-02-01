SRC_DIR=src
ENGINE_DIR=lib/engine
BUILD_DIR=build
DEBUG_DIR=debug
PRECOMPILE_DIR=precompiled
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))


SRC_FILES=$(call rwildcard,$(SRC_DIR)/,*.cc)
ENGINE_FILES=$(call rwildcard,$(ENGINE_DIR)/,*.cc)

OBJ_FILES=$(addprefix $(PRECOMPILE_DIR)/,$(notdir $(SRC_FILES:.cc=.o)))
ENGINE_OBJ_FILE=$(addprefix $(PRECOMPILE_DIR)/,$(notdir $(ENGINE_FILES:.cc=.o)))

SRC_DIRS=$(sort $(dir $(SRC_FILES)))
ENGINE_DIRS=$(sort $(dir $(ENGINE_FILES)))

ifeq ($(OS),Windows_NT)
	OUT_FILE=game.exe
else
	OUT_FILE=game
endif
ENGINE_LIB=libEngine.a

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

LIBRARY=-L$(VCPKG_INSTALL)/lib -L$(PRECOMPILE_DIR)

ifeq ($(OS),Windows_NT)
LINKER+=-lEngine -luser32 -lImm32 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
endif




INCLUDES=-I$(VCPKG_INSTALL)/include -Isrc -Ilib

VPATH = $(SRC_DIR) $(SRC_DIRS) $(ENGINE_DIR) $(ENGINE_DIRS)

$(PRECOMPILE_DIR)/%.o: %.cc | $(PRECOMPILE_DIR)
	$(CC) -c $< -o $@ $(CPP_FLAG) $(INCLUDES)

$(PRECOMPILE_DIR)/$(ENGINE_LIB): $(ENGINE_OBJ_FILE)
	ar rvs $(PRECOMPILE_DIR)/$(ENGINE_LIB) $(ENGINE_OBJ_FILE)

$(OUT_PATH): $(OBJ_FILES) $(PRECOMPILE_DIR)/$(ENGINE_LIB)
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
	