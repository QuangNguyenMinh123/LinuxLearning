.PHONY: clean a b run_a run_b build_a build_b

CUR_DIR := .
BIN_DIR := $(CUR_DIR)/bin
LIB_DIR := $(CUR_DIR)/lib
LIB_SHA_DIR := $(LIB_DIR)/shared
LIB_STA_DIR := $(LIB_DIR)/static
OBJ_DIR := $(CUR_DIR)/obj
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
COMPILER := gcc
MAIN_PROCESS_A_O_DIR := $(OBJ_DIR)/main_ProcessA.o
MAIN_PROCESS_B_O_DIR := $(OBJ_DIR)/main_ProcessB.o

SRC_FILES := $(foreach SRC_DIR, $(SRC_DIR), $(wildcard $(SRC_DIR)/*.c))
INC_FILES := $(foreach INC_DIR, $(INC_DIR), $(wildcard $(INC_DIR)/*.h))

OBJ_FILES := $(notdir $(SRC_FILES))
OBJ_FILES := $(subst .c,.o, $(OBJ_FILES))
PATH_CLIENT_OBJS := $(foreach OBJ_FILES, $(OBJ_FILES), $(OBJ_DIR)/$(OBJ_FILES))
PATH_CLIENT_OBJS += $(MAIN_PROCESS_A_O_DIR)
PATH_SERVER_OBJS := $(foreach OBJ_FILES, $(OBJ_FILES), $(OBJ_DIR)/$(OBJ_FILES))
PATH_SERVER_OBJS += $(MAIN_PROCESS_B_O_DIR)

vpath %.c $(SRC_DIR)
vpath %.h $(INC_DIR)

build_a: $(OBJ_FILES) main_ProcessA.o
	@$(COMPILER) $(PATH_CLIENT_OBJS) -o $(BIN_DIR)/a

build_b: $(OBJ_FILES) main_ProcessB.o
	@$(COMPILER) $(PATH_SERVER_OBJS) -o $(BIN_DIR)/b

%.o: %.c $(INC_FILES)
	@$(COMPILER) -I$(INC_DIR) -c $< -o $(OBJ_DIR)/$@

main_ProcessA.o:
	@$(COMPILER) -I$(INC_DIR) -c main_ProcessA.c -o $(CUR_DIR)/$(OBJ_DIR)/main_ProcessA.o

main_ProcessB.o:
	@$(COMPILER) -I$(INC_DIR) -c main_ProcessB.c -o $(CUR_DIR)/$(OBJ_DIR)/main_ProcessB.o

run_a:
	@$(BIN_DIR)/a

run_b:
	@$(BIN_DIR)/b

a: clean build_a run_a

b: clean build_b run_b

clean:
	@rm -rf $(OBJ_DIR)/*.o