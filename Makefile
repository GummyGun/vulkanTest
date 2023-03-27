target = X11
sendIp = $(TESTIP)

TMP = tmps
OBJ = $(TMP)/obj
DEP = $(TMP)/dep
SRC = src
BLD = BUILD
EXE = ADASTRA
inc_paths = 
	
SPV = res/shaders
rpath =  $(shell pwd)


files = main.c window.c vInit.c vGraphics.c utils.c
all_OBJS = $(addprefix $(OBJ)/, $(files:.c=.o))
all_DEPS = $(addprefix $(DEP)/, $(files:.c=.d))

.SECONDEXPANSION:

compiler = $(compiler$(target))
compilerWay = $(CC) compilerX11 = gcc

defineFlags = $(define$(target)Flags)
defineWayFlags = -D WAY_PROT
defineX11Flags = -D X11_PROT

includeDirs = $(includeDirs$(target))
includeDirsWay = -Ires/wayland
includeDirsX11 = 

compilerFlags = $(compiler$(target)Flags)
compilerWayFlags = 
compilerX11Flags = 

linkerFlags = $(linker$(target)Flags)
linkerWayFlags = -lvulkan -lpthread -lwayland-client
linkerX11Flags = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

startAction = $(startAction$(target))
startActionWay = genXDG
startActionX11 = runningX11

endAction = $(endAction$(target))
endActionWay = send
endActionX11 = symlink


build: $(startAction) all $(endAction)

all: $(all_DEPS) $(all_OBJS) 
	$(CC) $(all_OBJS) $(linkerFlags) -o $(BLD)/$(EXE).out

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(compilerFlags) \
	$(defineFlags) $(includeDirs) -c \
	-o $@ $< \
	$(inc_paths)

$(DEP)/%.d: $(SRC)/%.c
	$(CC) $^ -MM \
	-MT $(OBJ)/$(basename $(notdir $@)).o \
	$(inc_paths) $(includeDirs) $(defineFlags) \
	-MF $@

.PHONY:clean getBld
clean: 
	rm -f $(OBJ)/*
	rm -f $(DEP)/*
	rm -f $(BLD)/$(EXE).out

cleanSh:
	rm -f $(SPV)/*.spv

genXDG:
	@echo "compiling for wayland"

runningX11:
	@echo "compiling for X11"

symlink:
	ln $(BLD)/$(EXE).out ./$(EXE) -sf

send:
	scp $(BLD)/$(EXE).out root@$(sendIp):~

getBld:
	@echo "----------------------------------------------"
	@echo "target $(target): "
	@echo "----------------------------------------------"
	@echo "defines $(defineFlags)"
	@echo "----------------------------------------------"
	@echo "compiler $(compiler) VAR not used currently"
	@echo "----------------------------------------------"
	@echo "linker $(linkerFlags)"
	@echo "----------------------------------------------"
	@echo "action $(action)"
	@echo "----------------------------------------------"

-include $(DEP)/*
