.PHONY: clean all build run

CUR_DIR := .
BIN_DIR := $(CUR_DIR)/bin
LIB_DIR := $(CUR_DIR)/lib
LIB_SHA_DIR := $(LIB_DIR)/shared
LIB_STA_DIR := $(LIB_DIR)/static
OBJ_DIR := $(CUR_DIR)/obj
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
COMPILER := gcc
MAIN_O_DIR := $(OBJ_DIR)/main.o

SRC_FILES := $(foreach SRC_DIR, $(SRC_DIR), $(wildcard $(SRC_DIR)/*.c))
INC_FILES := $(foreach INC_DIR, $(INC_DIR), $(wildcard $(INC_DIR)/*.h))

OBJ_FILES := $(notdir $(SRC_FILES))
OBJ_FILES := $(subst .c,.o, $(OBJ_FILES))
PATH_CLIENT_OBJS := $(foreach OBJ_FILES, $(OBJ_FILES), $(OBJ_DIR)/$(OBJ_FILES))
PATH_CLIENT_OBJS += $(MAIN_O_DIR)
PATH_SERVER_OBJS := $(foreach OBJ_FILES, $(OBJ_FILES), $(OBJ_DIR)/$(OBJ_FILES))
PATH_SERVER_OBJS += $(MAIN_PROCESS_B_O_DIR)

vpath %.c $(SRC_DIR)
vpath %.h $(INC_DIR)

build: $(OBJ_FILES) main.o
	@$(COMPILER) $(PATH_CLIENT_OBJS) -o $(BIN_DIR)/app

%.o: %.c $(INC_FILES)
	@$(COMPILER) -I$(INC_DIR) -c $< -o $(OBJ_DIR)/$@

main.o:
	@$(COMPILER) -I$(INC_DIR) -c main.c -o $(CUR_DIR)/$(OBJ_DIR)/main.o

run:
	@$(BIN_DIR)/app

all: clean build run

clean:
	@rm -rf $(OBJ_DIR)/*.o