CC = gcc
CFLAGS = -Wall -Wextra -ggdb
LINK = -lGL -lGLU -lglut -lm -lIL

main: main.o model.o grid.o setup.o movement.o
	$(CC) $(CFLAGS) $(LINK) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

model.o: model.c model.h 
	$(CC) $(CFLAGS) -c -o $@ $<

grid.o: grid.c grid.h
	$(CC) $(CFLAGS) -c -o $@ $<

setup.o: setup.c setup.h
	$(CC) $(CFLAGS) -c -o $@ $<

movement.o: movement.c movement.h
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm *.o	