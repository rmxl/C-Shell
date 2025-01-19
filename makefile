CC = gcc
TARGET = a.out

SRCS = color_print.c execute.c get_input.c hop.c linkedlist.c log.c parser.c proclore.c reveal.c seek.c shell_prompt.c 

$(TARGET): $(SRCS)
	$(CC) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
