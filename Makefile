# Sources
SRC_DIR = .
OBJS = $(foreach dir,$(SRC_DIR),$(subst .c,.o,$(wildcard $(dir)/*.c)))

# Compiler Settings
OUTPUT = POTC3D_Rehash
CFLAGS = -O2 -flto -Wall -Wno-unused-variable -I. -std=c11
CC = gcc
ifeq ($(OS),Windows_NT)
    #Windows Build CFG
    CFLAGS += -Wno-unused-but-set-variable
    LIBS += -static-libgcc -static-libstdc++
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        # OS X
        CFLAGS +=
        LIBS += -liconv
    else
        # Linux
        CFLAGS += -Wno-unused-but-set-variable
        LIBS +=
    endif
endif

main: $(OBJS)
	$(CC) -s -o $(OUTPUT) $(LIBS) $(OBJS)

clean:
	rm -rf $(OUTPUT) $(OUTPUT).exe $(OBJS)
