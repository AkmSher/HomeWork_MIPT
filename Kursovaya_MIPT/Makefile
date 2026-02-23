CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = temperature_stats
SOURCES = main.c temp_functions.c
HEADERS = temp_functions.h
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET) -h

.PHONY: all clean run
