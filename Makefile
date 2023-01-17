target = Way

TMP = tmps
OBJ = $(TMP)/obj
DEP = $(TMP)/dep
SRC = src
BLD = BUILD
EXE = ADASTRA
inc_paths = 
	
SPV = res/shaders
rpath =  $(shell pwd)


files = main.c window.c vulkan.c vGraphics.c utils.c
all_OBJS = $(addprefix $(OBJ)/, $(files:.c=.o))
all_DEPS = $(addprefix $(DEP)/, $(files:.c=.d))

.SECONDEXPANSION:

compiler = $(compiler$(target))
compilerWay = $(CC)
compilerX11 = gcc

defineFlags = $(define$(target)Flags)
defineWayFlags = -D WAY_PROT
defineX11Flags = -D X11_PROT

compilerFlags = $(compiler$(target)Flags)
compilerWayFlags = 
compilerX11Flags = -O3 -m64

linkerFlags = $(linker$(target)Flags)
linkerWayFlags = -lvulkan -lpthread -lwayland-client
linkerX11Flags = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

action = $(action$(target))
actionWay = send
actionX11 = symlink


build: all $(action)

all: $(all_DEPS) $(all_OBJS) 
	$(CC) $(all_OBJS) $(linkerFlags) -o $(BLD)/$(EXE).out

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(compilerFlags) $(defineFlags) -c \
	-o $@ $< \
	$(inc_paths)

$(DEP)/%.d: $(SRC)/%.c
	$(CC) $^ -MM \
	-MT $(OBJ)/$(basename $(notdir $@)).o \
	$(inc_paths)  $(defineFlags) \
	-MF $@

.PHONY:clean getBld
clean: 
	rm -f $(OBJ)/*
	rm -f $(DEP)/*
	rm -f $(BLD)/$(EXE).out

getBld:
	@echo "----------------------------------------------"
	@echo "defines $(defineFlags)"
	@echo "----------------------------------------------"
	@echo "compiler $(compiler) VAR not used currently"
	@echo "----------------------------------------------"
	@echo "linker $(linkerFlags)"
	@echo "----------------------------------------------"
	@echo "action $(action)"
	@echo "----------------------------------------------"

symlink:
	ln $(BLD)/$(EXE).out ./$(EXE) -s

send:
	scp $(BLD)/$(EXE).out root@$(TESTIP):~

-include $(DEP)/*
