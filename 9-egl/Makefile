
#CC = g++
CC = gcc

COMPILER_FLAGS = -w

LINKER_FLAGS = -lwayland-client -lEGL -lwayland-egl

OBJ_NAME3 = window

all:
	$(CC) $(OBJ_NAME3).c $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME3)

clean:
	rm -rf $(OBJ_NAME3)
