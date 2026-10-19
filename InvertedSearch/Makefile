CC     = gcc
CFLAGS = -Wall
TARGET = inverted
SRCS   = main.c create.c display.c inverted.c save.c search.c update.c

$(TARGET): $(SRCS) inverted.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	./$(TARGET) f1.txt f2.txt f3.txt

clean:
	rm -f $(TARGET) a.out *.o

.PHONY: run clean
