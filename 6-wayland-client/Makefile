
#CC = g++
CC = gcc

COMPILER_FLAGS = -w

LINKER_FLAGS = -lwayland-client

OBJ_NAME1 = connect
OBJ_NAME2 = registry
OBJ_NAME3 = surface

all:
	$(CC) $(OBJ_NAME1).c $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME1)
	$(CC) $(OBJ_NAME2).c $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME2)
	$(CC) $(OBJ_NAME3).c $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME3)

clean:
	rm -rf $(OBJ_NAME1)
	rm -rf $(OBJ_NAME2)
	rm -rf $(OBJ_NAME3)