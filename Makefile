CC     		:= g++
LD 			:= g++
CFLAGS 		:= $(CFLAGS) -pthread -Wall	
LFLAGS		:= $(LFLAGS) -lpthread -pthread

SRC_DIR     := src
OBJ_DIR 	:= bin

SRC_LIST    := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_LIST    := $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRC_LIST))
INCLUDE     := $(patsubst %,-I %,$(shell find $(SRC_DIR) -type d)) 

TARGET 		:= oven.app

default: clean $(TARGET)

$(OBJ_DIR)/%.o:
	@echo "Compiling" $@
	@mkdir -p $(dir $@)
	@$(CC) -c $(CFLAG) -o $@ $(patsubst $(OBJ_DIR)%.o,$(SRC_DIR)%.cpp,$@) $(INCLUDE)

$(TARGET): $(OBJ_LIST)
	@echo "Linking" $@
	@$(LD) -o $@ $(OBJ_LIST) $(LFLAGS)

.PHONY: clean
clean:
	@rm -rf $(OBJ_DIR) $(TARGET)
	
.PHONY: info
info:
	@echo "OBJ_DIR=" $(OBJ_DIR)
	@echo "SRC_DIR=" $(SRC_DIR)
	@echo "OBJ_LIST=" $(OBJ_LIST)
	@echo "SRC_LIST=" $(SRC_LIST)
	@echo "HEADERS=" $(HEADERS)
	@echo "INCLUDE=" $(INCLUDE)