CC     		:= g++
LD 			:= g++
CFLAGS 		:= $(CFLAGS) -pthread -Wall	
LFLAGS		:= $(LFLAGS) -lpthread -pthread

SRC_DIR     := src
OBJ_DIR 	:= bin

SRC_LIST    := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_LIST    := $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRC_LIST))

TARGET 		:= oven.app

default: mkdirs $(TARGET)

mkdirs:
	mkdir -p $(SRC_DIR)
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o:
	$(CC) -c $(CFLAG) -o $@ $(SRC_LIST)

$(TARGET): $(OBJ_LIST)
	@echo OBJ_LIST: $(OBJ_LIST)
	$(LD) -o $@ $(OBJ_LIST) $(LFLAGS)

.PHONY: clean
clean:
	rm -rf $(OBJ_LIST) $(TARGET)
	make
	
