CC = gcc
CFLAGS = -std=c17 -Wextra -Werror -Wall -Wno-unused-parameter

TARGET = traceroute

SRCS = network.c check.c main.c
OBJS = network.o check.o main.o

.PHONY: clean, distclean

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

clean:
		rm $(OBJS) 

distclean:
		rm $(OBJS) $(TARGET)