#
ifeq ($(OS), Windows_NT)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		OS=Lin
		UNAME_M := $(shell uname -m)
		ifeq ($(UNAME_M), x86_64)
			ARCH = 64
		else
			ARCH = 32
		endif
	endif
endif


DEBUG = 0

CC_PATH = 
CC = $(CC_PATH)gcc
CC_FLAGS = -Wall -Wextra -I$(INCDIR_PATH)
LD_FLAGS = 

ROOTDIR_PATH= .
BUILDDIR_PATH= $(ROOTDIR_PATH)/Build
SRCDIR_PATH= $(ROOTDIR_PATH)/Src
INCDIR_PATH= $(ROOTDIR_PATH)/Inc
OBJDIR_PATH= $(ROOTDIR_PATH)/Obj
EXE=output_$(OS)$(ARCH)


ifeq ($(ARCH), 32)
	CC_FLAGS := $(CC_FLAGS) -m32
endif

ifeq ($(OS), Lin)
	EXE := $(EXE).out
endif

VPATH := $(SRCDIR_PATH)

SRCS = main.c RandomFunctions.c 
OBJS = $(patsubst %.c, $(OBJDIR_PATH)/%.o, $(SRCS))


.PHONY: all clean build

all : clean makdir build
makdir:
	mkdir -p $(OBJDIR_PATH)
	mkdir -p $(BUILDDIR_PATH)

# Automatic Variables
# $@ - Contains target file name
# $< - Contains 1st dependency file name
$(OBJDIR_PATH)/%.o: $(SRCDIR_PATH)/%.c
		$(CC) -c $(CC_FLAGS) $< -o $@

build:	$(OBJS)
	$(CC) $(CC_FLAGS) $(OBJS) $(LD_FLAGS) -o $(BUILDDIR_PATH)/$(EXE) 
		
clean :
	rm -f $(OBJS) $(EXE)


