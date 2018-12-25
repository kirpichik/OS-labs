
CC=gcc
CFLAGS=-c -O2 -Wall -std=c99 -D__EXTENSIONS__ -D_REENTRANT
LDFLAGS=-lm -lpthread -lrt
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

