CC = gcc
CFLAGS ?= -Wall -Wextra -g
CPPFLAGS ?=
LDFLAGS ?=

SOURCES = main.c openbmp.c grayscale.c brightness.c invert.c flip.c rotate.c crop.c blur.c sharpen.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
CC = C:/msys64/mingw64/bin/gcc.exe
IUP_ROOT ?= C:/iup
CPPFLAGS += -I$(IUP_ROOT)/include
IUP_LIBS ?= -L$(IUP_ROOT) -liup -lgdi32 -luser32 -lole32 -luuid -lcomdlg32 -lcomctl32 -lwinspool -lshell32 -ladvapi32 -lwinmm
TARGET = photoeditor.exe
RUN = .\$(TARGET)
RM = del /Q
else
IUP_CFLAGS ?= $(shell pkg-config --cflags iup 2>/dev/null)
IUP_LIBS ?= $(shell pkg-config --libs iup 2>/dev/null || printf '%s' '-liup')
CPPFLAGS += $(IUP_CFLAGS)
TARGET = photoeditor
RUN = ./$(TARGET)
RM = rm -f
endif

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(IUP_LIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	$(RUN)

clean:
	$(RM) $(OBJECTS) $(TARGET)
