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


files = main.c window.c vulkan.c
all_OBJS = $(addprefix $(OBJ)/, $(files:.c=.o))
all_DEPS = $(addprefix $(DEP)/, $(files:.c=.d))

compilerFlags = -c -O3 -m64
linkerFlags = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

all: $(all_DEPS) $(all_OBJS) 
	$(CC) $(all_OBJS) $(linkerFlags) -o $(BLD)/$(EXE)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(compilerFlags) -o $(OBJ)/$(basename $(notdir $@)).o $^ -c \
	$(inc_paths)

$(DEP)/%.d: $(SRC)/%.c
	$(CC) $^ -M \
	$(inc_paths)\
	> $(DEP)/$(basename $(notdir $@)).d

$(EXE):
	ln $(BLD)/$(EXE) . -s

.PHONY:clean
clean: 
	rm $(OBJ)/*
	rm $(DEP)/*
	rm $(BLD)/$(EXE)

include $(DEPS)
