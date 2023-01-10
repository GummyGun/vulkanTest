TMP = tmps
OBJ = $(TMP)/obj
DEP = $(TMP)/dep
SRC = src
BLD = BUILD
CC = gcc
EXE = ADASTRA
inc_paths = 
	
SPV = res/shaders
rpath =  $(shell pwd)


files = main.c window.c vulkan.c vGraphics.c
all_OBJS = $(addprefix $(OBJ)/, $(files:.c=.o))
all_DEPS = $(addprefix $(DEP)/, $(files:.c=.d))

compilerFlags = -c -O3 -m64
linkerFlags = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi


all: $(all_DEPS) $(all_OBJS) 
	$(CC) $(all_OBJS) $(linkerFlags) -o $(BLD)/$(EXE).out

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(compilerFlags) -o $@ $< -c \
	$(inc_paths)

$(DEP)/%.d: $(SRC)/%.c
	$(CC) $^ -MM \
	-MT $(OBJ)/$(basename $(notdir $@)).o \
	$(inc_paths) \
	-MF $@

$(EXE):
	ln $(BLD)/$(EXE).out ./$(EXE) -s

.PHONY:clean
clean: 
	rm -f $(OBJ)/*
	rm -f $(DEP)/*
	rm -f $(BLD)/$(EXE)

-include $(DEP)/*

