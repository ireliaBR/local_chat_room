
INCLUDE_DIR = ./src/lib
SRC_DIR = src
BUILD_DIR = build
PRODUCT_DIR = product


SERVER_TARGET = server
CLIENT_TARGET = client
CC = gcc

CPPFLAGS= -I$(INCLUDE_DIR)
CFLAGS= -g -Wall
LDFLAGS= 

SRCEXT := .c 
SRCDIRS := $(patsubst ./%,%,$(shell find $(SRC_DIR) -type d))
SRC	:= $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*, $(SRCEXT))))
OBJ = $(patsubst %.c,%.o,$(SRC))

all:$(CLIENT_TARGET) $(SERVER_TARGET)

$(CLIENT_TARGET):$(OBJ)
	$(CC) $(filter-out src/server.o,$^) -o $(PRODUCT_DIR)/$@
$(SERVER_TARGET):$(OBJ)
	$(CC) $(filter-out src/client.o,$^) -o $(PRODUCT_DIR)/$@
$(OBJ):%.o:%.c
	$(CC) -c $(CPPFLAGS) $< -o $@

.PHONY:clean
clean:
	-find ./ -name '*.o' | xargs rm -rf
	-rm -rf $(PRODUCT_DIR)/*
