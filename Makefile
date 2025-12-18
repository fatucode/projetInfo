CC=gcc
CFLAGS=-Wall -Wextra -Iinclude
SRC=src/main.c src/usine.c src/avl_usine.c src/histogramme.c src/csv_reader.c
OBJ=$(SRC:.c=.o)

wildwater: $(OBJ)
	$(CC) $(CFLAGS) -o wildwater $(OBJ)

clean:
	rm -f $(OBJ) wildwater
