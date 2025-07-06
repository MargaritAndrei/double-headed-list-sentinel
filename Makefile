EXEC = tema1

SRC = main.c
OBJ = $(SRC:.c=.o)

build:
	gcc -Wall -Wextra -o $(EXEC) $(SRC)

run: build
	./$(EXEC)

clean:
	rm -f $(EXEC) *.o