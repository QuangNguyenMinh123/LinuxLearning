.PHONY: clean client server

A := .
CUR_DIR := .
BIN_DIR := $(CUR_DIR)/bin
LIB_DIR := $(CUR_DIR)/lib
LIB_SHA_DIR := $(LIB_DIR)/shared
LIB_STA_DIR := $(LIB_DIR)/static
OBJ_DIR := $(CUR_DIR)/obj
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
COMPILER := gcc
MAIN_CLIENT_O_DIR := $(OBJ_DIR)/main_client.o
MAIN_SERVER_O_DIR := $(OBJ_DIR)/main_server.o

temp := 20
DEFAULT_TEMP := 20
DEFAULT_IP := "192.168.0.106"

SRC_FILES := $(foreach SRC_DIR, $(SRC_DIR), $(wildcard $(SRC_DIR)/*.c))
INC_FILES := $(foreach INC_DIR, $(INC_DIR), $(wildcard $(INC_DIR)/*.h))

OBJ_FILES := $(notdir $(SRC_FILES))
OBJ_FILES := $(subst .c,.o, $(OBJ_FILES))
PATH_CLIENT_OBJS := $(foreach OBJ_FILES, $(OBJ_FILES), $(OBJ_DIR)/$(OBJ_FILES))
PATH_CLIENT_OBJS += $(MAIN_CLIENT_O_DIR)
PATH_SERVER_OBJS := $(foreach OBJ_FILES, $(OBJ_FILES), $(OBJ_DIR)/$(OBJ_FILES))
PATH_SERVER_OBJS += $(MAIN_SERVER_O_DIR)

vpath %.c $(SRC_DIR)
vpath %.h $(INC_DIR)

build_client:
	@$(COMPILER) main_client.c -o $(BIN_DIR)/client

build_server: $(OBJ_FILES) main_server.o
	@$(COMPILER) $(PATH_SERVER_OBJS) -o $(BIN_DIR)/server -pthread -fno-stack-protector -lm

%.o: %.c $(INC_FILES)
	@$(COMPILER) -I$(INC_DIR) -c $< -o $(OBJ_DIR)/$@

main_client.o:
	@$(COMPILER) -I$(INC_DIR) -c main_client.c -o $(CUR_DIR)/$(OBJ_DIR)/main_client.o

main_server.o:
	@$(COMPILER) -I$(INC_DIR) -c main_server.c -o $(CUR_DIR)/$(OBJ_DIR)/main_server.o

run_client:
	@$(BIN_DIR)/client $(DEFAULT_IP) $(temp)

run_server:
	@$(BIN_DIR)/server

client: clean build_client clean run_client

server: clean build_server clean run_server

clean:
	@rm -rf $(OBJ_DIR)/*.o