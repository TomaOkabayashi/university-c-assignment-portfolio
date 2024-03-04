CC = gcc
CFLAGS = -Wall -Werror
OBJ = main.o file_io.o game_logic.o
EXEC = traffic


$(EXEC) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

main.o : main.c file_io.c game_logic.c
	$(CC) $(CFLAGS) -c main.c

file_io.o : file_io.c file_io.h
	$(CC) $(CFLAGS) -c file_io.c

game_logic.o : game_logic.c game_logic.h
	$(CC) $(CFLAGS) -c game_logic.c

clean : 
	rm -f $(EXEC) $(OBJ)
