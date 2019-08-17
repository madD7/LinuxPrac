#
ifeq ($(OS), Windows_NT)
	exit
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


CC_PATH = 
CC = $(CC_PATH)gcc
CC_FLAGS = -Wall -Wextra
LD_FLAGS = 

ROOTDIR_PATH := $(LINUX_DEVDIR)/Framework
BUILDDIR_PATH := $(ROOTDIR_PATH)/Exe
SRCDIR_PATH := $(ROOTDIR_PATH)/Src/KeyGen
INCDIR_PATH := $(ROOTDIR_PATH)/Inc
OBJDIR_PATH := $(ROOTDIR_PATH)/Obj
LIB:=libkeygen.a

CC_FLAGS += -I $(INCDIR_PATH)

ifeq ($(ARCH), 32)
	CC_FLAGS += -m32
endif

ifeq ($(DEBUG), 1)
CC_FLAGS += -g
endif

LD_FLAGS := -L$(BUILDDIR_PATH) -llog 

ifeq ($(TESTMODULE), 1)
OUTFILE = key
CC_FLAGS += -D__TESTING__
CMD = $(CC) $(CC_FLAGS) $(OBJS) $(LD_FLAGS) -o $(BUILDDIR_PATH)/$(OUTFILE)
else
OUTFILE = $(LIB)
CMD = ar -r $(BUILDDIR_PATH)/$(OUTFILE) -o $^
endif


VPATH := $(SRCDIR_PATH)

SRCS := $(wildcard $(SRCDIR_PATH)/*.c) 
OBJS := $(patsubst $(SRCDIR_PATH)/%.c, $(OBJDIR_PATH)/%.o, $(SRCS))

.PHONY: all clean build

all: clean makdir build
makdir:
	mkdir -p $(OBJDIR_PATH)
	mkdir -p $(BUILDDIR_PATH)

# Automatic Variables
# $@ - Contains target file name
# $< - Contains 1st dependency file name
$(OBJDIR_PATH)/%.o: $(SRCDIR_PATH)/%.c
		$(CC) -c $(CC_FLAGS) $(LD_FLAGS) $< -o $@

build: $(OBJS)
	$(CMD)
		
clean:
	rm -f $(OBJS) $(BUILDDIR_PATH)/$(OUTFILE)

help:
	@echo "Targets:"
	@echo "all - Complete build"
	@echo "clean - clean all build files"
	@echo "Use DEBUG=1 - To compile with debugging enabled"
	@echo "Use TESTMODULE=1 - To generate exe to test the module code"

