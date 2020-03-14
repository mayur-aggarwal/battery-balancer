LIBS = -lpthread
CC = gcc
all:
	$(CC) -o simulation main.c process.c $(LIBS)

clean:
	rm -rf *.out