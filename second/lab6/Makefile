
CC=gcc
CFLAGS=-c -Wall -std=c99 -D_POSIX_PTHREAD_SEMANTICS -D__EXTENSIONS__
LDFLAGS=-lm -lpthread
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=lab

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

clear: clean

rebuild: clean all

