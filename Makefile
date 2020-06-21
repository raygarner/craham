FLAGS=-O3
CC=gcc
EXES=opening selfplay userselfplay
SRC=util.c search.c eval.c ui.c init.c

opening: $(SRC) opening.c
	$(CC) $(FLAGS) $? -o opening

clean:
	rm -f $(EXES) *.o
