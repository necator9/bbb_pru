
BUILDDIR = build
TARGET = $(BUILDDIR)/ov7670
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default


OBJECTS = $(patsubst %.c, $(BUILDDIR)/%.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

$(BUILDDIR)/%.o: %.c $(HEADERS)
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -rf $(BUILDDIR)

run: $(TARGET)
	./$(TARGET)

