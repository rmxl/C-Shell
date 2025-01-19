CC = gcc
TARGET = a.out

SRCS = *.c

$(TARGET): $(SRCS)
	$(CC) -g -Wall -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
