CC = gcc

CFLAGS  = -g -Wall
TARGET = read_gpio

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)	

run: $(TARGET)
	./$(TARGET)